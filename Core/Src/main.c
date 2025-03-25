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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "display.h"

#include "gpio_expander.h"


#include "Temp_controller.h"
#include "main_data.h"
#include "INA229.h"
#include  "config.h"
#include "mcu.h"
#include <math.h>



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
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;

/* USER CODE BEGIN PV */
int resistance [6]= {4,8,12,16,20,24};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_SPI3_Init(void);
/* USER CODE BEGIN PFP */

void Scan_I2C_Bus(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  /* USER CODE BEGIN 2 */


  mcu_spiInit(0);
  HAL_Delay(10);
  HAL_Delay(10);

  INA229_config(INA229_0);

  HAL_Delay(10);
  INA229_config(&INA229_1);

  HAL_Delay(10);
  INA229_config(&INA229_2);

  HAL_Delay(10);
  INA229_config(&INA229_3);

  HAL_Delay(10);
  INA229_config(&INA229_4);

  INA229_config(&INA229_5);

  HAL_Delay(10);
  INA229_config(&INA229_6);

  HAL_Delay(10);
  INA229_config(&INA229_7);

  HAL_Delay(10);
  INA229_config(&INA229_8);

  HAL_Delay(10);
  INA229_config(&INA229_9);

  HAL_Delay(10);
  INA229_config(&INA229_10);

  HAL_Delay(10);
  INA229_config(&INA229_11);

  HAL_Delay(10);
  INA229_config(&INA229_12);

  HAL_Delay(10);

  INA229_config(INA229_13);

  HAL_Delay(10);
  INA229_config(&INA229_14);

  HAL_Delay(10);
  INA229_config(&INA229_15);

  HAL_Delay(10);
  INA229_config(&INA229_16);

  HAL_Delay(10);
  INA229_config(&INA229_17);

  HAL_Delay(10);
  INA229_config(&INA229_18);

  HAL_Delay(10);
  INA229_config(&INA229_19);

  HAL_Delay(10);
  INA229_config(&INA229_20);

  HAL_Delay(10);
  INA229_config(&INA229_21);

  HAL_Delay(10);
  INA229_config(&INA229_22);

  HAL_Delay(10);
  INA229_config(&INA229_23);

  HAL_Delay(10);
  INA229_config(&INA229_24);

  HAL_Delay(10);
  INA229_config(&INA229_25);

  HAL_Delay(10);



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
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	   Scan_I2C_Bus();
//	   Display_MainTitlePage();


	  Set_Output_Voltage(&hi2c2, CELL_1, 4.0f);
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

//		  cell12_Temp_01_Set(resistance[0]);
//		  cell12_Temp_02_Set(resistance[1]);
//		  cell12_Temp_03_Set(resistance[2]);
//		  cell11_Temp_01_Set(resistance[3]);
//		  cell11_Temp_02_Set(resistance[4]);
//		  cell11_Temp_03_Set(resistance[5]);

	  ////////////////////////////////////////////////////////////

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
HAL_Delay(1000);

Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);

///////////////////////////////////////////////////////////////////

Set_Output_Voltage(&hi2c2, CELL_2, 3.3f);

Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_02_LED_01 , HIGH);

			  HAL_GPIO_WritePin(GPIOE, CELL12_CS_02_Pin, GPIO_PIN_RESET);

			HAL_Delay(10);
//
		  busVoltage_02 = INA229_getVBUS_V(INA229_1);
		HAL_Delay(10);
//
//HAL_Delay(1000);
		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_02_Pin, GPIO_PIN_SET);



		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_02_Pin, GPIO_PIN_RESET);
//
		HAL_Delay(10);
		  temperatureC_02 = INA229_getDIETEMP_C(INA229_1);
		HAL_Delay(10);
//
//HAL_Delay(1000);
	  HAL_GPIO_WritePin(GPIOE, CELL12_CS_02_Pin, GPIO_PIN_SET);
	  HAL_Delay(1000);

	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_02_LED_01 , LOW);

	  ///////////////////////////////////////////////////////////////////
	 ///////////////////////////////////////////////////////////////////

			  Set_Output_Voltage(&hi2c2, CELL_3, 4.0f);

			  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_03_LED_01 , HIGH);

			  			  HAL_GPIO_WritePin(GPIOE, CELL12_CS_03_Pin, GPIO_PIN_RESET);
			  HAL_Delay(10);
			  		  busVoltage_03 = INA229_getVBUS_V(INA229_2);
			  		HAL_Delay(10);
			  //
			  //HAL_Delay(1000);
			  		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_03_Pin, GPIO_PIN_SET);
			  		HAL_Delay(10);


			  		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_03_Pin, GPIO_PIN_RESET);
			  //
			  		  temperatureC_03 = INA229_getDIETEMP_C(INA229_2);

			  //
			  //HAL_Delay(1000);
			  	  HAL_GPIO_WritePin(GPIOE, CELL12_CS_03_Pin, GPIO_PIN_SET);
			  	  HAL_Delay(1000);
			  	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_01, CELL_03_LED_01 , LOW);
			  	  ///////////////////////////////////////////////////////////////////

				  Set_Output_Voltage(&hi2c2, CELL_4, 3.6f);

				  Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_02, CELL_01_LED_01 , HIGH);

				  			  HAL_GPIO_WritePin(GPIOE, CELL12_CS_04_Pin, GPIO_PIN_RESET);
				  HAL_Delay(10);
				  		  busVoltage_04 = INA229_getVBUS_V(INA229_3);
				  		HAL_Delay(10);
				  //
				  //HAL_Delay(1000);
				  		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_04_Pin, GPIO_PIN_SET);
				  		HAL_Delay(10);


				  		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_04_Pin, GPIO_PIN_RESET);
				  //
				  		  temperatureC_04 = INA229_getDIETEMP_C(INA229_3);

				  //
				  //HAL_Delay(1000);
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
					  //HAL_Delay(1000);
					  		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_05_Pin, GPIO_PIN_SET);
					  		  HAL_Delay(1000);


					  		  HAL_GPIO_WritePin(GPIOE, CELL12_CS_05_Pin, GPIO_PIN_RESET);
					  //
					  		  temperatureC_05 = INA229_getDIETEMP_C(INA229_4);

					  //
					  //HAL_Delay(1000);
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
						  //HAL_Delay(1000);
						  		  HAL_GPIO_WritePin(GPIOI, CELL12_CS_06_Pin, GPIO_PIN_SET);
						  		  HAL_Delay(1000);


						  		  HAL_GPIO_WritePin(GPIOI, CELL12_CS_06_Pin, GPIO_PIN_RESET);
						  //
						  		  temperatureC_06 = INA229_getDIETEMP_C(INA229_5);

						  //
						  //HAL_Delay(1000);
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
							  //HAL_Delay(1000);
							  		HAL_GPIO_WritePin(CELL12_CS_07_GPIO_Port, CELL12_CS_07_Pin, GPIO_PIN_SET);
							  		  HAL_Delay(1000);


							  		HAL_GPIO_WritePin(CELL12_CS_07_GPIO_Port, CELL12_CS_07_Pin, GPIO_PIN_RESET);
							  //
							  		  temperatureC_07 = INA229_getDIETEMP_C(INA229_6);

							  //
							  //HAL_Delay(1000);
							  		HAL_GPIO_WritePin(CELL12_CS_07_GPIO_Port, CELL12_CS_07_Pin, GPIO_PIN_SET);
							  	  HAL_Delay(1000);
							  	Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_01_LED_01 , LOW);

								  ////////////////////////////////////////////////////////////

								  Set_Output_Voltage(&hi2c2, CELL_8, 2.5f);
					Expander_SetPinState(&hi2c2, GPIO_EXPANDER_ID_03, CELL_02_LED_01 , HIGH);
					HAL_Delay(10);
								  HAL_GPIO_WritePin(GPIOI, CELL12_CS_08_Pin, GPIO_PIN_RESET);
					//
							  busVoltage_08 = INA229_getVBUS_V(INA229_7);

					//
					//HAL_Delay(1000);
							  HAL_GPIO_WritePin(GPIOI, CELL12_CS_08_Pin, GPIO_PIN_SET);
							  HAL_Delay(10);


							  HAL_GPIO_WritePin(GPIOI, CELL12_CS_08_Pin, GPIO_PIN_RESET);
					//
							  temperatureC_08 = INA229_getDIETEMP_C(INA229_7);

					//
					//HAL_Delay(1000);
						  HAL_GPIO_WritePin(GPIOI, CELL12_CS_08_Pin, GPIO_PIN_SET);
						  HAL_Delay(1000);

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
						  //HAL_Delay(1000);
						  		  HAL_GPIO_WritePin(GPIOI, CELL12_CS_09_Pin, GPIO_PIN_SET);
						  		HAL_Delay(10);


						  		  HAL_GPIO_WritePin(GPIOI, CELL12_CS_09_Pin, GPIO_PIN_RESET);
						  //
						  		HAL_Delay(10);
						  		  temperatureC_09 = INA229_getDIETEMP_C(INA229_8);
						  		HAL_Delay(10);
						  //
						  //HAL_Delay(1000);
						  	  HAL_GPIO_WritePin(GPIOI, CELL12_CS_09_Pin, GPIO_PIN_SET);
						  	  HAL_Delay(1000);

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
							  //HAL_Delay(1000);
							  		  HAL_GPIO_WritePin(GPIOI, CELL12_CS_10_Pin, GPIO_PIN_SET);
							  		HAL_Delay(10);


							  		  HAL_GPIO_WritePin(GPIOI, CELL12_CS_10_Pin, GPIO_PIN_RESET);
							  //
							  		  temperatureC_10 = INA229_getDIETEMP_C(INA229_9);

							  //
							  //HAL_Delay(1000);
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
								  //HAL_Delay(1000);
								  		  HAL_GPIO_WritePin(GPIOF, CELL12_CS_11_Pin, GPIO_PIN_SET);
								  		HAL_Delay(10);


								  		  HAL_GPIO_WritePin(GPIOF, CELL12_CS_11_Pin, GPIO_PIN_RESET);
								  //
								  		  temperatureC_11 = INA229_getDIETEMP_C(INA229_10);

								  //
								  //HAL_Delay(1000);
								  	  HAL_GPIO_WritePin(GPIOF, CELL12_CS_11_Pin, GPIO_PIN_SET);
								  	  HAL_Delay(1000);
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
									  //HAL_Delay(1000);
									  		  HAL_GPIO_WritePin(GPIOF, CELL12_CS_12_Pin, GPIO_PIN_SET);
									  		HAL_Delay(10);


									  		  HAL_GPIO_WritePin(GPIOF, CELL12_CS_12_Pin, GPIO_PIN_RESET);
									  //
									  		  temperatureC_12 = INA229_getDIETEMP_C(INA229_11);

									  //
									  //HAL_Delay(1000);
									  	  HAL_GPIO_WritePin(GPIOF, CELL12_CS_12_Pin, GPIO_PIN_SET);
									  	  HAL_Delay(1000);
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
									//HAL_Delay(1000);
									  HAL_GPIO_WritePin(GPIOB, CELL11_CS_01_Pin, GPIO_PIN_SET);



									  HAL_GPIO_WritePin(GPIOB, CELL11_CS_01_Pin, GPIO_PIN_RESET);
									//
									  temperatureC_13 = INA229_getDIETEMP_C(INA229_13);

									//
									//HAL_Delay(1000);
									HAL_GPIO_WritePin(GPIOB, CELL11_CS_01_Pin, GPIO_PIN_SET);
									HAL_Delay(1000);

									Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_01_LED_01 , LOW);

									///////////////////////////////////////////////////////////////////
									  Set_Output_Voltage(&hi2c3, CELL_2, 4.0f);
								Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_02_LED_01 , HIGH);

									  HAL_GPIO_WritePin(GPIOB, CELL11_CS_02_Pin, GPIO_PIN_RESET);
								//
								  busVoltage_14 = INA229_getVBUS_V(INA229_14);

								//
								//HAL_Delay(1000);
								  HAL_GPIO_WritePin(GPIOB, CELL11_CS_02_Pin, GPIO_PIN_SET);



								  HAL_GPIO_WritePin(GPIOB, CELL11_CS_02_Pin, GPIO_PIN_RESET);
								//
								  temperatureC_14 = INA229_getDIETEMP_C(INA229_14);

								//
								//HAL_Delay(1000);
								HAL_GPIO_WritePin(GPIOB, CELL11_CS_02_Pin, GPIO_PIN_SET);
								HAL_Delay(1000);

								Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_02_LED_01 , LOW);



									  	/////////////////////////////////////////////////////////////
								///////////////////////////////////////////////////////////////////
								  Set_Output_Voltage(&hi2c3, CELL_3, 4.0f);
							Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_03_LED_01 , HIGH);

								  HAL_GPIO_WritePin(GPIOF, CELL11_CS_03_Pin, GPIO_PIN_RESET);
							//
							  busVoltage_15 = INA229_getVBUS_V(INA229_15);

							//
							//HAL_Delay(1000);
							  HAL_GPIO_WritePin(GPIOF, CELL11_CS_03_Pin, GPIO_PIN_SET);



							  HAL_GPIO_WritePin(GPIOF, CELL11_CS_03_Pin, GPIO_PIN_RESET);
							//
							  temperatureC_15 = INA229_getDIETEMP_C(INA229_15);

							//
							//HAL_Delay(1000);
							HAL_GPIO_WritePin(GPIOF, CELL11_CS_03_Pin, GPIO_PIN_SET);
							HAL_Delay(1000);

							Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_01, CELL_03_LED_01 , LOW);



								  	/////////////////////////////////////////////////////////////

						  	/////////////////////////////////////////////////////////////

							  Set_Output_Voltage(&hi2c3, CELL_4, 3.3f);
						Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_01_LED_01 , HIGH);

							  HAL_GPIO_WritePin(GPIOF, CELL11_CS_04_Pin, GPIO_PIN_RESET);
						//
						  busVoltage_16 = INA229_getVBUS_V(INA229_16);

						//
						//HAL_Delay(1000);
						  HAL_GPIO_WritePin(GPIOF, CELL11_CS_04_Pin, GPIO_PIN_SET);



						  HAL_GPIO_WritePin(GPIOF, CELL11_CS_04_Pin, GPIO_PIN_RESET);
						//
						  temperatureC_16 = INA229_getDIETEMP_C(INA229_16);

						//
						//HAL_Delay(1000);
						HAL_GPIO_WritePin(GPIOF, CELL11_CS_04_Pin, GPIO_PIN_SET);
						HAL_Delay(1000);

						Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_01_LED_01 , LOW);

						///////////////////////////////////////////////////////////////////
						  Set_Output_Voltage(&hi2c3, CELL_5, 3.3f);
					Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_02_LED_01 , HIGH);

						  HAL_GPIO_WritePin(GPIOF, CELL11_CS_05_Pin, GPIO_PIN_RESET);
					//
					  busVoltage_17 = INA229_getVBUS_V(INA229_17);

					//
					//HAL_Delay(1000);
					  HAL_GPIO_WritePin(GPIOF, CELL11_CS_05_Pin, GPIO_PIN_SET);



					  HAL_GPIO_WritePin(GPIOF, CELL11_CS_05_Pin, GPIO_PIN_RESET);
					//
					  temperatureC_17 = INA229_getDIETEMP_C(INA229_17);

					//
					//HAL_Delay(1000);
					HAL_GPIO_WritePin(GPIOF, CELL11_CS_05_Pin, GPIO_PIN_SET);
					HAL_Delay(1000);

					Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_02_LED_01 , LOW);



						  	/////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////
					  Set_Output_Voltage(&hi2c3, CELL_6, 3.3f);
				Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_03_LED_01 , HIGH);

					  HAL_GPIO_WritePin(GPIOF, CELL11_CS_06_Pin, GPIO_PIN_RESET);
				//
				  busVoltage_18 = INA229_getVBUS_V(INA229_18);

				//
				//HAL_Delay(1000);
				  HAL_GPIO_WritePin(GPIOF, CELL11_CS_06_Pin, GPIO_PIN_SET);



				  HAL_GPIO_WritePin(GPIOF, CELL11_CS_06_Pin, GPIO_PIN_RESET);
				//
				  temperatureC_18 = INA229_getDIETEMP_C(INA229_18);

				//
				//HAL_Delay(1000);
				HAL_GPIO_WritePin(GPIOF, CELL11_CS_06_Pin, GPIO_PIN_SET);
				HAL_Delay(1000);

				Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_02, CELL_03_LED_01 , LOW);
			  	/////////////////////////////////////////////////////////////

				  Set_Output_Voltage(&hi2c3, CELL_7, 4.0f);
			Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_01_LED_01 , HIGH);

				  HAL_GPIO_WritePin(GPIOF, CELL11_CS_07_Pin, GPIO_PIN_RESET);
			//
			  busVoltage_19 = INA229_getVBUS_V(INA229_19);

			//
			//HAL_Delay(1000);
			  HAL_GPIO_WritePin(GPIOF, CELL11_CS_07_Pin, GPIO_PIN_SET);



			  HAL_GPIO_WritePin(GPIOF, CELL11_CS_07_Pin, GPIO_PIN_RESET);
			//
			  temperatureC_19 = INA229_getDIETEMP_C(INA229_19);

			//
			//HAL_Delay(1000);
			HAL_GPIO_WritePin(GPIOF, CELL11_CS_07_Pin, GPIO_PIN_SET);
			HAL_Delay(1000);

			Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_01_LED_01 , LOW);

			///////////////////////////////////////////////////////////////////
			  Set_Output_Voltage(&hi2c3, CELL_8, 4.0f);
		Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_02_LED_01 , HIGH);

			  HAL_GPIO_WritePin(GPIOG, CELL11_CS_08_Pin, GPIO_PIN_RESET);
		//
		  busVoltage_20 = INA229_getVBUS_V(INA229_20);

		//
		//HAL_Delay(1000);
		  HAL_GPIO_WritePin(GPIOG, CELL11_CS_05_Pin, GPIO_PIN_SET);



		  HAL_GPIO_WritePin(GPIOG, CELL11_CS_08_Pin, GPIO_PIN_RESET);
		//
		  temperatureC_20 = INA229_getDIETEMP_C(INA229_20);

		//
		//HAL_Delay(1000);
		HAL_GPIO_WritePin(GPIOG, CELL11_CS_08_Pin, GPIO_PIN_SET);
		HAL_Delay(1000);

		Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_02_LED_01 , LOW);



			  	/////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////
		  Set_Output_Voltage(&hi2c3, CELL_9, 4.0f);
	Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_03_LED_01 , HIGH);

		  HAL_GPIO_WritePin(GPIOG, CELL11_CS_09_Pin, GPIO_PIN_RESET);
	//
	  busVoltage_21 = INA229_getVBUS_V(INA229_21);

	//
	//HAL_Delay(1000);
	  HAL_GPIO_WritePin(GPIOG, CELL11_CS_09_Pin, GPIO_PIN_SET);



	  HAL_GPIO_WritePin(GPIOG, CELL11_CS_09_Pin, GPIO_PIN_RESET);
	//
	  temperatureC_21 = INA229_getDIETEMP_C(INA229_21);

	//
	//HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOG, CELL11_CS_09_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);

	Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_03, CELL_03_LED_01 , LOW);


					  	/////////////////////////////////////////////////////////////
  	/////////////////////////////////////////////////////////////

	  Set_Output_Voltage(&hi2c3, CELL_10, 4.0f);
Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_01_LED_01 , HIGH);

	  HAL_GPIO_WritePin(GPIOE, CELL11_CS_10_Pin, GPIO_PIN_RESET);
//
  busVoltage_22 = INA229_getVBUS_V(INA229_22);

//
//HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOE, CELL11_CS_10_Pin, GPIO_PIN_SET);



  HAL_GPIO_WritePin(GPIOE, CELL11_CS_10_Pin, GPIO_PIN_RESET);
//
  temperatureC_22 = INA229_getDIETEMP_C(INA229_22);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL11_CS_10_Pin, GPIO_PIN_SET);
HAL_Delay(1000);

Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_01_LED_01 , LOW);

///////////////////////////////////////////////////////////////////
  Set_Output_Voltage(&hi2c3, CELL_11, 4.0f);
Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_02_LED_01 , HIGH);

  HAL_GPIO_WritePin(GPIOE, CELL11_CS_11_Pin, GPIO_PIN_RESET);
//
busVoltage_23 = INA229_getVBUS_V(INA229_23);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL11_CS_11_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CELL11_CS_11_Pin, GPIO_PIN_RESET);
//
temperatureC_23 = INA229_getDIETEMP_C(INA229_23);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL11_CS_11_Pin, GPIO_PIN_SET);
HAL_Delay(1000);

Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_02_LED_01 , LOW);



  	/////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
Set_Output_Voltage(&hi2c3, CELL_12, 4.0f);
Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_03_LED_01 , HIGH);

HAL_GPIO_WritePin(GPIOE, CELL11_CS_12_Pin, GPIO_PIN_RESET);
//
busVoltage_24 = INA229_getVBUS_V(INA229_24);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL11_CS_12_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CELL11_CS_12_Pin, GPIO_PIN_RESET);
//
temperatureC_24 = INA229_getDIETEMP_C(INA229_24);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CELL11_CS_12_Pin, GPIO_PIN_SET);
HAL_Delay(1000);

Expander_SetPinState(&hi2c3, GPIO_EXPANDER_ID_04, CELL_03_LED_01 , LOW);


		  	/////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


  HAL_GPIO_WritePin(GPIOF, CSU_12_CELLS_Pin, GPIO_PIN_RESET);
//
busVoltage_25 = INA229_getVBUS_V(INA229_12);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOF, CSU_12_CELLS_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOF, CSU_12_CELLS_Pin, GPIO_PIN_RESET);
//
temperatureC_25 = INA229_getDIETEMP_C(INA229_12);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOF, CSU_12_CELLS_Pin, GPIO_PIN_SET);
HAL_Delay(1000);





  	/////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


HAL_GPIO_WritePin(GPIOE, CSU_11_CELLS_Pin, GPIO_PIN_RESET);
//
busVoltage_26 = INA229_getVBUS_V(INA229_25);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CSU_11_CELLS_Pin, GPIO_PIN_SET);



HAL_GPIO_WritePin(GPIOE, CSU_11_CELLS_Pin, GPIO_PIN_RESET);
//
temperatureC_26 = INA229_getDIETEMP_C(INA229_25);

//
//HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOE, CSU_11_CELLS_Pin, GPIO_PIN_SET);
HAL_Delay(1000);




		  	/////////////////////////////////////////////////////////////


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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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
  HAL_GPIO_WritePin(GPIOI, CELL12_CS_06_Pin|CELL12_CS_08_Pin|CELL12_CS_10_Pin|CELL12_CS_09_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CELL12_CS_07_GPIO_Port, CELL12_CS_07_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, CELL12_CS_11_Pin|CELL12_CS_12_Pin|CSU_12_CELLS_Pin|CELL12_TEMP_03_CS_Pin
                          |CELL12_TEMP_03_LED_Pin|CELL12_TEMP_02_CS_Pin|CELL11_CS_03_Pin|CELL11_CS_04_Pin
                          |CELL11_CS_05_Pin|CELL11_CS_06_Pin|CELL11_CS_07_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_02_LED_Pin|CELL12_TEMP_01_CS_Pin|SPI3_CS_03_Pin|SPI3_CS_02_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CELL12_TEMP_01_LED_Pin|GPIO_PIN_1, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CELL11_CS_01_Pin|CELL11_CS_02_Pin|CELL11_TEMP_03_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, CELL11_CS_08_Pin|CELL11_CS_09_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_LED_Pin|CELL11_TEMP_02_CS_Pin|CELL11_TEMP_02_LED_Pin|CELL11_TEMP_01_CS_Pin
                          |CELL11_TEMP_01_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, DISPLAY_IO_1_Pin|DISPLAY_IO_2_Pin|BACKLIGHT_1_Pin|BACKLIGHT_2_Pin
                          |LED_09_Pin|LED_04_Pin|LED_08_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, LED_01_Pin|LED_02_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, DISPLAY_CS_Pin|DISPLAY_CSD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_03_Pin|LED_07_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED_06_Pin|LED_05_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, LED_DC_Y_Pin|LED_DC_G_Pin|LED_PC_Y_Pin|LED_PC_G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CELL12_CS_01_Pin CELL12_CS_02_Pin CELL12_CS_03_Pin CELL12_CS_04_Pin
                           CELL12_CS_05_Pin */
  GPIO_InitStruct.Pin = CELL12_CS_01_Pin|CELL12_CS_02_Pin|CELL12_CS_03_Pin|CELL12_CS_04_Pin
                          |CELL12_CS_05_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL12_CS_06_Pin LED_DC_Y_Pin LED_DC_G_Pin LED_PC_Y_Pin
                           LED_PC_G_Pin */
  GPIO_InitStruct.Pin = CELL12_CS_06_Pin|LED_DC_Y_Pin|LED_DC_G_Pin|LED_PC_Y_Pin
                          |LED_PC_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL12_CS_07_Pin SPI3_CS_03_Pin SPI3_CS_02_Pin */
  GPIO_InitStruct.Pin = CELL12_CS_07_Pin|SPI3_CS_03_Pin|SPI3_CS_02_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL12_CS_08_Pin CELL12_CS_10_Pin CELL12_CS_09_Pin */
  GPIO_InitStruct.Pin = CELL12_CS_08_Pin|CELL12_CS_10_Pin|CELL12_CS_09_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL12_CS_11_Pin CELL11_CS_03_Pin CELL11_CS_04_Pin CELL11_CS_05_Pin
                           CELL11_CS_06_Pin CELL11_CS_07_Pin */
  GPIO_InitStruct.Pin = CELL12_CS_11_Pin|CELL11_CS_03_Pin|CELL11_CS_04_Pin|CELL11_CS_05_Pin
                          |CELL11_CS_06_Pin|CELL11_CS_07_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL12_CS_12_Pin CSU_12_CELLS_Pin */
  GPIO_InitStruct.Pin = CELL12_CS_12_Pin|CSU_12_CELLS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

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

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : CELL11_CS_01_Pin CELL11_CS_02_Pin LED_03_Pin LED_07_Pin */
  GPIO_InitStruct.Pin = CELL11_CS_01_Pin|CELL11_CS_02_Pin|LED_03_Pin|LED_07_Pin;
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

  /*Configure GPIO pins : CELL11_CS_10_Pin CELL11_CS_11_Pin CELL11_CS_12_Pin CSU_11_CELLS_Pin
                           LED_06_Pin LED_05_Pin */
  GPIO_InitStruct.Pin = CELL11_CS_10_Pin|CELL11_CS_11_Pin|CELL11_CS_12_Pin|CSU_11_CELLS_Pin
                          |LED_06_Pin|LED_05_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

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

  /*Configure GPIO pins : LED_01_Pin LED_02_Pin */
  GPIO_InitStruct.Pin = LED_01_Pin|LED_02_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : DISPLAY_CS_Pin DISPLAY_CSD3_Pin */
  GPIO_InitStruct.Pin = DISPLAY_CS_Pin|DISPLAY_CSD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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
