/*
 * Temp_controller.c
 *
 *  Created on: Feb 26, 2025
 *      Author: Asus
 */
#include "main.h"
#include "stdint.h"
#include <stdbool.h>
#include "main_data.h"
#include "Temp_controller.h"
#include "BMS_test_protocol.h"
#include <math.h>

#define R25    10000.0f     // 10kΩ at 25°C
#define BETA   3984.0f      // Beta constant
#define T0     (25.0f + 273.15f)  // 25°C in Kelvin

volatile int debug_success = 0;

volatile int debug_failure = 0;

//int res = 0;

float temp = 0;

volatile float Readback_Temp = 0.0f;

uint16_t readback_rdac_value=0;

uint8_t receivedReadbackRDAC[2] = {0};

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

uint8_t controlArray [2] = {0x1C,0x03};
uint8_t nopCommand [2]  = {0x00,0x00};
uint8_t dataRead [2]  = {0x08,0x00};
uint8_t memoryCommand [2] = {0x0C,0x00};
//uint8_t memoryRead [2] = {0x02,0x00};
uint8_t readRDACCommand[2]= {0x08,0x00};
uint8_t writeRDACCommand[2];   // Will be populated with 0x04XX
uint8_t writeControlRegCommand[2];        // Will be populated with 0x1CYY

uint8_t misoCell12Res1 [2];
uint8_t misoCell12Res2 [2];
uint8_t misoCell12Res3 [2];
uint8_t misoCell11Res1 [2];
uint8_t misoCell11Res2 [2];
uint8_t misoCell11Res3 [2];

void intToBinary(int num, int *binaryArray) {
		    for (int i = 0; i < 10; i++) {
		        binaryArray[15 - i] = (num >> i) & 1;
		    }
		}

void assignBytes(int *resArray, uint8_t *r) {
		    r[0] = 0;
		    r[1] = 0;

		    for (int i = 0; i < 8; i++) {

		        r[0] |= (resArray[i] << (7 - i));
		        r[1] |= (resArray[i + 8] << (7 - i));
		    }
		}


bool arrayComparison (uint8_t *array1,uint8_t *array2){
		for (int i = 0;i<2;i++){
			if (array1[i]!=array2[i]){
				return false;
			}


		}
		return true;

	}


void TempCard_Set_Resistance(uint8_t id, float resistance)
{
    Temp_Card_Config *t = &temp_cards[id];
    uint16_t rdac = (uint16_t)((resistance / 50000.0f) * 1024.0f);

    uint8_t writeRDAC[2];
    writeRDAC[0] = 0x04 | ((rdac >> 8) & 0x03);
    writeRDAC[1] = rdac & 0xFF;

    HAL_GPIO_WritePin(t->led_port, t->led_pin, GPIO_PIN_SET);

    // Send NOP
    HAL_GPIO_WritePin(t->cs_port, t->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(t->spi, (uint8_t*)&nopCommand, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(t->cs_port, t->cs_pin, GPIO_PIN_SET);

    HAL_Delay(10);

    // Send control array
    HAL_GPIO_WritePin(t->cs_port, t->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(t->spi, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(t->cs_port, t->cs_pin, GPIO_PIN_SET);

    HAL_Delay(10);

    // Write RDAC
    HAL_GPIO_WritePin(t->cs_port, t->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(t->spi, writeRDAC, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(t->cs_port, t->cs_pin, GPIO_PIN_SET);

    HAL_Delay(10);

    HAL_GPIO_WritePin(t->led_port, t->led_pin, GPIO_PIN_RESET);
}

float Get_TempCard_TempC(uint8_t id)
{   Temp_Card_Config *t = &temp_cards[id];

	HAL_GPIO_WritePin(t->led_port, t->led_pin, GPIO_PIN_SET);

	// Send NOP
	HAL_GPIO_WritePin(t->cs_port, t->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(t->spi, (uint8_t*)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(t->cs_port, t->cs_pin, GPIO_PIN_SET);

	HAL_Delay(10);

	// Read RDAC back
	HAL_GPIO_WritePin(t->cs_port, t->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(t->spi, readRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(t->cs_port, t->cs_pin, GPIO_PIN_SET);

	HAL_Delay(10);

	// Get response
	uint8_t rx[2];
	HAL_GPIO_WritePin(t->cs_port, t->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(t->spi, nopCommand, rx, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(t->cs_port, t->cs_pin, GPIO_PIN_SET);

	uint16_t rdac_value = ((rx[0] << 8) | rx[1]) & 0x03FF;
	readback_rdac_value = rdac_value;

	temp = convert_resistance_to_temp_c(readback_rdac_value);

	HAL_Delay(10);

	HAL_GPIO_WritePin(t->led_port, t->led_pin, GPIO_PIN_RESET);
	return temp;
}

void cell12_Temp_01_Set(float resistance){
//	int res = (resistance/50.0)*1024;
//	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
//	uint8_t resByteArray [2];
//	intToBinary(res, resArray);
//	assignBytes(resArray, resByteArray);

	int res = (resistance/50000.0)*1024;//50kOhm
	writeRDACCommand[0] = 0x04 | ((res >> 8) & 0x03); // Top 2 bits (D9, D8) are 0x03 mask
	writeRDACCommand[1] = (uint8_t)(res & 0xFF); // Lower 8 bits (D7-D0)


	HAL_GPIO_WritePin(CELL12_TEMP_01_LED_GPIO_Port, CELL12_TEMP_01_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi1, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, writeRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
//	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&dataRead, (uint8_t*)&misoCell12Res1, 2, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, readRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, nopCommand, receivedReadbackRDAC, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CELL12_TEMP_01_LED_GPIO_Port, CELL12_TEMP_01_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

	uint16_t readback_16bit_word = (receivedReadbackRDAC[0] << 8) | receivedReadbackRDAC[1];
	readback_rdac_value = readback_16bit_word & 0x03FF;
}

void cell12_Temp_02_Set(float resistance){

	int res = (resistance/50000.0)*1024;

	writeRDACCommand[0] = 0x04 | ((res >> 8) & 0x03); // Top 2 bits (D9, D8) are 0x03 mask
	writeRDACCommand[1] = (uint8_t)(res & 0xFF); // Lower 8 bits (D7-D0)

	HAL_GPIO_WritePin(CELL12_TEMP_02_LED_GPIO_Port, CELL12_TEMP_02_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, writeRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, readRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, nopCommand, receivedReadbackRDAC, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(CELL12_TEMP_02_LED_GPIO_Port, CELL12_TEMP_02_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

	uint16_t readback_16bit_word = (receivedReadbackRDAC[0] << 8) | receivedReadbackRDAC[1];
	readback_rdac_value = readback_16bit_word & 0x03FF;
}

void cell12_Temp_03_Set(float resistance){

	int res = (resistance/50000.0)*1024;

	writeRDACCommand[0] = 0x04 | ((res >> 8) & 0x03); // Top 2 bits (D9, D8) are 0x03 mask
	writeRDACCommand[1] = (uint8_t)(res & 0xFF); // Lower 8 bits (D7-D0)

	HAL_GPIO_WritePin(CELL12_TEMP_03_LED_GPIO_Port, CELL12_TEMP_03_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, writeRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, readRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, nopCommand, receivedReadbackRDAC, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(CELL12_TEMP_03_LED_GPIO_Port, CELL12_TEMP_03_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

	uint16_t readback_16bit_word = (receivedReadbackRDAC[0] << 8) | receivedReadbackRDAC[1];
	readback_rdac_value = readback_16bit_word & 0x03FF;
}

void cell11_Temp_01_Set(float resistance){

	int res = (resistance/50000.0)*1024;

	writeRDACCommand[0] = 0x04 | ((res >> 8) & 0x03); // Top 2 bits (D9, D8) are 0x03 mask
	writeRDACCommand[1] = (uint8_t)(res & 0xFF); // Lower 8 bits (D7-D0)

	HAL_GPIO_WritePin(CELL11_TEMP_01_LED_GPIO_Port, CELL11_TEMP_01_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, writeRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, readRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, nopCommand, receivedReadbackRDAC, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(CELL11_TEMP_01_LED_GPIO_Port, CELL11_TEMP_01_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

	uint16_t readback_16bit_word = (receivedReadbackRDAC[0] << 8) | receivedReadbackRDAC[1];
	readback_rdac_value = readback_16bit_word & 0x03FF;
}

void cell11_Temp_02_Set(float resistance){

	int res = (resistance/50000.0)*1024;

	writeRDACCommand[0] = 0x04 | ((res >> 8) & 0x03); // Top 2 bits (D9, D8) are 0x03 mask
	writeRDACCommand[1] = (uint8_t)(res & 0xFF); // Lower 8 bits (D7-D0)

	HAL_GPIO_WritePin(CELL11_TEMP_02_LED_GPIO_Port, CELL11_TEMP_02_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, writeRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, readRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, nopCommand, receivedReadbackRDAC, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(CELL11_TEMP_02_LED_GPIO_Port, CELL11_TEMP_02_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

	uint16_t readback_16bit_word = (receivedReadbackRDAC[0] << 8) | receivedReadbackRDAC[1];
	readback_rdac_value = readback_16bit_word & 0x03FF;
}

void cell11_Temp_03_Set(float resistance){

	int res = (resistance/50000.0)*1024;

	writeRDACCommand[0] = 0x04 | ((res >> 8) & 0x03); // Top 2 bits (D9, D8) are 0x03 mask
	writeRDACCommand[1] = (uint8_t)(res & 0xFF); // Lower 8 bits (D7-D0)

	HAL_GPIO_WritePin(CELL11_TEMP_03_LED_GPIO_Port, CELL11_TEMP_03_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, writeRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, readRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, nopCommand, receivedReadbackRDAC, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(CELL11_TEMP_03_LED_GPIO_Port, CELL11_TEMP_03_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

	uint16_t readback_16bit_word = (receivedReadbackRDAC[0] << 8) | receivedReadbackRDAC[1];
	readback_rdac_value = readback_16bit_word & 0x03FF;
}

//void cell11_Temp_03_Set(float resistance){
//	int res = (resistance/50.0)*1024;
//	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
//	uint8_t resByteArray [2];
//	intToBinary(res, resArray);
//	assignBytes(resArray, resByteArray);
//	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_LED_Pin, GPIO_PIN_SET);
//	HAL_Delay(10);
//	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
//	HAL_Delay(10);
//	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi2, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
//	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
//	HAL_Delay(10);
//	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi2, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
//	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
//	HAL_Delay(10);
//	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi2, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
//	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
//	HAL_Delay(10);
//	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
//	HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)&dataRead, (uint8_t*)&misoCell12Res1, 2, HAL_MAX_DELAY);
//	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
//	HAL_Delay(10);
//	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_LED_Pin, GPIO_PIN_RESET);
//	HAL_Delay(10);
//
//}


void cell12_Temp_01_startup(float resistance){
	int res = (resistance/50000.0)*1024;
//	int res = (resistance/50.0)*1024;
//	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
//	uint8_t resByteArray [2];
//	intToBinary(res, resArray);
//	assignBytes(resArray, resByteArray);

	writeRDACCommand[0] = 0x04 | ((res >> 8) & 0x03);
	writeRDACCommand[1] = (uint8_t)(res & 0xFF);

	HAL_GPIO_WritePin(CELL12_TEMP_01_LED_GPIO_Port, CELL12_TEMP_01_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

//	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi1, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
//	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
//	HAL_Delay(10);
//	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi1, (uint8_t*)&memoryCommand, 2, HAL_MAX_DELAY);
//	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
//	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, writeRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&memoryCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	/*HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&memoryRead, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);*/

	HAL_GPIO_WritePin(CELL12_TEMP_01_LED_GPIO_Port, CELL12_TEMP_01_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}

void cell12_Temp_02_startup(float resistance){
	int res = (resistance/50000.0)*1024;
	writeRDACCommand[0] = 0x04 | ((res >> 8) & 0x03);
	writeRDACCommand[1] = (uint8_t)(res & 0xFF);

	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_02_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, writeRDACCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&memoryCommand,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	/*HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&memoryRead,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);*/
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_02_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}
void cell12_Temp_03_startup(float resistance){
	int res = (resistance/50000.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
	HAL_GPIO_WritePin(GPIOA, CELL12_TEMP_03_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&memoryCommand,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	/*HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&memoryRead,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);*/
	HAL_GPIO_WritePin(GPIOA, CELL12_TEMP_03_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}

void cell11_Temp_01_startup(float resistance){
	int res = (resistance/50000.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&memoryCommand,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	/*HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&memoryRead,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);*/
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}
void cell11_Temp_02_startup(float resistance){
	int res = (resistance/50000.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&memoryCommand,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	/*HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&memoryRead,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);*/
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}



void cell11_Temp_03_startup(float resistance){
	int res = (resistance/50000.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&memoryCommand,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	/*HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&memoryRead,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);*/
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}


void Set_Resistance(uint8_t tempCardId,float temperature)

{
	switch (tempCardId)
	{
	case 0x01:
		cell12_Temp_01_Set(temperature);
		break;

	case 0x02:
		cell12_Temp_02_Set(temperature);
		break;

	case 0x03:
		cell12_Temp_03_Set(temperature);
		break;

	case 0x04:
		cell11_Temp_01_Set(temperature);
		break;

	case 0x05:
		cell11_Temp_02_Set(temperature);
		break;

	case 0x06:
		cell11_Temp_03_Set(temperature);
		break;
	}
}


float R = 0;

// Calculate NTC resistance for a given temperature (°C)
float ntc_resistance(float temp_C) {
    float T = temp_C + 273.15f;  // Convert to Kelvin
    R = R25 * expf(BETA * ((1.0f / T) - (1.0f / T0)));
    return R;
}


volatile float temperature_celsius = 0;

volatile float readback_resistance = 0;

float convert_resistance_to_temp_c( uint16_t readback_rdac_value) {

//	readback_rdac_value =204;

//	res = (resistance/50000.0)*1024;

	readback_resistance = ((float)readback_rdac_value*50000.0f)/1024.0f;

 // Ensure resistance_ohms is positive to avoid logf(negative)

 if (readback_resistance <= 0.0f) {

 return -999.0f;

 }

 float T_pot = 1.0f / (logf(readback_resistance / R25) / BETA + (1.0f / T0));

 // Ensure one_over_T is not zero or negative (would indicate invalid resistance)

 if (T_pot <= 0.0f) {

 return -888.0f;

 }





 // Step 4: Convert T from Kelvin to Celsius

 temperature_celsius = T_pot - 273.15f;

 Readback_Temp = temperature_celsius;

 return temperature_celsius;

}
