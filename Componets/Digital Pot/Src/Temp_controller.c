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

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

uint8_t controlArray [2]= {0x1C,0x03};
uint8_t nopCommand [2] = {0x00,0x00};
uint8_t dataRead [2] = {0x08,0x00};
uint8_t memoryCommand [2] = {0x0C,0x00};
uint8_t memoryRead [2] = {0x02,0x00};
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

void cell12_Temp_01_Set(float resistance){
	int res = (resistance/50.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
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
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&dataRead, (uint8_t*)&misoCell12Res1, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CELL12_TEMP_01_LED_GPIO_Port, CELL12_TEMP_01_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}

void cell12_Temp_02_Set(float resistance){
	int res = (resistance/50.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
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
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&dataRead, (uint8_t*)&misoCell12Res1, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_02_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}

void cell12_Temp_03_Set(float resistance){
	int res = (resistance/50.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_LED_Pin, GPIO_PIN_SET);
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
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&dataRead, (uint8_t*)&misoCell12Res1, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}

void cell11_Temp_01_Set(float resistance){
	int res = (resistance/50.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_LED_Pin, GPIO_PIN_SET);
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
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)&dataRead, (uint8_t*)&misoCell12Res1, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}

void cell11_Temp_02_Set(float resistance){
	int res = (resistance/50.0)*1024;
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
	HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)&dataRead, (uint8_t*)&misoCell12Res1, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}

void cell11_Temp_03_Set(float resistance){
	int res = (resistance/50.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)&dataRead, (uint8_t*)&misoCell12Res1, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}


void cell12_Temp_01_startup(float resistance){
	int res = (resistance/50.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
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
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&memoryCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	/*HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&memoryRead, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);*/
	HAL_GPIO_WritePin(CELL12_TEMP_01_LED_GPIO_Port, CELL12_TEMP_01_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}

void cell12_Temp_02_startup(float resistance){
	int res = (resistance/50.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
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
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
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
	int res = (resistance/50.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_LED_Pin, GPIO_PIN_SET);
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
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&memoryCommand,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	/*HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&memoryRead,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);*/
	HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}

void cell11_Temp_01_startup(float resistance){
	int res = (resistance/50.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_LED_Pin, GPIO_PIN_SET);
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
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&memoryCommand,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	/*HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&memoryRead,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);*/
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

}
void cell11_Temp_02_startup(float resistance){
	int res = (resistance/50.0)*1024;
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
	int res = (resistance/50.0)*1024;
	int resArray [16]= {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	uint8_t resByteArray [2];
	intToBinary(res, resArray);
	assignBytes(resArray, resByteArray);
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t *)&nopCommand, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&controlArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&resByteArray, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&memoryCommand,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	/*HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&memoryRead,  2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(CELL11_TEMP_03_CS_GPIO_Port, CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);*/
	HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_LED_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

}


void Set_Resistance(uint8_t tempCardId,uint8_t temperature)
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












