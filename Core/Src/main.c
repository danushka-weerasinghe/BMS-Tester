/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "main_data.h"

#include "display.h"

#include "Temp_controller.h"


#include "INA229.h"
#include  "config.h"
#include "mcu.h"
#include <math.h>


#include "flash_LTC.h"
#include "LTC6811.h"
#include "LTC681x.h"
#include <stdlib.h>

#include "gpio_expander.h"
#include "rtc.h"

//#include "../../Componets/GPIO expander/gpio_expander.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

uint8_t address;
HAL_StatusTypeDef result;

int count =0 ;

#define UI_BUFFER_SIZE 32
#define CRC16_CCITT 0x1021
#define debug
#define BAUD    115200
#define T_char  750
#define T_frame 1750

float busVoltage_01 = 0 ;
float temperatureC_01 = 0 ;

float busVoltage_02 = 0 ;
float temperatureC_02 = 0 ;

float busVoltage_03 = 0 ;
float temperatureC_03 = 0 ;


float busVoltage_04 = 0 ;
float temperatureC_04 = 0 ;

float busVoltage_05 = 0 ;
float temperatureC_05 = 0 ;


float busVoltage_06 = 0 ;
float temperatureC_06 = 0 ;

float busVoltage_07 = 0 ;
float temperatureC_07 = 0 ;

float busVoltage_08 = 0 ;
float temperatureC_08 = 0 ;

float busVoltage_09 = 0 ;
float temperatureC_09 = 0 ;


float busVoltage_10 = 0 ;
float temperatureC_10 = 0 ;

float busVoltage_11 = 0 ;
float temperatureC_11 = 0 ;


float busVoltage_12 = 0 ;
float temperatureC_12 = 0 ;

float busVoltage_13 = 0 ;
float temperatureC_13 = 0 ;

float busVoltage_14 = 0 ;
float temperatureC_14 = 0 ;


float busVoltage_15 = 0 ;
float temperatureC_15 = 0 ;

float busVoltage_16 = 0 ;
float temperatureC_16 = 0 ;


float busVoltage_17 = 0 ;
float temperatureC_17 = 0 ;

float busVoltage_18 = 0 ;
float temperatureC_18 = 0 ;


float busVoltage_19 = 0 ;
float temperatureC_19 = 0 ;

float busVoltage_20 = 0 ;
float temperatureC_20 = 0 ;

float busVoltage_21 = 0 ;
float temperatureC_21 = 0 ;


float busVoltage_22 = 0 ;
float temperatureC_22 = 0 ;

float busVoltage_23 = 0 ;
float temperatureC_23 = 0 ;


float busVoltage_24 = 0 ;
float temperatureC_24 = 0 ;

float busVoltage_25 = 0 ;
float temperatureC_25 = 0 ;

float busVoltage_26 = 0 ;
float temperatureC_26 = 0 ;


#define NUM_INA229 26


 INA229_State* ina229_devices[NUM_INA229] = {&INA229_0, &INA229_1, &INA229_2, &INA229_3, &INA229_4, &INA229_5,
								&INA229_6, &INA229_7, &INA229_8, &INA229_9, &INA229_10,
								&INA229_11, &INA229_12, &INA229_13, &INA229_14, &INA229_15,
								&INA229_16, &INA229_17, &INA229_18, &INA229_19,
								&INA229_20, &INA229_21, &INA229_22, &INA229_23, &INA229_24, &INA229_25};


static const signed char TemperatureDataTable[] = { 100, 98, 93, 91, 87, 85, 83,
		81, 80, 78, 77, 76, 74, 73, 72, 71, 70, 69, 68, 67, 67, 66, 65, 64, 63,
		63, 62, 61, 60, 60, 59, 58, 58, 57, 56, 56, 55, 54, 54, 53, 53, 52, 51,
		51, 50, 50, 49, 49, 48, 47, 47, 46, 46, 45, 45, 44, 44, 43, 43, 42, 42,
		41, 41, 40, 40, 39, 39, 38, 38, 37, 37, 36, 36, 36, 35, 35, 34, 34, 33,
		33, 32, 32, 32, 31, 31, 30, 30, 29, 29, 29, 28, 28, 27, 27, 27, 26, 26,
		25, 25, 24, 24, 24, 23, 23, 23, 22, 22, 21, 21, 21, 20, 20, 19, 19, 19,
		18, 18, 18, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 12, 12,
		12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5,
		4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, -1, -1, -1, -2, -2, -2, -3,
		-3, -3, -4, -4, -4, -5, -5, -5, -5, -6, -6, -6, -7, -7, -7, -8, -8, -8,
		-9, -9, -9, -10, -10, -10, -10, -11, -11, -11, -12, -12, -12, -13, -13,
		-13, -13, -14, -14, -14, -15, -15, -15, -16, -16, -16, -16, -25 };

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;
SPI_HandleTypeDef hspi4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

//temp data
int resistance [6]= {10,10,10,10,10,10};


//iso spi data
cell_asic BMS_IC[TOTAL_IC];
setup_data_sv setup_data;

uint8_t               TxData[8];
uint32_t              TxMailbox;

const uint8_t ADC_OPT = ADC_OPT_DISABLED; //!< ADC Mode option bit
const uint8_t ADC_CONVERSION_MODE = MD_7KHZ_3KHZ; //!< ADC ModeMD_27KHZ_14KHZ;/
const uint8_t ADC_DCP = DCP_DISABLED; //!< Discharge Permitted
const uint8_t CELL_CH_TO_CONVERT = CELL_CH_ALL; //!< Channel Selection for ADC conversion
const uint8_t AUX_CH_TO_CONVERT = AUX_CH_ALL; //!< Channel Selection for ADC conversion
const uint8_t STAT_CH_TO_CONVERT = STAT_CH_ALL; //!< Channel Selection for ADC conversion
const uint8_t SEL_ALL_REG = REG_ALL; //!< Register Selection
const uint8_t SEL_REG_A = REG_1; //!< Register Selection
const uint8_t SEL_REG_B = REG_2; //!< Register Selection
const uint16_t OV_THRESHOLD = 41000; //!< Over voltage threshold ADC Code. LSB = 0.0001 ---(4.1V)
const uint16_t UV_THRESHOLD = 30000; //!< Under voltage threshold ADC Code. LSB = 0.0001 ---(3V)

unsigned char REF_ON = true; //!< Reference Powered Up Bit
unsigned char ADCOPT = false; //!< ADC Mode option bit
unsigned char GPIOBITS_A[5] = { false, false, true, true, true }; //!< GPIO Pin Control // Gpio 1,2,3,4,5
uint16_t UV = UV_THRESHOLD; //!< Under-voltage Comparison Voltage
uint16_t OV = OV_THRESHOLD; //!< Over-voltage Comparison Voltage
unsigned char DCCBITS_A[12] = { false, false, false, false, false, false, false, false,
		false, false, false, false }; //!< Discharge cell switch //Dcc 1,2,3,4,5,6,7,8,9,10,11,12
unsigned char DCTOBITS[4] = { true, false, true, false }; //!< Discharge time value // Dcto 0,1,2,3 // Programed for 4 min

/*
 * error data
 */
int8_t error_c = 0;
int8_t error_cell = 0;
int8_t error_aux = 0;
int8_t error_stat = 0;
int8_t adc_overlap_error = 0;
int8_t error_aux_dr = 0;
int8_t error_stat_dr = 0;
int16_t crc_error_count_gl = 0;

/*
 * Cell balancing data
 */
volatile uint8_t cell_balancing_enable = 0;
volatile uint8_t cell_balancing_enable_int = 0;
volatile uint8_t balancing_timer = 0;

/*
 * Highest & lowest data
 */

uint8_t highest_board_t = 0;
uint8_t highest_board_t_id = 0xFF;
uint8_t lowest_board_t = 0xFF;
uint8_t lowest_board_t_id = 0xFF;

int8_t highest_t = 0, lowest_t = 0xFF;
uint8_t highest_t_id = 0xFF;
uint8_t lowest_t_id = 0xFF;

uint16_t lowest_cell_id = 0xFFFF;
uint16_t highest_cell_id = 0xFFFF;
uint16_t lowest_cell_voltage = 0xFFFF;
uint16_t highest_cell_voltage = 0;

uint16_t total_pack_voltage = 0;

uint16_t globle_loweset_v = dCOV_THRESHOLD;

uint8_t env_board_t = 0;
uint8_t pwr_board_t = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_SPI3_Init(void);
static void MX_SPI4_Init(void);
static void MX_CAN1_Init(void);
static void MX_CAN2_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */

void Scan_I2C_Bus(void);
//--------------------------------------------------------------//
signed char tempDataTableRead(uint16_t tempe_v_in);


void cs_low();  //CS pin asserted
void cs_high(); //// Disable Pack, /CS deserted
void delay_time_us(uint32_t time_delay_val);

uint8_t spi_write_reg_analog(uint8_t data  //data byte
		);

void spi_transfer_byte(uint8_t cs_pin,      //!< Chip select pin
		uint8_t tx,          //!< Byte to be transmitted
		uint8_t *rx          //!< Byte to be received
		);

void spi_transfer_word(uint8_t cs_pin,      //!< Chip select pin
		uint16_t tx,         //!< Byte to be transmitted
		uint16_t *rx         //!< Byte to be received
		);

void spi_transfer_block(uint8_t cs_pin,     //!< Chip select pin
		uint8_t *tx,        //!< Byte array to be transmitted
		uint8_t *rx,        //!< Byte array to be received
		uint8_t length      //!< Length of array
		);

uint8_t spi_read_byte(uint8_t tx_dat);

void spi_write_array(uint8_t len, // Option: Number of bytes to be written on the SPI port
		uint8_t data[] //Array of bytes to be written on the SPI port
		);

void spi_write_read(uint8_t tx_Data[], //array of data to be written on SPI port
		uint8_t tx_len, //length of the tx data array
		uint8_t *rx_data, //Input: array that will store the data read by the SPI port
		uint8_t rx_len //Option: number of bytes to be read from the SPI port
		);
//--------------------------------------------------------------//

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//--------------------------------------------------------------//


void cs_low() {

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_SET);// Enable Pack, /CS asserted


}

void cs_high() {

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_SET);// Disable Pack, /CS deasserted


}

void delay_time_us(uint32_t time_delay_val) {
	uint32_t time_delay_count;
	uint32_t time_delay_val_temp = time_delay_val * 9;
	for (time_delay_count = 0; time_delay_count < time_delay_val_temp;
			time_delay_count++) {
		__NOP();
	}
}

/*
 * @brief Function Name:  spi_write_reg_analog .
 * @brief Description  :  Writes just 1 byte into the SPI BUS.
 * @param parameters   :  data
 * @return Value       :  return the data RX buffer
 */
uint8_t spi_write_reg_analog(uint8_t data_tx) {
	uint8_t data_rx = 0;
	if (HAL_SPI_TransmitReceive(&hspi4, &data_tx, &data_rx, 1, 20) != HAL_OK) {
		Error_Handler();
	}
	return data_rx;
}
/*
 * @brief Function Name:  spi_write_array
 * @brief Description  :  Writes an array of bytes out of the SPI port
 * @param parameters   :  Data length, data to transmit
 */

void spi_write_array(uint8_t len, // Option: Number of bytes to be written on the SPI port
		uint8_t data[] //Array of bytes to be written on the SPI port
		) {
	uint8_t len_t;
	for (len_t = 0; len_t < len; len_t++) {
		if (HAL_SPI_Transmit(&hspi4, &data[len_t], 1, 20) != HAL_OK) {
			/* Transfer error in transmission process */
			Error_Handler();
		}
	}

}
/*
 * @brief Function Name:  spi_write_read .
 * @brief Description  :  Writes and read a set number of bytes using the SPI port.
 * @param parameters   :  TX data array, TX data length, RX data pointer, number of bytes to be read from the SPI port
 * @return Value       :  return the data RX buffer
 */
uint8_t rx_len_local=0;
HAL_StatusTypeDef status_spi_out = HAL_OK;
HAL_StatusTypeDef status_spi_in = HAL_OK;
void spi_write_read(uint8_t tx_Data[], //array of data to be written on SPI port
		uint8_t tx_len, //length of the tx data array
		uint8_t *rx_data, //Input: array that will store the data read by the SPI port
		uint8_t rx_len //Option: number of bytes to be read from the SPI port
		) {
	status_spi_out = HAL_SPI_Transmit(&hspi4, tx_Data, tx_len, 20);
	rx_len_local = rx_len;
	if (status_spi_out != HAL_OK) {

		Error_Handler();
	}
	status_spi_in = HAL_SPI_Receive(&hspi4, rx_data, rx_len, 20);
	if (status_spi_in != HAL_OK) {

		Error_Handler();
	}

}

/*
 * @brief Function Name:  spi_read_byte .
 * @brief Description  :  Read just 1 byte using the SPI port
 * @param parameters   :  TX data array, TX data length, RX data pointer, number of bytes to be read from the SPI port
 * @return Value       :  return the data RX buffer
 */

uint8_t spi_read_byte(uint8_t tx_dat) {
	uint8_t data;
	if (HAL_SPI_TransmitReceive(&hspi4, &tx_dat, &data, 1, 20) != HAL_OK) {
		Error_Handler();
	}
	return (data);
}

//---------------------------------------------------------------------------------------------------------------------------//

/**
 * @brief Function Name: temparature_data_read.
 * @brief Description  : Read all LTC6811-1 temperature sensors
 */

void temparature_data_read()
{
   LTC6811_clraux();
   int8_t error = 0;
   int32_t time = 0;
   uint8_t i_1;
   uint32_t conv_time = 0;
   //-----------------------
   wakeup_sleep(TOTAL_IC);
   LTC6811_adax(ADC_CONVERSION_MODE, AUX_CH_ALL);
   conv_time = LTC6811_pollAdc();
   time += conv_time / 1000;
   //-----------------------
   wakeup_sleep(TOTAL_IC);
   error = LTC6811_rdaux(SEL_ALL_REG, TOTAL_IC, BMS_IC); // Set to read back all aux registers

   if (error == -1)
   {
       for (i_1 = 0; i_1 < TEMP_PER_IC; i_1++)
       {
           BMS_IC[0].heat.temp[i_1] = TemperatureDataTable[123];
           BMS_IC[1].heat.temp[i_1] = TemperatureDataTable[123];
       }
   }
   else
   {
       for (i_1 = 0; i_1 < TEMP_PER_IC; i_1++)
       {
           BMS_IC[0].heat.temp[i_1] = tempDataTableRead(
                   BMS_IC[0].aux.a_codes[i_1]);
           BMS_IC[1].heat.temp[i_1] = tempDataTableRead(
                               BMS_IC[1].aux.a_codes[i_1]);
       }
   }
}

/*
 * @Function : tempDataTableRead
 * @Description : per calculated temperature values for voltage readings
 */
signed char tempDataTableRead(uint16_t tempe_v_in)
{
    signed char temperature_val_tb;
    int temp_val_tp;
    temp_val_tp = (tempe_v_in >> 7) - 0x12;//0x30;
    if (temp_val_tp < 0)
    {
        temperature_val_tb = 105;
    }
    else if (temp_val_tp > 222)
    {
        temperature_val_tb = -25;
    }
    else
    {
        temperature_val_tb = TemperatureDataTable[temp_val_tp];
    }
    return temperature_val_tb;
}

int8_t cell_voltage_read(void) {
	int8_t error = 0;
	wakeup_sleep(TOTAL_IC);
	LTC6811_adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
	 LTC6811_pollAdc();
	wakeup_sleep(TOTAL_IC);
	error = LTC6811_rdcv(SEL_ALL_REG, TOTAL_IC, BMS_IC);
//	check_error(error);
	uint8_t i_1, current_ic_1;
	if (error == -1) {
		for (current_ic_1 = 0; current_ic_1 < TOTAL_IC; current_ic_1++) {
			for (i_1 = 0; i_1 < BMS_IC[0].ic_reg.cell_channels; i_1++) {
				BMS_IC[current_ic_1].cells.c_codes[i_1] = 0;
			}
		}
	}
	//cell_data_print(error, conv_time);
	return error;
}

/**
 * @brief Function Name: aux_data_read.
 * @brief Description  : Read all LTC6811-1 auxiliary voltages
 */
uint8_t aux_data_read(void) {
	int8_t error = 0;
	uint8_t i_1, current_ic_1;
	wakeup_sleep(TOTAL_IC);
	LTC6811_adax(ADC_CONVERSION_MODE, AUX_CH_TO_CONVERT);
	LTC6811_pollAdc();
	//----------------------------
	wakeup_sleep(TOTAL_IC);
	error = LTC6811_rdaux(SEL_ALL_REG, TOTAL_IC, BMS_IC); // Set to read back all aux registers
	if (error == -1) {

		for (current_ic_1 = 0; current_ic_1 < TOTAL_IC; current_ic_1++) {
			for (i_1 = 0; i_1 < 6; i_1++) {
				BMS_IC[current_ic_1].aux.a_codes[i_1] = 0xFFFF;
			}
		}
	}
	return error;
}

/**
 * @brief Function Name: stat_data_read.
 * @brief Description  : Read all LTC6811-1 cell status register data
 */
int8_t stat_data_read(void) {
	int8_t error = 0;
	wakeup_sleep(TOTAL_IC);
	LTC6811_adstat(ADC_CONVERSION_MODE, STAT_CH_TO_CONVERT);
	LTC6811_pollAdc();
	//----------------------------
	wakeup_sleep(TOTAL_IC);
	error = LTC6811_rdstat(SEL_ALL_REG, TOTAL_IC, BMS_IC); // Set to read back all stat registers
	return (error);
}

/*
 * @brief Function Name : adc_test_data_read
 * @brief Description	: ADC test whether is there ADC error
 */
int8_t adc_test_data_read() {
	wakeup_sleep(TOTAL_IC);
	error_cell = LTC6811_run_cell_adc_st(CELL, TOTAL_IC, BMS_IC,
			ADC_CONVERSION_MODE, ADCOPT);

	wakeup_sleep(TOTAL_IC);
	error_aux = LTC6811_run_cell_adc_st(AUX, TOTAL_IC, BMS_IC,
			ADC_CONVERSION_MODE, ADCOPT);

	wakeup_sleep(TOTAL_IC);
	error_stat = LTC6811_run_cell_adc_st(STAT, TOTAL_IC, BMS_IC,
			ADC_CONVERSION_MODE, ADCOPT);
	return (error_cell + error_aux + error_stat);
}

/*
 * @brief Function Name : adc_overlap_data_read
 * @brief Description	: return the number of errors in overlap test
 * 							if errors == 0 then pass
 */
int8_t adc_overlap_data_read() {
	int8_t error = 0;
	wakeup_sleep(TOTAL_IC);
	error = (int8_t) LTC6811_run_adc_overlap(TOTAL_IC, BMS_IC);
	return (error);
}

/*
 * @brief Function Name : digital_redundancy_check
 * @brief Description	: ADC redundancy check
 */
int8_t digital_redundancy_check() {
	wakeup_sleep(TOTAL_IC);
	error_aux_dr = LTC6811_run_adc_redundancy_st(ADC_CONVERSION_MODE, AUX,
	TOTAL_IC, BMS_IC);
	wakeup_sleep(TOTAL_IC);
	error_stat_dr = LTC6811_run_adc_redundancy_st(ADC_CONVERSION_MODE, STAT,
	TOTAL_IC, BMS_IC);
	return (error_aux_dr + error_stat_dr);
}

/*
 * @brief Function Name : crc_error_read
 * @brief Description	: return the number of errors in CRC check
 */
uint16_t crc_error_read() {
	uint16_t error_count = 0;
	uint8_t current_ic = 0;
	for (current_ic = 0; current_ic < TOTAL_IC; current_ic++) {
		error_count += BMS_IC[current_ic].crc_count.pec_count;
	}
	return (error_count);
}

/*
 * @brief Function Name : open_wire_check
 * @brief Description	: return the open wire connection in cells
 */
void open_wire_check() {
	wakeup_sleep(TOTAL_IC);
	LTC681x_run_openwire_multi(TOTAL_IC, BMS_IC);
}

/*
 * @brief Function Name : open_wire_gpio_check
 * @brief Description	: return the open wire connection in GPIO
 */
void open_wire_gpio_check() {
	wakeup_sleep(TOTAL_IC);
	LTC681x_run_gpio_openwire(TOTAL_IC, BMS_IC);
}

/*
 *  ____________________________________________________________________________
 * |REG  |RD/WR |BIT 7  |BIT 6  |BIT 5  |BIT 4  |BIT 3  |BIT 2  |BIT 1  |BIT 0  |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 * |CFGR0|RD/WR |GPIO5  |GPIO4  |GPIO3  |GPIO2  |GPIO1  |REFON  | DTEN  |ADCOPT |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 * |CFGR1|RD/WR |VUV[7] |VUV[6] |VUV[5] |VUV[4] |VUV[3] |VUV[2] |VUV[1] |VUV[0] |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 * |CFGR2|RD/WR |VOV[3] |VOV[2] |VOV[1] |VOV[0] |VUV[11]|VUV[10]|VUV[9] |VUV[8] |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 * |CFGR3|RD/WR |VOV[11]|VOV[10]|VOV[9] |VOV[8] |VOV[7] |VOV[6] |VOV[5] |VOV[4] |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 * |CFGR4|RD/WR |DCC8   |DCC7   |DCC6   |DCC5   |DCC4   |DCC3   |DCC2   |DCC1   |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 * |CFGR5|RD/WR |DCTO[3]|DCTO[2]|DCTO[1]|DCTO[0]|DCC12  |DCC11  |DCC10  |DCC9   |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 */

int8_t config_data_read() {
	int8_t error = 0;
	wakeup_sleep(TOTAL_IC);
	error = LTC6811_rdcfg(TOTAL_IC, BMS_IC);
	return (error);
}

/*
 * BMS_IC[current_ic].pwm.rx_data[i] >> i = 0 to 5
 * 4 bits represent 1 cell duty cycle
 * F is 100%
 */
void pwm_reg_read() {
	wakeup_idle(TOTAL_IC);
	LTC6811_rdpwm(TOTAL_IC, 0, BMS_IC);
}

/*
 * S CTRL register - used for serial pulsing through s pins
 * 4 BITs - 0000 for de-asserted
 *          1xxx for asserted
 *          others - pulses according to the number represent
 * BMS_IC[current_ic].sctrl.rx_data[i] >> i 0 to 5
 */
int8_t s_control_reg_read() {
	uint8_t streg = 0;
	int8_t error = 0;
	wakeup_idle(TOTAL_IC);
	error = LTC6811_rdsctrl(TOTAL_IC, streg, BMS_IC);

	return (error);
}

/*
 * Cell balancing
 */
#define  CELL_BALANCING_EN
#define  cell_bal_algo_2
#ifdef CELL_BALANCING_EN
void cell_balance_LTC() {
	int discharge_cells = 0;
	uint8_t local_ic_dis = 0;
	uint8_t local_cell_dis = 0;
	if (cell_balancing_enable) {
#ifdef cell_bal_algo_1
        for (local_ic_dis = 0; local_ic_dis < TOTAL_IC; local_ic_dis++)
        {
            discharge_cells = 0;
            if (highest_cell_voltage - lowest_cell_voltage
                    >= setup_data.balancing_max_vgap)
            {
                for (local_cell_dis = 0;
                        local_cell_dis < BMS_IC[local_ic_dis].ic_reg.cell_channels;
                        local_cell_dis++)
                {

                    if ((highest_cell_voltage
                            - BMS_IC[local_ic_dis].cells.c_codes[local_cell_dis])
                            < /*setup_data.balancing_max_vgap*/dCELL_BALANCING_THRESHOLD)
                    {
                        discharge_cells = discharge_cells
                                | (1 << local_cell_dis);
                    }
                }
            }
            LTC6811_set_discharge(discharge_cells, local_ic_dis, BMS_IC);
        }
        wakeup_sleep(TOTAL_IC);
        LTC6811_wrcfg(TOTAL_IC, BMS_IC);
#endif
#ifdef cell_bal_algo_2
		for (local_ic_dis = 0; local_ic_dis < TOTAL_IC; local_ic_dis++) {
			for (local_cell_dis = 0;
					local_cell_dis < BMS_IC[0].ic_reg.cell_channels;
					local_cell_dis++) {
				if (BMS_IC[local_ic_dis].cells.c_codes[local_cell_dis]
						- lowest_cell_voltage > setup_data.balancing_max_vgap) {
					discharge_cells = discharge_cells
							| (0x01 << local_cell_dis);
				}
			}
			LTC6811_set_discharge(discharge_cells, local_ic_dis, BMS_IC);
		}
		wakeup_sleep(TOTAL_IC);
		LTC6811_wrcfg(TOTAL_IC, BMS_IC);
#endif
	} else {
		for (local_ic_dis = 0; local_ic_dis < TOTAL_IC; local_ic_dis++) {
			LTC6811_set_discharge(0, local_ic_dis, BMS_IC);
		}
		wakeup_sleep(TOTAL_IC);
		LTC6811_wrcfg(TOTAL_IC, BMS_IC);
	}
}
#endif

/*
 * Function name : analyze_cell_data
 * Description : # sort highest and lowest variables
 *               # sort according cell positions and temperature
 *                 sensor positions
 *      Temperature id start with 1
 *      Cell id start with 1
 *      temperature initialize value - (-25C)
 */
void analyze_cell_data()
{
    uint8_t current_ic_1 = 0;
    uint8_t current_cell_1 = 0;
    uint8_t current_temp_1 = 0;
    highest_cell_voltage = 0;
    lowest_cell_voltage = 0xFFFF;
    lowest_cell_id = 0;
    highest_cell_id = 0;
    total_pack_voltage = 0;

    highest_t = TemperatureDataTable[123];
    lowest_t = TemperatureDataTable[0];
    highest_t_id = 0;
    lowest_t_id = 0;

    highest_board_t = 0;
    highest_board_t_id = 0xFF;
    lowest_board_t = 0xFF;
    lowest_board_t_id = 0xFF;
//highest and lowest cell voltages
    for (current_ic_1 = 0; current_ic_1 < TOTAL_IC; current_ic_1++)
    {
        BMS_IC[current_ic_1].cells.max_cell_v_local = 0;
        BMS_IC[current_ic_1].cells.min_cell_v_local = 0xFFFF;
        BMS_IC[current_ic_1].cells.max_cell_id_local = 0;
        BMS_IC[current_ic_1].cells.min_cell_id_local = 0;

        BMS_IC[current_ic_1].heat.highest_t_local = TemperatureDataTable[123];
        BMS_IC[current_ic_1].heat.lowest_t_local = TemperatureDataTable[0];
        BMS_IC[current_ic_1].heat.highest_t_id_local = 0;
        BMS_IC[current_ic_1].heat.lowest_t_id_local = 0;

        for (current_cell_1 = 0; current_cell_1 < CELLS_PER_IC;
                current_cell_1++)
        {

            if (BMS_IC[current_ic_1].cells.max_cell_v_local
                    < BMS_IC[current_ic_1].cells.c_codes[current_cell_1])
            {
                BMS_IC[current_ic_1].cells.max_cell_v_local =
                        BMS_IC[current_ic_1].cells.c_codes[current_cell_1];
                BMS_IC[current_ic_1].cells.max_cell_id_local = current_cell_1;
            }
            //sorting for lowest voltage cell
            if (BMS_IC[current_ic_1].cells.min_cell_v_local
                    > BMS_IC[current_ic_1].cells.c_codes[current_cell_1])
            {
                BMS_IC[current_ic_1].cells.min_cell_v_local =
                        BMS_IC[current_ic_1].cells.c_codes[current_cell_1];
                BMS_IC[current_ic_1].cells.min_cell_id_local = current_cell_1;
            }
        }
        //temperature
        for (current_temp_1 = 0; current_temp_1 < TEMP_PER_IC; current_temp_1++)
        {
            if (BMS_IC[current_ic_1].heat.highest_t_local
                    < BMS_IC[current_ic_1].heat.temp[current_temp_1])
            {
                BMS_IC[current_ic_1].heat.highest_t_local =
                        BMS_IC[current_ic_1].heat.temp[current_temp_1];
                BMS_IC[current_ic_1].heat.highest_t_id_local = current_temp_1;
            }
            if (BMS_IC[current_ic_1].heat.lowest_t_local
                    > BMS_IC[current_ic_1].heat.temp[current_temp_1])
            {
                BMS_IC[current_ic_1].heat.lowest_t_local =
                        BMS_IC[current_ic_1].heat.temp[current_temp_1];
                BMS_IC[current_ic_1].heat.lowest_t_id_local = current_temp_1;
            }
        }
    }
    highest_t = BMS_IC[0].heat.highest_t_local;
    highest_t_id = BMS_IC[0].heat.highest_t_id_local + 1;
    lowest_t = BMS_IC[0].heat.lowest_t_local;
    lowest_t_id = BMS_IC[0].heat.lowest_t_id_local + 1;
	total_pack_voltage = BMS_IC[0].stat.stat_codes[0];
}

/*
 * Function name 	: make_data_packet(uint8_t data_buffer_local[])
 * Description 		: Make USART data packet
 *
 *
 */
/*
 *  ___________________________________________________
 * |Item |Content                            |hex bytes|
 * |_____|___________________________________|_________|
 * |0    |Start           0x55               |1        |
 * |_____|___________________________________|_________|
 * |1    |Function code   0x46               |1        |
 * |_____|___________________________________|_________|
 * |2    |Length          0x46               |1        |
 * |_____|___________________________________|_________|
 * |3    |Cell quantity                      |1        |
 * |_____|___________________________________|_________|
 * |4    |Voltage value of the 1st cell (mV) |2        |
 * |     |___________________________________|_________|
 * |     |Voltage value of the 2nd cell (mV) |2        |
 * |     |___________________________________|_________|
 * |     |...                                |2        |
 * |     |___________________________________|_________|
 * |     |Voltage value of the Mth cell (mV) |2        |
 * |_____|___________________________________|_________|
 * |5    |Temperature value quantity N=6     |1        |
 * |_____|___________________________________|_________|
 * |6    |Temperature of the 1st cell(0.1C)  |2        |
 * |     |___________________________________|_________|
 * |     |Temperature of the 2nd cell (0.1C) |2        |
 * |     |___________________________________|_________|
 * |     |Temperature of the 3rd cell (0.1C) |2        |
 * |     |___________________________________|_________|
 * |     |Temperature of the 4th cell (0.1C) |2        |
 * |     |___________________________________|_________|
 * |     |Ambient temperature (0.1C)         |2        |
 * |     |___________________________________|_________|
 * |     |BMS temperature (0.1C)             |2        |
 * |_____|___________________________________|_________|
 * |7    |Charge and discharge current(0.01A)|2        |
 * |_____|___________________________________|_________|
 * |8    |Total voltage (0.01V)              |2        |
 * |_____|___________________________________|_________|
 * |9    |Remaining capacity (0.01Ah)        |2        |
 * |_____|___________________________________|_________|
 * |10   |Customize P=10                     |1        |
 * |_____|___________________________________|_________|
 * |11   |Battery capacity (0.01Ah)          |2        |
 * |_____|___________________________________|_________|
 * |12   |Cycle life                         |2        |
 * |_____|___________________________________|_________|
 * |13   |Voltage of the port(0.01V)         |2        |
 * |_____|___________________________________|_________|
 * |14   |Discharge                          |0        |
 * |     |___________________________________|_________|
 * |     |Charge                             |1        |
 * |     |___________________________________|_________|
 * |     |Float charge                       |2        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |3        |
 * |     |___________________________________|_________|
 * |     |Standby                            |4        |
 * |     |___________________________________|_________|
 * |     |Power off                          |5        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |6        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |7        |
 * |_____|___________________________________|_________|
 * |15   |Discharge on/off status            |0        |
 * |     |___________________________________|_________|
 * |     |Charge on/off status               |1        |
 * |     |___________________________________|_________|
 * |     |Current limiting on/off status     |2        |
 * |     |___________________________________|_________|
 * |     |Heating on/off status              |3        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |4        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |5        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |6        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |7        |
 * |_____|___________________________________|_________|
 * |16   |Warning status 1                   |0        |
 * |     |___________________________________|_________|
 * |     |Warning status 2                   |1        |
 * |     |___________________________________|_________|
 * |     |Warning status 3                   |2        |
 * |     |___________________________________|_________|
 * |     |Warning status 4                   |3        |
 * |     |___________________________________|_________|
 * |     |Warning status 5                   |4        |
 * |     |___________________________________|_________|
 * |     |Warning status 6                   |5        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |6        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |7        |
 * |_____|___________________________________|_________|
 * |17   |Voltage sensor failure             |0        |
 * |     |___________________________________|_________|
 * |     |Temperature sensor failure         |1        |
 * |     |___________________________________|_________|
 * |     |Current sensor failure             |2        |
 * |     |___________________________________|_________|
 * |     |Button failure                     |3        |
 * |     |___________________________________|_________|
 * |     |Voltage difference failure         |4        |
 * |     |___________________________________|_________|
 * |     |Charge switch failure              |5        |
 * |     |___________________________________|_________|
 * |     |Discharge switch failure           |6        |
 * |     |___________________________________|_________|
 * |     |Current limiting failure           |7        |
 * |_____|___________________________________|_________|
 * |18   |Over V warning of individual cell  |0 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over V trip of individual cell     |1 1-bit  |
 * |     |___________________________________|_________|
 * |     |Low V warning of individual cell   |2 1-bit  |
 * |     |___________________________________|_________|
 * |     |Low V trip of individual cell      |3 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over V warning of pack voltage     |4 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over V trip of pack voltage        |5 1-bit  |
 * |     |___________________________________|_________|
 * |     |Low V warning of pack voltage      |6 1-bit  |
 * |     |___________________________________|_________|
 * |     |Low V trip of pack voltage         |7 1-bit  |
 * |_____|___________________________________|_________|
 * |19   |Charge high temperature warning    |0 1-bit  |
 * |     |___________________________________|_________|
 * |     |Charge high temperature trip       |1 1-bit  |
 * |     |___________________________________|_________|
 * |     |Charge low temperature warning     |2 1-bit  |
 * |     |___________________________________|_________|
 * |     |Charge low temperature trip        |3 1-bit  |
 * |     |___________________________________|_________|
 * |     |Discharge high temperature warning |4 1-bit  |
 * |     |___________________________________|_________|
 * |     |Discharge high temperature trip    |5 1-bit  |
 * |     |___________________________________|_________|
 * |     |Discharge low temperature warning  |6 1-bit  |
 * |     |___________________________________|_________|
 * |     |Discharge low temperature trip     |7 1-bit  |
 * |_____|___________________________________|_________|
 * |20   |Ambient high temperature warning   |0 1-bit  |
 * |     |___________________________________|_________|
 * |     |Ambient high temperature trip      |1 1-bit  |
 * |     |___________________________________|_________|
 * |     |Ambient low temperature warning    |2 1-bit  |
 * |     |___________________________________|_________|
 * |     |Ambient low temperature trip       |3 1-bit  |
 * |     |___________________________________|_________|
 * |     |High temperature warning (PCB)     |4 1-bit  |
 * |     |___________________________________|_________|
 * |     |High temperature trip (PCB)        |5 1-bit  |
 * |     |___________________________________|_________|
 * |     |Heating                            |6 1-bit  |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |7 1-bit  |
 * |_____|___________________________________|_________|
 * |21   |Over current warning (Charge)      |0 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over current trip (Charge)         |1 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over current warning (Discharge)   |2 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over current trip (Discharge)      |3 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over current trip (Transient)      |4 1-bit  |
 * |     |___________________________________|_________|
 * |     |Short circuit trip (Output current)|5 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over current lock (Transient)      |6 1-bit  |
 * |     |___________________________________|_________|
 * |     |Short circuit lock (Output current)|7 1-bit  |
 * |_____|___________________________________|_________|
 * |22   |High voltage trip (Charge)         |0 1-bit  |
 * |     |___________________________________|_________|
 * |     |Intermittent power supply waiting  |1 1-bit  |
 * |     |___________________________________|_________|
 * |     |Remaining capacity warning         |2 1-bit  |
 * |     |___________________________________|_________|
 * |     |Remaining capacity trip            |3 1-bit  |
 * |     |___________________________________|_________|
 * |     |Stop charging warning of low cell  |4 1-bit  |
 * |     |__voltage__________________________|_________|
 * |     |Reverse polarity protection(Output)|5 1-bit  |
 * |     |___________________________________|_________|
 * |     |Output connection failure          |6 1-bit  |
 * |     |___________________________________|_________|
 * |     |Internal bit                       |7 1-bit  |
 * |_____|___________________________________|_________|
 * |23   |Reserved                           |1        |
 * |_____|___________________________________|_________|
 * |24   |Reserved                           |1        |
 * |_____|___________________________________|_________|
 * |25   |CRC                                |2        |
 * |_____|___________________________________|_________|
 * |26   |END                                |1        |
 * |_____|___________________________________|_________|
 *
 */

//void make_data_packet(uint8_t data_buffer_local[]){
//	data_buffer_local[0] = 0x55;
//	data_buffer_local[1] = 0x46;
//	data_buffer_local[2] = 0x46;
//	//Cells voltage measurement
//	data_buffer_local[3] = CELLS_PER_BMS;
//	data_buffer_local[4] = (BMS_IC[0].cells.c_codes[0]/10)>>8;
//	data_buffer_local[5] = (BMS_IC[0].cells.c_codes[0]/10) & 0xFF;
//	data_buffer_local[6] = (BMS_IC[0].cells.c_codes[1]/10)>>8;
//	data_buffer_local[7] = (BMS_IC[0].cells.c_codes[1]/10) & 0xFF;
//	data_buffer_local[8] = (BMS_IC[0].cells.c_codes[2]/10)>>8;
//	data_buffer_local[9] = (BMS_IC[0].cells.c_codes[2]/10) & 0xFF;
//	data_buffer_local[10] = (BMS_IC[0].cells.c_codes[3]/10)>>8;
//	data_buffer_local[11] = (BMS_IC[0].cells.c_codes[3]/10) & 0xFF;
//	data_buffer_local[12] = (BMS_IC[0].cells.c_codes[4]/10)>>8;
//	data_buffer_local[13] = (BMS_IC[0].cells.c_codes[4]/10) & 0xFF;
//	data_buffer_local[14] = (BMS_IC[0].cells.c_codes[5]/10)>>8;
//	data_buffer_local[15] = (BMS_IC[0].cells.c_codes[5]/10) & 0xFF;
//	data_buffer_local[16] = (BMS_IC[0].cells.c_codes[6]/10)>>8;
//	data_buffer_local[17] = (BMS_IC[0].cells.c_codes[6]/10) & 0xFF;
//	data_buffer_local[18] = (BMS_IC[0].cells.c_codes[7]/10)>>8;
//	data_buffer_local[19] = (BMS_IC[0].cells.c_codes[7]/10) & 0xFF;
//	data_buffer_local[20] = (BMS_IC[0].cells.c_codes[8]/10)>>8;
//	data_buffer_local[21] = (BMS_IC[0].cells.c_codes[8]/10) & 0xFF;
//	data_buffer_local[22] = (BMS_IC[0].cells.c_codes[9]/10)>>8;
//	data_buffer_local[23] = (BMS_IC[0].cells.c_codes[9]/10) & 0xFF;
//	data_buffer_local[24] = (BMS_IC[0].cells.c_codes[10]/10)>>8;
//	data_buffer_local[25] = (BMS_IC[0].cells.c_codes[10]/10) & 0xFF;
//	data_buffer_local[26] = (BMS_IC[0].cells.c_codes[12]/10)>>8;
//	data_buffer_local[27] = (BMS_IC[0].cells.c_codes[12]/10) & 0xFF;
//	data_buffer_local[28] = (BMS_IC[0].cells.c_codes[13]/10)>>8;
//	data_buffer_local[29] = (BMS_IC[0].cells.c_codes[13]/10) & 0xFF;
//	data_buffer_local[30] = (BMS_IC[0].cells.c_codes[14]/10)>>8;
//	data_buffer_local[31] = (BMS_IC[0].cells.c_codes[14]/10) & 0xFF;
//	data_buffer_local[32] = (BMS_IC[0].cells.c_codes[15]/10)>>8;
//	data_buffer_local[33] = (BMS_IC[0].cells.c_codes[15]/10) & 0xFF;
//	data_buffer_local[34] = (BMS_IC[0].cells.c_codes[16]/10)>>8;
//	data_buffer_local[35] = (BMS_IC[0].cells.c_codes[16]/10) & 0xFF;
//	//Temperatures measurement
//	/*
//	 * Unsigned integer (0.1K), actual value = (transmission value-2731)/10 (℃).
//	 * For example: 3032 means (3032-2731)/10 (℃)=30.1℃
//	 */
//	data_buffer_local[36] = TEMPS_PER_BMS;
//	data_buffer_local[37] = BMS_IC[0].heat.temp[0]>>8;
//	data_buffer_local[38] = BMS_IC[0].heat.temp[0]& 0xFF;
//	data_buffer_local[39] = BMS_IC[0].heat.temp[1]>>8;
//	data_buffer_local[40] = BMS_IC[0].heat.temp[1]& 0xFF;
//	data_buffer_local[41] = BMS_IC[0].heat.temp[2]>>8;
//	data_buffer_local[42] = BMS_IC[0].heat.temp[2]& 0xFF;
//	data_buffer_local[43] = BMS_IC[0].heat.temp[3]>>8;
//	data_buffer_local[44] = BMS_IC[0].heat.temp[3]& 0xFF;
//	data_buffer_local[45] = BMS_IC[0].heat.temp[4]>>8;
//	data_buffer_local[46] = BMS_IC[0].heat.temp[4]& 0xFF;
//	data_buffer_local[41] = BMS_IC[0].heat.temp[5]>>8;
//	data_buffer_local[42] = BMS_IC[0].heat.temp[5]& 0xFF;
//	//Current
////	data_buffer_local[42] = BMS_IC;
//}
//



//--------------------------------------------------------------//

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */



  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_SPI3_Init();
  MX_SPI4_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  RTC_Init();

// int INA229
  for (int i = 0; i < NUM_INA229; i++)
  { INA229_config(ina229_devices[i]); HAL_Delay(10); }



  mcu_spiInit(0);
  HAL_Delay(10);
  HAL_Delay(10);


  HAL_Delay(10);

  //init ltc ics

  //--------------------------------------------------------------//

	LTC6811_init_cfg(TOTAL_IC, BMS_IC);
	uint8_t main_current_ic;
	for (main_current_ic = 0; main_current_ic < TOTAL_IC; main_current_ic++) {
		LTC6811_set_cfgr(main_current_ic, BMS_IC, REF_ON, ADCOPT, GPIOBITS_A,
				DCCBITS_A, DCTOBITS, UV, OV);
	}
	LTC6811_wrcfg(TOTAL_IC, BMS_IC);
	LTC6811_reset_crc_count(TOTAL_IC, BMS_IC);
	LTC6811_init_reg_limits(TOTAL_IC, BMS_IC);

  //--------------------------------------------------------------//

  /* Initialize the display module */
  Display_Init();

  /* Display the main title page */
  Display_MainTitlePage();

  /* Initialize the expander at address 0x20 by configuring all its pins as outputs */
  Expander_InitAllDevices(&hi2c2);
  Expander_InitAllDevices(&hi2c3);

//fixing the startup resistance of temperature cards
#ifdef start_Resistance_fix
		  cell12_Temp_01_startup(10);
		  cell12_Temp_02_startup(10);
		  cell12_Temp_03_startup(10);
		  cell11_Temp_01_startup(10);
		  cell11_Temp_02_startup(10);
		  cell11_Temp_03_startup(10);

#endif


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  RTC_ReadTime();
//	  char timeStr[16];
//	  char dateStr[16];
//	  sprintf(timeStr, "%02d:%02d:%02d", time.hour, time.minute, time.second);
//	  sprintf(dateStr, "%02d/%02d/%02d", time.day, time.month, time.year);
//	  display_lcd(timeStr);
//	  HAL_Delay(100);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	  HAL_GPIO_WritePin(GPIOC, SPI3_CS_03_Pin|SPI3_CS_02_Pin, GPIO_PIN_RESET);

//	   Scan_I2C_Bus();
//	   Display_MainTitlePage();


//	  Set_Output_Voltage(&hi2c2, CELL_1, 4.0f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c2, CELL_1, 2.5f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c2, CELL_1, 2.8f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c2, CELL_1, 3.3f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c2, CELL_1, 3.4f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c2, CELL_1, 3.6f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c2, CELL_1, 4.0f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c2, CELL_1, 4.2f);
//
//
//
//	  Set_Output_Voltage(&hi2c3, CELL_1, 2.0f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c3, CELL_1, 2.5f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c3, CELL_1, 2.8f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c3, CELL_1, 3.3f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c3, CELL_1, 3.4f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c3, CELL_1, 3.6f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c3, CELL_1, 4.0f);
//	  HAL_Delay(1000);
//	  Set_Output_Voltage(&hi2c3, CELL_1, 4.2f);

//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_02_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_03_LED_01 , HIGH);
//
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_01_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_02_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_03_LED_01 , HIGH);
//
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_01_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_02_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_03_LED_01 , HIGH);
//
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_01_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_02_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_03_LED_01 , HIGH);
//
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_02_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_03_LED_01 , HIGH);
//
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_01_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_02_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_03_LED_01 , HIGH);
//
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_01_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_02_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_03_LED_01 , HIGH);
//
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_04, CELL_01_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_04, CELL_02_LED_01 , HIGH);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_04, CELL_03_LED_01 , HIGH);



//			  HAL_Delay(1000);
//
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_02_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_03_LED_01 , LOW);
//
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_01_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_02_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_03_LED_01 , LOW);
//
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_01_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_02_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_03_LED_01 , LOW);
//
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_01_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_02_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_03_LED_01 , LOW);
//
//
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_02_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_03_LED_01 , LOW);
//
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_01_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_02_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_03_LED_01 , LOW);
//
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_01_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_02_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_03_LED_01 , LOW);
//
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_04, CELL_01_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_04, CELL_02_LED_01 , LOW);
//			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_04, CELL_03_LED_01 , LOW);



//	    HAL_Delay(100);
//
//	    // Run the LED sequence
//	     RunLEDSequence();
//
//	     HAL_Delay(100); // Delay between sequences

		  ////////////////////////////////////////////////////

		  cell12_Temp_01_Set(resistance[0]);
		  cell12_Temp_02_Set(resistance[1]);
		  cell12_Temp_03_Set(resistance[2]);
		  cell11_Temp_01_Set(resistance[3]);
		  cell11_Temp_02_Set(resistance[4]);
		  cell11_Temp_03_Set(resistance[4]);

	  ////////////////////////////////////////////////////////////
		  HAL_Delay(2000);
	  Set_Output_Voltage(&hi2c2, CELL_1, 2.0f);
Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , HIGH);

HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
  busVoltage_01 = INA229_getVBUS_V(INA229_0);

//
//HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);



  HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
  temperatureC_01 = INA229_getDIETEMP_C(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);
HAL_Delay(10);

Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);

HAL_Delay(2000);
Set_Output_Voltage(&hi2c2, CELL_1, 2.5f);
Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , HIGH);

HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
busVoltage_01 = INA229_getVBUS_V(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
temperatureC_01 = INA229_getDIETEMP_C(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);
HAL_Delay(10);

Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);
HAL_Delay(2000);
Set_Output_Voltage(&hi2c2, CELL_1, 2.8f);
Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , HIGH);

HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
busVoltage_01 = INA229_getVBUS_V(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
temperatureC_01 = INA229_getDIETEMP_C(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);
HAL_Delay(10);

Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);

HAL_Delay(2000);
Set_Output_Voltage(&hi2c2, CELL_1, 3.3f);
Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , HIGH);

HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
busVoltage_01 = INA229_getVBUS_V(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
temperatureC_01 = INA229_getDIETEMP_C(INA229_0);

//

HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);
HAL_Delay(10);

Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);
HAL_Delay(2000);
Set_Output_Voltage(&hi2c2, CELL_1, 3.4f);
Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , HIGH);

HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
busVoltage_01 = INA229_getVBUS_V(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
temperatureC_01 = INA229_getDIETEMP_C(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);
HAL_Delay(10);

Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);

HAL_Delay(2000);
Set_Output_Voltage(&hi2c2, CELL_1, 3.6f);
Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , HIGH);

HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
busVoltage_01 = INA229_getVBUS_V(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
temperatureC_01 = INA229_getDIETEMP_C(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);
HAL_Delay(10);

Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);
HAL_Delay(2000);
Set_Output_Voltage(&hi2c2, CELL_1, 4.0f);
Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , HIGH);

HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
busVoltage_01 = INA229_getVBUS_V(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
temperatureC_01 = INA229_getDIETEMP_C(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);
HAL_Delay(10);

Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);

HAL_Delay(2000);
Set_Output_Voltage(&hi2c2, CELL_1, 4.2f);
Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , HIGH);

HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
busVoltage_01 = INA229_getVBUS_V(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_RESET);
//
temperatureC_01 = INA229_getDIETEMP_C(INA229_0);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin, GPIO_PIN_SET);
HAL_Delay(10);

Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);

HAL_Delay(2000);
///////////////////////////////////////////////////////////////////

Set_Output_Voltage(&hi2c2, CELL_2, 3.3f);

Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_02_LED_01 , HIGH);

			  HAL_GPIO_WritePin(GPIOE, CELL12_CS_02_Pin, GPIO_PIN_RESET);


//
		  busVoltage_02 = INA229_getVBUS_V(INA229_1);

//
//HAL_Delay(1000);
		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_02_Pin, GPIO_PIN_SET);



		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_02_Pin, GPIO_PIN_RESET);
//

		  temperatureC_02 = INA229_getDIETEMP_C(INA229_1);

//
//HAL_Delay(1000);
	  HAL_GPIO_WritePin(GPIOE, CELL12_CS_02_Pin, GPIO_PIN_SET);
	  HAL_Delay(10);

	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_02_LED_01 , LOW);

	  ///////////////////////////////////////////////////////////////////
	 ///////////////////////////////////////////////////////////////////

			  Set_Output_Voltage(&hi2c2, CELL_3, 4.0f);

			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_03_LED_01 , HIGH);

			  			  HAL_GPIO_WritePin(GPIOE, CELL12_CS_03_Pin, GPIO_PIN_RESET);

			  		  busVoltage_03 = INA229_getVBUS_V(INA229_2);

			  //
			  //HAL_Delay(10);
			  		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_03_Pin, GPIO_PIN_SET);



			  		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_03_Pin, GPIO_PIN_RESET);
			  //
			  		  temperatureC_03 = INA229_getDIETEMP_C(INA229_2);

			  //
			  //HAL_Delay(10);
			  	  HAL_GPIO_WritePin(GPIOE, CELL12_CS_03_Pin, GPIO_PIN_SET);
			  	  HAL_Delay(10);
			  	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_03_LED_01 , LOW);
			  	  ///////////////////////////////////////////////////////////////////

				  Set_Output_Voltage(&hi2c2, CELL_4, 3.6f);

				  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_01_LED_01 , HIGH);

				  			  HAL_GPIO_WritePin(GPIOE, CELL12_CS_04_Pin, GPIO_PIN_RESET);

				  		  busVoltage_04 = INA229_getVBUS_V(INA229_3);

				  //
				  //HAL_Delay(10);
				  		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_04_Pin, GPIO_PIN_SET);



				  		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_04_Pin, GPIO_PIN_RESET);
				  //
				  		  temperatureC_04 = INA229_getDIETEMP_C(INA229_3);

				  //
				  //HAL_Delay(10);
				  	  HAL_GPIO_WritePin(GPIOE, CELL12_CS_04_Pin, GPIO_PIN_SET);
				  	HAL_Delay(10);
				  	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_01_LED_01 , LOW);
				  	  ///////////////////////////////////////////////////////////////////

					  Set_Output_Voltage(&hi2c2, CELL_5, 2.5f);

					  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_02_LED_01 , HIGH);
					  HAL_Delay(10);
					  			  HAL_GPIO_WritePin(GPIOE, CELL12_CS_05_Pin, GPIO_PIN_RESET);
					  HAL_Delay(10);
					  		  busVoltage_05 = INA229_getVBUS_V(INA229_4);
					  		HAL_Delay(10);
					  //
					  //HAL_Delay(10);
					  		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_05_Pin, GPIO_PIN_SET);
					  		  HAL_Delay(10);


					  		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_05_Pin, GPIO_PIN_RESET);
					  //
					  		  temperatureC_05 = INA229_getDIETEMP_C(INA229_4);

					  //
					  //HAL_Delay(10);
					  	  HAL_GPIO_WritePin(GPIOE, CELL12_CS_05_Pin, GPIO_PIN_SET);
					  	HAL_Delay(10);
					  	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_02_LED_01 , LOW);
					  	  ///////////////////////////////////////////////////////////////////

						  Set_Output_Voltage(&hi2c2, CELL_6, 3.6f);

						  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_03_LED_01 , HIGH);
						  HAL_Delay(10);
						  			  HAL_GPIO_WritePin(GPIOI, CELL12_CS_06_Pin, GPIO_PIN_RESET);
						  HAL_Delay(10);
						  		  busVoltage_06 = INA229_getVBUS_V(INA229_5);
						  		HAL_Delay(10);
						  //
						  //HAL_Delay(10);
						  		  HAL_GPIO_WritePin(GPIOI, CELL12_CS_06_Pin, GPIO_PIN_SET);
						  		  HAL_Delay(10);


						  		  HAL_GPIO_WritePin(GPIOI, CELL12_CS_06_Pin, GPIO_PIN_RESET);
						  //
						  		  temperatureC_06 = INA229_getDIETEMP_C(INA229_5);

						  //
						  //HAL_Delay(10);
						  	  HAL_GPIO_WritePin(GPIOI, CELL12_CS_06_Pin, GPIO_PIN_SET);
						  	HAL_Delay(10);
						  	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_03_LED_01 , LOW);
						  	  ///////////////////////////////////////////////////////////////////

							  Set_Output_Voltage(&hi2c2, CELL_7, 4.0f);

							  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_01_LED_01 , HIGH);
							  HAL_Delay(10);
							  HAL_GPIO_WritePin(CELL12_CS_07_GPIO_Port, CELL12_CS_07_Pin, GPIO_PIN_RESET);
							  HAL_Delay(10);
							  		  busVoltage_07 = INA229_getVBUS_V(INA229_6);
							  		HAL_Delay(10);
							  //
							  //HAL_Delay(10);
							  		HAL_GPIO_WritePin(CELL12_CS_07_GPIO_Port, CELL12_CS_07_Pin, GPIO_PIN_SET);
							  		  HAL_Delay(10);


							  		HAL_GPIO_WritePin(CELL12_CS_07_GPIO_Port, CELL12_CS_07_Pin, GPIO_PIN_RESET);
							  //
							  		  temperatureC_07 = INA229_getDIETEMP_C(INA229_6);

							  //
							  //HAL_Delay(10);
							  		HAL_GPIO_WritePin(CELL12_CS_07_GPIO_Port, CELL12_CS_07_Pin, GPIO_PIN_SET);
							  	  HAL_Delay(10);
							  	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_01_LED_01 , LOW);

								  ////////////////////////////////////////////////////////////

								  Set_Output_Voltage(&hi2c2, CELL_8, 2.5f);
					Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_02_LED_01 , HIGH);
					HAL_Delay(10);
								  HAL_GPIO_WritePin(GPIOI, CELL12_CS_08_Pin, GPIO_PIN_RESET);
					//
							  busVoltage_08 = INA229_getVBUS_V(INA229_7);

					//
					//HAL_Delay(10);
							  HAL_GPIO_WritePin(GPIOI, CELL12_CS_08_Pin, GPIO_PIN_SET);
							  HAL_Delay(10);


							  HAL_GPIO_WritePin(GPIOI, CELL12_CS_08_Pin, GPIO_PIN_RESET);
					//
							  temperatureC_08 = INA229_getDIETEMP_C(INA229_7);

					//
					//HAL_Delay(10);
						  HAL_GPIO_WritePin(GPIOI, CELL12_CS_08_Pin, GPIO_PIN_SET);
						  HAL_Delay(10);

						  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_02_LED_01 , LOW);

						  ///////////////////////////////////////////////////////////////////

						  Set_Output_Voltage(&hi2c2, CELL_9,3.6f);

						  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_03_LED_01 , HIGH);
						  HAL_Delay(10);
						  			  HAL_GPIO_WritePin(GPIOI, CELL12_CS_09_Pin, GPIO_PIN_RESET);

						  			HAL_Delay(10);
						  //
						  		  busVoltage_09 = INA229_getVBUS_V(INA229_8);
						  		HAL_Delay(10);
						  //
						  //HAL_Delay(10);
						  		  HAL_GPIO_WritePin(GPIOI, CELL12_CS_09_Pin, GPIO_PIN_SET);
						  		HAL_Delay(10);


						  		  HAL_GPIO_WritePin(GPIOI, CELL12_CS_09_Pin, GPIO_PIN_RESET);
						  //
						  		HAL_Delay(10);
						  		  temperatureC_09 = INA229_getDIETEMP_C(INA229_8);
						  		HAL_Delay(10);
						  //
						  //HAL_Delay(10);
						  	  HAL_GPIO_WritePin(GPIOI, CELL12_CS_09_Pin, GPIO_PIN_SET);
						  	  HAL_Delay(10);

						  	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_03_LED_01 , LOW);

						  	  ///////////////////////////////////////////////////////////////////

							  ///////////////////////////////////////////////////////////////////

							  Set_Output_Voltage(&hi2c2, CELL_10, 4.0f);

							  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_04, CELL_01_LED_01 , HIGH);

							  			  HAL_GPIO_WritePin(GPIOI, CELL12_CS_10_Pin, GPIO_PIN_RESET);
							  HAL_Delay(10);
							  		  busVoltage_10 = INA229_getVBUS_V(INA229_9);
							  		HAL_Delay(10);
							  //
							  //HAL_Delay(10);
							  		  HAL_GPIO_WritePin(GPIOI, CELL12_CS_10_Pin, GPIO_PIN_SET);
							  		HAL_Delay(10);


							  		  HAL_GPIO_WritePin(GPIOI, CELL12_CS_10_Pin, GPIO_PIN_RESET);
							  //
							  		  temperatureC_10 = INA229_getDIETEMP_C(INA229_9);

							  //
							  //HAL_Delay(10);
							  	  HAL_GPIO_WritePin(GPIOI, CELL12_CS_10_Pin, GPIO_PIN_SET);
							  	HAL_Delay(10);
							  	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_04, CELL_01_LED_01 , LOW);
							  	  ///////////////////////////////////////////////////////////////////

								  Set_Output_Voltage(&hi2c2, CELL_11, 2.5f);

								  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_04, CELL_02_LED_01 , HIGH);
								  HAL_Delay(10);
								  			  HAL_GPIO_WritePin(GPIOF, CELL12_CS_11_Pin, GPIO_PIN_RESET);
								  HAL_Delay(10);
								  		  busVoltage_11 = INA229_getVBUS_V(INA229_10);
								  		HAL_Delay(10);
								  //
								  //HAL_Delay(10);
								  		  HAL_GPIO_WritePin(GPIOF, CELL12_CS_11_Pin, GPIO_PIN_SET);
								  		HAL_Delay(10);


								  		  HAL_GPIO_WritePin(GPIOF, CELL12_CS_11_Pin, GPIO_PIN_RESET);
								  //
								  		  temperatureC_11 = INA229_getDIETEMP_C(INA229_10);

								  //
								  //HAL_Delay(10);
								  	  HAL_GPIO_WritePin(GPIOF, CELL12_CS_11_Pin, GPIO_PIN_SET);
								  	  HAL_Delay(10);
								  	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_04, CELL_02_LED_01 , LOW);
								  	  ///////////////////////////////////////////////////////////////////

									  Set_Output_Voltage(&hi2c2, CELL_12, 3.6f);

									  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_04, CELL_03_LED_01 , HIGH);
									  HAL_Delay(10);
									  			  HAL_GPIO_WritePin(GPIOF, CELL12_CS_12_Pin, GPIO_PIN_RESET);
									  HAL_Delay(10);
									  		  busVoltage_12 = INA229_getVBUS_V(INA229_11);
									  		HAL_Delay(10);
									  //
									  //HAL_Delay(10);
									  		  HAL_GPIO_WritePin(GPIOF, CELL12_CS_12_Pin, GPIO_PIN_SET);
									  		HAL_Delay(10);


									  		  HAL_GPIO_WritePin(GPIOF, CELL12_CS_12_Pin, GPIO_PIN_RESET);
									  //
									  		  temperatureC_12 = INA229_getDIETEMP_C(INA229_11);

									  //
									  //HAL_Delay(10);
									  	  HAL_GPIO_WritePin(GPIOF, CELL12_CS_12_Pin, GPIO_PIN_SET);
									  	  HAL_Delay(10);
									  	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_04, CELL_03_LED_01 , LOW);
									  	  ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
									  	/////////////////////////////////////////////////////////////

										  Set_Output_Voltage(&hi2c3, CELL_1, 4.0f);
									Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , HIGH);

										  HAL_GPIO_WritePin(GPIOB, CELL11_CS_01_Pin, GPIO_PIN_RESET);
									//
									  busVoltage_13 = INA229_getVBUS_V(INA229_13);

									//
									//HAL_Delay(10);
									  HAL_GPIO_WritePin(GPIOB, CELL11_CS_01_Pin, GPIO_PIN_SET);



									  HAL_GPIO_WritePin(GPIOB, CELL11_CS_01_Pin, GPIO_PIN_RESET);
									//
									  temperatureC_13 = INA229_getDIETEMP_C(INA229_13);

									//
									//HAL_Delay(10);
									HAL_GPIO_WritePin(GPIOB, CELL11_CS_01_Pin, GPIO_PIN_SET);
									HAL_Delay(10);

									Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);

									///////////////////////////////////////////////////////////////////
									  Set_Output_Voltage(&hi2c3, CELL_2, 4.0f);
								Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_02_LED_01 , HIGH);

									  HAL_GPIO_WritePin(GPIOB, CELL11_CS_02_Pin, GPIO_PIN_RESET);
								//
								  busVoltage_14 = INA229_getVBUS_V(INA229_14);

								//
								//HAL_Delay(10);
								  HAL_GPIO_WritePin(GPIOB, CELL11_CS_02_Pin, GPIO_PIN_SET);



								  HAL_GPIO_WritePin(GPIOB, CELL11_CS_02_Pin, GPIO_PIN_RESET);
								//
								  temperatureC_14 = INA229_getDIETEMP_C(INA229_14);

								//
								//HAL_Delay(10);
								HAL_GPIO_WritePin(GPIOB, CELL11_CS_02_Pin, GPIO_PIN_SET);
								HAL_Delay(10);

								Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_02_LED_01 , LOW);



									  	/////////////////////////////////////////////////////////////
								///////////////////////////////////////////////////////////////////
								  Set_Output_Voltage(&hi2c3, CELL_3, 4.0f);
							Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_03_LED_01 , HIGH);

								  HAL_GPIO_WritePin(GPIOF, CELL11_CS_03_Pin, GPIO_PIN_RESET);
							//
							  busVoltage_15 = INA229_getVBUS_V(INA229_15);

							//
							//HAL_Delay(10);
							  HAL_GPIO_WritePin(GPIOF, CELL11_CS_03_Pin, GPIO_PIN_SET);



							  HAL_GPIO_WritePin(GPIOF, CELL11_CS_03_Pin, GPIO_PIN_RESET);
							//
							  temperatureC_15 = INA229_getDIETEMP_C(INA229_15);

							//
							//HAL_Delay(10);
							HAL_GPIO_WritePin(GPIOF, CELL11_CS_03_Pin, GPIO_PIN_SET);
							HAL_Delay(10);

							Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_03_LED_01 , LOW);



								  	/////////////////////////////////////////////////////////////

						  	/////////////////////////////////////////////////////////////

							  Set_Output_Voltage(&hi2c3, CELL_4, 4.0f);
						Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_01_LED_01 , HIGH);

							  HAL_GPIO_WritePin(GPIOF, CELL11_CS_04_Pin, GPIO_PIN_RESET);
						//
						  busVoltage_16 = INA229_getVBUS_V(INA229_16);

						//
						//HAL_Delay(10);
						  HAL_GPIO_WritePin(GPIOF, CELL11_CS_04_Pin, GPIO_PIN_SET);



						  HAL_GPIO_WritePin(GPIOF, CELL11_CS_04_Pin, GPIO_PIN_RESET);
						//
						  temperatureC_16 = INA229_getDIETEMP_C(INA229_16);

						//
						//HAL_Delay(10);
						HAL_GPIO_WritePin(GPIOF, CELL11_CS_04_Pin, GPIO_PIN_SET);
						HAL_Delay(10);

						Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_01_LED_01 , LOW);

						///////////////////////////////////////////////////////////////////
						  Set_Output_Voltage(&hi2c3, CELL_5, 4.0f);
					Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_02_LED_01 , HIGH);

						  HAL_GPIO_WritePin(GPIOF, CELL11_CS_05_Pin, GPIO_PIN_RESET);
					//
					  busVoltage_17 = INA229_getVBUS_V(INA229_17);

					//
					//HAL_Delay(10);
					  HAL_GPIO_WritePin(GPIOF, CELL11_CS_05_Pin, GPIO_PIN_SET);



					  HAL_GPIO_WritePin(GPIOF, CELL11_CS_05_Pin, GPIO_PIN_RESET);
					//
					  temperatureC_17 = INA229_getDIETEMP_C(INA229_17);

					//
					//HAL_Delay(10);
					HAL_GPIO_WritePin(GPIOF, CELL11_CS_05_Pin, GPIO_PIN_SET);
					HAL_Delay(10);

					Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_02_LED_01 , LOW);



						  	/////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////
					  Set_Output_Voltage(&hi2c3, CELL_6, 4.0f);
				Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_03_LED_01 , HIGH);

					  HAL_GPIO_WritePin(GPIOF, CELL11_CS_06_Pin, GPIO_PIN_RESET);
				//
				  busVoltage_18 = INA229_getVBUS_V(INA229_18);

				//
				//HAL_Delay(10);
				  HAL_GPIO_WritePin(GPIOF, CELL11_CS_06_Pin, GPIO_PIN_SET);



				  HAL_GPIO_WritePin(GPIOF, CELL11_CS_06_Pin, GPIO_PIN_RESET);
				//
				  temperatureC_18 = INA229_getDIETEMP_C(INA229_18);

				//
				//HAL_Delay(10);
				HAL_GPIO_WritePin(GPIOF, CELL11_CS_06_Pin, GPIO_PIN_SET);
				HAL_Delay(10);

				Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_03_LED_01 , LOW);
			  	/////////////////////////////////////////////////////////////

				  Set_Output_Voltage(&hi2c3, CELL_7, 4.0f);
			Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_01_LED_01 , HIGH);

				  HAL_GPIO_WritePin(GPIOF, CELL11_CS_07_Pin, GPIO_PIN_RESET);
			//
			  busVoltage_19 = INA229_getVBUS_V(INA229_19);

			//
			//HAL_Delay(10);
			  HAL_GPIO_WritePin(GPIOF, CELL11_CS_07_Pin, GPIO_PIN_SET);



			  HAL_GPIO_WritePin(GPIOF, CELL11_CS_07_Pin, GPIO_PIN_RESET);
			//
			  temperatureC_19 = INA229_getDIETEMP_C(INA229_19);

			//
			//HAL_Delay(10);
			HAL_GPIO_WritePin(GPIOF, CELL11_CS_07_Pin, GPIO_PIN_SET);
			HAL_Delay(10);

			Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_01_LED_01 , LOW);

			///////////////////////////////////////////////////////////////////
			  Set_Output_Voltage(&hi2c3, CELL_8, 4.0f);
		Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_02_LED_01 , HIGH);

			  HAL_GPIO_WritePin(GPIOG, CELL11_CS_08_Pin, GPIO_PIN_RESET);
		//
		  busVoltage_20 = INA229_getVBUS_V(INA229_20);

		//
		//HAL_Delay(10);
		  HAL_GPIO_WritePin(GPIOG, CELL11_CS_08_Pin, GPIO_PIN_SET);



		  HAL_GPIO_WritePin(GPIOG, CELL11_CS_08_Pin, GPIO_PIN_RESET);
		//
		  temperatureC_20 = INA229_getDIETEMP_C(INA229_20);

		//
		//HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOG, CELL11_CS_08_Pin, GPIO_PIN_SET);
		HAL_Delay(10);

		Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_02_LED_01 , LOW);



			  	/////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////
		  Set_Output_Voltage(&hi2c3, CELL_9, 4.0f);
	Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_03_LED_01 , HIGH);

		  HAL_GPIO_WritePin(GPIOG, CELL11_CS_09_Pin, GPIO_PIN_RESET);
	//
	  busVoltage_21 = INA229_getVBUS_V(INA229_21);

	//
	//HAL_Delay(10);
	  HAL_GPIO_WritePin(GPIOG, CELL11_CS_09_Pin, GPIO_PIN_SET);



	  HAL_GPIO_WritePin(GPIOG, CELL11_CS_09_Pin, GPIO_PIN_RESET);
	//
	  temperatureC_21 = INA229_getDIETEMP_C(INA229_21);

	//
	//HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOG, CELL11_CS_09_Pin, GPIO_PIN_SET);
	HAL_Delay(10);

	Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_03_LED_01 , LOW);


					  	/////////////////////////////////////////////////////////////
  	/////////////////////////////////////////////////////////////

	  Set_Output_Voltage(&hi2c3, CELL_10, 4.0f);
Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_01_LED_01 , HIGH);

	  HAL_GPIO_WritePin(GPIOE, CELL11_CS_10_Pin, GPIO_PIN_RESET);
//
  busVoltage_22 = INA229_getVBUS_V(INA229_22);

//
//HAL_Delay(10);
  HAL_GPIO_WritePin(GPIOE, CELL11_CS_10_Pin, GPIO_PIN_SET);



  HAL_GPIO_WritePin(GPIOE, CELL11_CS_10_Pin, GPIO_PIN_RESET);
//
  temperatureC_22 = INA229_getDIETEMP_C(INA229_22);

//
//HAL_Delay(10);
HAL_GPIO_WritePin(GPIOE, CELL11_CS_10_Pin, GPIO_PIN_SET);
HAL_Delay(10);

Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_01_LED_01 , LOW);

///////////////////////////////////////////////////////////////////
  Set_Output_Voltage(&hi2c3, CELL_11, 4.0f);
Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_02_LED_01 , HIGH);

  HAL_GPIO_WritePin(GPIOE, CELL11_CS_11_Pin, GPIO_PIN_RESET);
//
busVoltage_23 = INA229_getVBUS_V(INA229_23);

//
//HAL_Delay(10);
HAL_GPIO_WritePin(GPIOE, CELL11_CS_11_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CELL11_CS_11_Pin, GPIO_PIN_RESET);
//
temperatureC_23 = INA229_getDIETEMP_C(INA229_23);

//
//HAL_Delay(10);
HAL_GPIO_WritePin(GPIOE, CELL11_CS_11_Pin, GPIO_PIN_SET);
HAL_Delay(10);

Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_02_LED_01 , LOW);



  	/////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
Set_Output_Voltage(&hi2c3, CELL_12, 4.0f);
Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_03_LED_01 , HIGH);

HAL_GPIO_WritePin(GPIOE, CELL11_CS_12_Pin, GPIO_PIN_RESET);
//
busVoltage_24 = INA229_getVBUS_V(INA229_24);

//
//HAL_Delay(10);
HAL_GPIO_WritePin(GPIOE, CELL11_CS_12_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CELL11_CS_12_Pin, GPIO_PIN_RESET);
//
temperatureC_24 = INA229_getDIETEMP_C(INA229_24);

//
//HAL_Delay(10);
HAL_GPIO_WritePin(GPIOE, CELL11_CS_12_Pin, GPIO_PIN_SET);
HAL_Delay(10);

Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_03_LED_01 , LOW);


		  	/////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


  HAL_GPIO_WritePin(GPIOF, CSU_12_CELLS_Pin, GPIO_PIN_RESET);
//
busVoltage_25 = INA229_getVBUS_V(INA229_12);

//
//HAL_Delay(10);
HAL_GPIO_WritePin(GPIOF, CSU_12_CELLS_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOF, CSU_12_CELLS_Pin, GPIO_PIN_RESET);
//
temperatureC_25 = INA229_getDIETEMP_C(INA229_12);

//
//HAL_Delay(10);
HAL_GPIO_WritePin(GPIOF, CSU_12_CELLS_Pin, GPIO_PIN_SET);
HAL_Delay(10);





  	/////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


HAL_GPIO_WritePin(GPIOE, CSU_11_CELLS_Pin, GPIO_PIN_RESET);
//
busVoltage_26 = INA229_getVBUS_V(INA229_25);

//
//HAL_Delay(10);
HAL_GPIO_WritePin(GPIOE, CSU_11_CELLS_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CSU_11_CELLS_Pin, GPIO_PIN_RESET);
//
temperatureC_26 = INA229_getDIETEMP_C(INA229_25);

//
//HAL_Delay(10);
HAL_GPIO_WritePin(GPIOE, CSU_11_CELLS_Pin, GPIO_PIN_SET);
HAL_Delay(10);


//--------------------------------------------------------------//

HAL_GPIO_WritePin(GPIOC, SPI3_CS_03_Pin|SPI3_CS_02_Pin, GPIO_PIN_RESET);


cell_voltage_read();
delay_time_us(500000);
temparature_data_read();

HAL_GPIO_WritePin(GPIOC, SPI3_CS_03_Pin|SPI3_CS_02_Pin, GPIO_PIN_SET);

HAL_Delay(10);


cell_voltage_read();
delay_time_us(500000);
temparature_data_read();


//delay_time_us(500000);
//--------------------------------------------------------------//

		  	/////////////////////////////////////////////////////////////
//Scan_I2C_Bus();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 10;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 16;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */

  /* USER CODE END CAN2_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 400000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 400000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI4_Init(void)
{

  /* USER CODE BEGIN SPI4_Init 0 */

  /* USER CODE END SPI4_Init 0 */

  /* USER CODE BEGIN SPI4_Init 1 */

  /* USER CODE END SPI4_Init 1 */
  /* SPI4 parameter configuration*/
  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI4_Init 2 */

  /* USER CODE END SPI4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CELL12_CS_01_Pin|CELL12_CS_02_Pin|CELL12_CS_03_Pin|CELL12_CS_04_Pin
                          |CELL12_CS_05_Pin|CELL11_CS_10_Pin|CELL11_CS_11_Pin|CELL11_CS_12_Pin
                          |CSU_11_CELLS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, CELL12_CS_06_Pin|CELL12_CS_08_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CELL12_CS_07_Pin|CELL12_TEMP_02_LED_Pin|CELL12_TEMP_01_CS_Pin|SPI3_CS_03_Pin
                          |SPI3_CS_02_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, CELL12_CS_09_Pin|CELL12_CS_10_Pin|LED_DC_Y_Pin|LED_DC_G_Pin
                          |LED_PC_Y_Pin|LED_PC_G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, CELL12_CS_11_Pin|CELL12_CS_12_Pin|CSU_12_CELLS_Pin|CELL12_TEMP_03_CS_Pin
                          |CELL12_TEMP_03_LED_Pin|CELL12_TEMP_02_CS_Pin|CELL11_CS_03_Pin|CELL11_CS_04_Pin
                          |CELL11_CS_05_Pin|CELL11_CS_06_Pin|CELL11_CS_07_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_02_Pin|GPIO_03_Pin|GPIO_04_Pin|GPIO_05_Pin
                          |GPIO_06_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CELL12_TEMP_01_LED_GPIO_Port, CELL12_TEMP_01_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USART2_ENABLE_GPIO_Port, USART2_ENABLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, GPIO_14_Pin|GPIO_07_Pin|LED_01_Pin|LED_02_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CELL11_CS_01_Pin|CELL11_CS_02_Pin|CELL11_TEMP_03_CS_Pin|CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, CELL11_CS_08_Pin|CELL11_CS_09_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_09_Pin|GPIO_10_Pin|LED_06_Pin|LED_05_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_11_Pin|GPIO_12_Pin|LED_03_Pin|LED_07_Pin
                          |USART1_ENABLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_LED_Pin|CELL11_TEMP_02_CS_Pin|CELL11_TEMP_02_LED_Pin|CELL11_TEMP_01_CS_Pin
                          |CELL11_TEMP_01_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, USART3_ENABLE_Pin|DISPLAY_CS_Pin|SPI5_CS_02_Pin|DIP_SWITCH_01_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, DISPLAY_IO_1_Pin|DISPLAY_IO_2_Pin|BACKLIGHT_1_Pin|BACKLIGHT_2_Pin
                          |LED_09_Pin|LED_04_Pin|LED_08_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USART6_ENABLE_GPIO_Port, USART6_ENABLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CELL12_CS_01_Pin CELL12_CS_02_Pin CELL12_CS_03_Pin CELL12_CS_04_Pin
                           CELL12_CS_05_Pin CELL11_CS_10_Pin CELL11_CS_11_Pin CELL11_CS_12_Pin
                           CSU_11_CELLS_Pin GPIO_09_Pin GPIO_10_Pin LED_06_Pin
                           LED_05_Pin */
  GPIO_InitStruct.Pin = CELL12_CS_01_Pin|CELL12_CS_02_Pin|CELL12_CS_03_Pin|CELL12_CS_04_Pin
                          |CELL12_CS_05_Pin|CELL11_CS_10_Pin|CELL11_CS_11_Pin|CELL11_CS_12_Pin
                          |CSU_11_CELLS_Pin|GPIO_09_Pin|GPIO_10_Pin|LED_06_Pin
                          |LED_05_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL12_CS_06_Pin CELL12_CS_08_Pin CELL12_CS_09_Pin CELL12_CS_10_Pin
                           LED_DC_Y_Pin LED_DC_G_Pin LED_PC_Y_Pin LED_PC_G_Pin */
  GPIO_InitStruct.Pin = CELL12_CS_06_Pin|CELL12_CS_08_Pin|CELL12_CS_09_Pin|CELL12_CS_10_Pin
                          |LED_DC_Y_Pin|LED_DC_G_Pin|LED_PC_Y_Pin|LED_PC_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL12_CS_07_Pin SPI3_CS_03_Pin SPI3_CS_02_Pin USART6_ENABLE_Pin */
  GPIO_InitStruct.Pin = CELL12_CS_07_Pin|SPI3_CS_03_Pin|SPI3_CS_02_Pin|USART6_ENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL12_CS_11_Pin CELL12_CS_12_Pin GPIO_02_Pin GPIO_03_Pin
                           GPIO_04_Pin GPIO_05_Pin GPIO_06_Pin CELL11_CS_03_Pin
                           CELL11_CS_04_Pin CELL11_CS_05_Pin CELL11_CS_06_Pin CELL11_CS_07_Pin */
  GPIO_InitStruct.Pin = CELL12_CS_11_Pin|CELL12_CS_12_Pin|GPIO_02_Pin|GPIO_03_Pin
                          |GPIO_04_Pin|GPIO_05_Pin|GPIO_06_Pin|CELL11_CS_03_Pin
                          |CELL11_CS_04_Pin|CELL11_CS_05_Pin|CELL11_CS_06_Pin|CELL11_CS_07_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : CSU_12_CELLS_Pin */
  GPIO_InitStruct.Pin = CSU_12_CELLS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CSU_12_CELLS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL12_TEMP_03_CS_Pin CELL12_TEMP_02_CS_Pin */
  GPIO_InitStruct.Pin = CELL12_TEMP_03_CS_Pin|CELL12_TEMP_02_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : CELL12_TEMP_03_LED_Pin */
  GPIO_InitStruct.Pin = CELL12_TEMP_03_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(CELL12_TEMP_03_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CELL12_TEMP_02_LED_Pin */
  GPIO_InitStruct.Pin = CELL12_TEMP_02_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(CELL12_TEMP_02_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CELL12_TEMP_01_CS_Pin */
  GPIO_InitStruct.Pin = CELL12_TEMP_01_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(CELL12_TEMP_01_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CELL12_TEMP_01_LED_Pin */
  GPIO_InitStruct.Pin = CELL12_TEMP_01_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(CELL12_TEMP_01_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USART2_ENABLE_Pin */
  GPIO_InitStruct.Pin = USART2_ENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(USART2_ENABLE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GPIO_14_Pin GPIO_07_Pin LED_01_Pin LED_02_Pin */
  GPIO_InitStruct.Pin = GPIO_14_Pin|GPIO_07_Pin|LED_01_Pin|LED_02_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL11_CS_01_Pin CELL11_CS_02_Pin GPIO_11_Pin GPIO_12_Pin
                           CS_Pin LED_03_Pin LED_07_Pin USART1_ENABLE_Pin */
  GPIO_InitStruct.Pin = CELL11_CS_01_Pin|CELL11_CS_02_Pin|GPIO_11_Pin|GPIO_12_Pin
                          |CS_Pin|LED_03_Pin|LED_07_Pin|USART1_ENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL11_CS_08_Pin CELL11_CS_09_Pin DISPLAY_IO_1_Pin DISPLAY_IO_2_Pin
                           BACKLIGHT_1_Pin BACKLIGHT_2_Pin LED_09_Pin LED_04_Pin
                           LED_08_Pin */
  GPIO_InitStruct.Pin = CELL11_CS_08_Pin|CELL11_CS_09_Pin|DISPLAY_IO_1_Pin|DISPLAY_IO_2_Pin
                          |BACKLIGHT_1_Pin|BACKLIGHT_2_Pin|LED_09_Pin|LED_04_Pin
                          |LED_08_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : CELL11_TEMP_03_CS_Pin */
  GPIO_InitStruct.Pin = CELL11_TEMP_03_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(CELL11_TEMP_03_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL11_TEMP_03_LED_Pin CELL11_TEMP_02_LED_Pin CELL11_TEMP_01_LED_Pin */
  GPIO_InitStruct.Pin = CELL11_TEMP_03_LED_Pin|CELL11_TEMP_02_LED_Pin|CELL11_TEMP_01_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL11_TEMP_02_CS_Pin CELL11_TEMP_01_CS_Pin */
  GPIO_InitStruct.Pin = CELL11_TEMP_02_CS_Pin|CELL11_TEMP_01_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : CS1_Pin USART3_ENABLE_Pin DISPLAY_CS_Pin SPI5_CS_02_Pin
                           DIP_SWITCH_01_Pin */
  GPIO_InitStruct.Pin = CS1_Pin|USART3_ENABLE_Pin|DISPLAY_CS_Pin|SPI5_CS_02_Pin
                          |DIP_SWITCH_01_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PUSH_BUTTON_01_Pin PUSH_BUTTON_02_Pin PUSH_BUTTON_03_Pin PUSH_BUTTON_04_Pin */
  GPIO_InitStruct.Pin = PUSH_BUTTON_01_Pin|PUSH_BUTTON_02_Pin|PUSH_BUTTON_03_Pin|PUSH_BUTTON_04_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : CD_Pin */
  GPIO_InitStruct.Pin = CD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(CD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DIP_SWITCH_02_Pin DIP_SWITCH_03_Pin DIP_SWITCH_04_Pin */
  GPIO_InitStruct.Pin = DIP_SWITCH_02_Pin|DIP_SWITCH_03_Pin|DIP_SWITCH_04_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : DIP_SWITCH_MODE_01_Pin DIP_SWITCH_MODE_02_Pin DIP_SWITCH_MODE_03_Pin DIP_SWITCH_MODE_04_Pin */
  GPIO_InitStruct.Pin = DIP_SWITCH_MODE_01_Pin|DIP_SWITCH_MODE_02_Pin|DIP_SWITCH_MODE_03_Pin|DIP_SWITCH_MODE_04_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void Scan_I2C_Bus(void)
{

      /* Declare the address variable */

    /* Iterate over the possible 7-bit I²C addresses (1 to 127) */
    for (address = 1U; address < 128U; address++)
    {
        /* Check if a device is ready at this address
         * The address is left-shifted by 1 as HAL_I2C_IsDeviceReady expects the 7-bit address in the upper bits.
         */
        result = HAL_I2C_IsDeviceReady(&hi2c2, (address << 1), 1U, 10U);

        if (result == HAL_OK)
        {
            /* Display the detected address (your Display_Address function should handle this)
             * For example, this could light up LEDs or update an LCD.
             */
            Display_Address(address);
            HAL_Delay(1000U);  /* Display the address for 1 second */
        }
    }
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
