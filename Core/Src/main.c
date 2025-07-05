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
#include "LTC68xx_API.h"
#include <stdlib.h>

#include "gpio_expander.h"
#include "rtc.h"
#include "button_led.h"

#include "BMS_test_protocol.h"

#include "modbusSlave.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

uint8_t modebus_rx_flag = 0 ;
uint8_t flag_1 = 0 ;
uint8_t flag_2 = 0 ;
uint8_t flag_3 = 0 ;

uint8_t RxData_modbus_01[256];
uint8_t TxData_modbus_01[256];

uint8_t RxData_modbus_02[256];
uint8_t TxData_modbus_02[256];

uint8_t RxData_modbus_03[256];
uint8_t TxData_modbus_03[256];

uint8_t RxData_modbus_04[256];
uint8_t TxData_modbus_04[256];

uint8_t address;
HAL_StatusTypeDef result;

int count =0 ;

#define UI_BUFFER_SIZE 32
#define CRC16_CCITT 0x1021
#define debug
#define BAUD    115200
#define T_char  750
#define T_frame 1750


INA229_Handle ina229_devices[NUM_INA229];



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

//void Scan_I2C_Bus(void);


//// Define configuration structure using constant values
//typedef struct {
//    uint8_t cell_id;      // Cell ID as a number instead of enum
//    uint8_t ina_index;    // Index of INA229 (0-23)
//    GPIO_TypeDef* gpio;   // GPIO port (this is actually a constant address)
//    uint16_t cs_pin;      // Chip select pin
//    uint16_t led_pin;     // LED pin
//} Cell_Config;


// Function declarations


void Voltage_Sequence_Automatic(void);

void Set_voltage_and_measure(const Cell_Config* cell, float voltage);

void init_ina229_devices(void) ;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);


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

    memset(RxData_modbus_01, 0, sizeof(RxData_modbus_01));
    memset(TxData_modbus_01, 0, sizeof(TxData_modbus_01));

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
  LED_Init();		/* Reset all LEDs */

  init_ina229_devices();

  for (int i = 0; i < NUM_INA229; i++)
  { INA229_config(ina229_devices[i]); HAL_Delay(10); }



  mcu_spiInit(0);
  HAL_Delay(10);


//



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

	HAL_UARTEx_ReceiveToIdle_IT(&huart1, RxData_modbus_01, 256);

	HAL_UARTEx_ReceiveToIdle_IT(&huart2, RxData_modbus_02, 256);

	HAL_UARTEx_ReceiveToIdle_IT(&huart3, RxData_modbus_03, 256);

	HAL_UARTEx_ReceiveToIdle_IT(&huart6, RxData_modbus_04, 256);

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
//	  LED_Set(7, 0);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */



	  if (modebus_rx_flag == 1 )

	  {
//		  memset(Rx_Data, 0, sizeof(Rx_Data));

//		  sprintf(Tx_Data, "F103 %d", indx++);
//		    send_Data (Tx_Data);
		    HAL_Delay(10);
	        tester_setup();


	    	memset(RxData_modbus_01, 0, sizeof(RxData_modbus_01));
	    	memset(TxData_modbus_01, 0, sizeof(TxData_modbus_01));
		  modebus_rx_flag = 0 ;

		  flag_1 = 5 ;

//		  HAL_TIM_Base_Stop_IT(&htim1);

	  }

		  cell12_Temp_01_Set(10);
		  cell12_Temp_02_Set(resistance[1]);
		  cell12_Temp_03_Set(resistance[2]);
		  cell11_Temp_01_Set(resistance[3]);
		  cell11_Temp_02_Set(resistance[4]);
		  cell11_Temp_03_Set(resistance[4]);

	  ////////////////////////////////////////////////////////////

	        // Perform voltage sequence and measurements

//		  Set_Output_Voltage(CELL_1, 4.2f);
//
//		  HAL_Delay(1000);
//
//		  Set_Output_Voltage(CELL_1, 2.0f);
//
//		  HAL_Delay(1000);
//
//		  Set_Output_Voltage(CELL_15, 4.2f);
//
//		  HAL_Delay(1000);
//
//
//		  Set_Output_Voltage(CELL_15, 2.0f);
//
//		  HAL_Delay(1000);
//
//		  Set_Output_Voltage(CELL_24, 4.2f);
//
//		  HAL_Delay(1000);
//
//		  Set_Output_Voltage(CELL_24, 2.0f);

//	        Voltage_Sequence_Automatic();

//	        tester_setup();

	        // Process battery tests
//	        for (int cell = CELL_1; cell <= CELL_24; cell++) {
//	        	Set_LED_status(cell, ON);
//	        }

//	        for (int cell = CELL_1; cell <= CELL_24; cell++) {
//	        	Set_LED_status(cell, OFF);
//	        }
//	        HAL_Delay(1000);
//
//	        for (int cell = CELL_1; cell <= CELL_24; cell++) {
//	        	Set_Output_Voltage(cell, 4.2);
//	        }
//
//
//	        HAL_Delay(1000);
//
//	        for (int cell = CELL_1; cell <= CELL_24; cell++) {
//	        	Set_Output_Voltage(cell, 2.0);
//	        }
//	        HAL_Delay(1000);
//
//	        for (int cell = CELL_1; cell <= CELL_24; cell++) {
//	        	Set_LED_status(cell, ON);
//	        }

//
//HAL_GPIO_WritePin(GPIOC, SPI3_CS_03_Pin|SPI3_CS_02_Pin, GPIO_PIN_RESET);
//
//
//cell_voltage_read();
//delay_time_us(500000);
//temparature_data_read();
//
//HAL_GPIO_WritePin(GPIOC, SPI3_CS_03_Pin|SPI3_CS_02_Pin, GPIO_PIN_SET);
//
//HAL_Delay(10);
//
//
//cell_voltage_read();
//delay_time_us(500000);
//temparature_data_read();


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
  HAL_GPIO_WritePin(GPIOI, CELL12_CS_06_Pin|CELL12_CS_08_Pin|CELL12_CS_09_Pin|CELL12_CS_10_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CELL12_CS_07_Pin|CELL12_TEMP_02_LED_Pin|CELL12_TEMP_01_CS_Pin|SPI3_CS_03_Pin
                          |SPI3_CS_02_Pin, GPIO_PIN_SET);

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
  HAL_GPIO_WritePin(GPIOD, USART3_ENABLE_Pin|DISPLAY_CS_Pin|SPI5_CS_02_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, DISPLAY_IO_1_Pin|DISPLAY_IO_2_Pin|BACKLIGHT_1_Pin|BACKLIGHT_2_Pin
                          |LED_09_Pin|LED_04_Pin|LED_08_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USART6_ENABLE_GPIO_Port, USART6_ENABLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, LED_DC_Y_Pin|LED_DC_G_Pin|LED_PC_Y_Pin|LED_PC_G_Pin, GPIO_PIN_RESET);

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

  /*Configure GPIO pins : CS1_Pin USART3_ENABLE_Pin DISPLAY_CS_Pin SPI5_CS_02_Pin */
  GPIO_InitStruct.Pin = CS1_Pin|USART3_ENABLE_Pin|DISPLAY_CS_Pin|SPI5_CS_02_Pin;
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

  /*Configure GPIO pins : DIP_SWITCH_01_Pin DIP_SWITCH_02_Pin DIP_SWITCH_03_Pin DIP_SWITCH_04_Pin */
  GPIO_InitStruct.Pin = DIP_SWITCH_01_Pin|DIP_SWITCH_02_Pin|DIP_SWITCH_03_Pin|DIP_SWITCH_04_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : DIP_SWITCH_MODE_01_Pin DIP_SWITCH_MODE_02_Pin DIP_SWITCH_MODE_03_Pin DIP_SWITCH_MODE_04_Pin */
  GPIO_InitStruct.Pin = DIP_SWITCH_MODE_01_Pin|DIP_SWITCH_MODE_02_Pin|DIP_SWITCH_MODE_03_Pin|DIP_SWITCH_MODE_04_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

//void init_ina229_devices(void) {
//    ina229_devices[0] = INA229_0;
//    ina229_devices[1] = INA229_1;
//    ina229_devices[2] = INA229_2;
//    ina229_devices[3] = INA229_3;
//    ina229_devices[4] = INA229_4;
//    ina229_devices[5] = INA229_5;
//    ina229_devices[6] = INA229_6;
//    ina229_devices[7] = INA229_7;
//    ina229_devices[8] = INA229_8;
//    ina229_devices[9] = INA229_9;
//    ina229_devices[10] = INA229_10;
//    ina229_devices[11] = INA229_11;
//    ina229_devices[12] = INA229_12;
//    ina229_devices[13] = INA229_13;
//    ina229_devices[14] = INA229_14;
//    ina229_devices[15] = INA229_15;
//    ina229_devices[16] = INA229_16;
//    ina229_devices[17] = INA229_17;
//    ina229_devices[18] = INA229_18;
//    ina229_devices[19] = INA229_19;
//    ina229_devices[20] = INA229_20;
//    ina229_devices[21] = INA229_21;
//    ina229_devices[22] = INA229_22;
//    ina229_devices[23] = INA229_23;
//    ina229_devices[24] = INA229_24;
//    ina229_devices[25] = INA229_25;
//}



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


// Helper macros to make the table more readable
#define I2C2_BUS 2
#define I2C3_BUS 3

//// Define the configurations table
//static const Cell_Config cell_configs[] = {
//		// First GPIO Expander (ID_01) - Cells 1-3
//
//		// FOR THE FIRST 12 CELLS
//    // Cell ID, INA idx, GPIO port,  CS pin,           LED pin
//    { 0,          0,       GPIOE,     CELL12_CS_01_Pin, CELL_01_LED_01},
//    { 1,          1,       GPIOE,     CELL12_CS_02_Pin, CELL_02_LED_01},
//    { 2,          2,       GPIOE,     CELL12_CS_03_Pin, CELL_03_LED_01},
//
//	// Second GPIO Expander (ID_02) - Cells 4-6
//    { 3,          3,       GPIOE,     CELL12_CS_04_Pin, CELL_01_LED_01},
//    { 4,          4,       GPIOE,     CELL12_CS_05_Pin, CELL_02_LED_01},
//    { 5,          5,       GPIOI,     CELL12_CS_06_Pin, CELL_03_LED_01},
//	// Third GPIO Expander (ID_03) - Cells 7-9
//    { 6,          6,       GPIOC,     CELL12_CS_07_Pin, CELL_01_LED_01},
//    { 7,          7,       GPIOI,     CELL12_CS_08_Pin, CELL_02_LED_01},
//    { 8,          8,       GPIOI,     CELL12_CS_09_Pin, CELL_03_LED_01},
//
//	// Fourth GPIO Expander (ID_04) - Cells 10-12
//    { 9,          9,       GPIOI,     CELL12_CS_10_Pin, CELL_01_LED_01},
//    {10,         10,       GPIOF,     CELL12_CS_11_Pin, CELL_02_LED_01},
//    {11,         11,       GPIOF,     CELL12_CS_12_Pin, CELL_03_LED_01},
//
//	// FOR THE SECOND 12 CELLS
//    // Cell ID, I2C bus, INA idx, GPIO port,  CS pin,           LED pin
//    {12,         13,       GPIOB,     CELL11_CS_01_Pin, CELL_01_LED_01},
//    {13,         14,       GPIOB,     CELL11_CS_02_Pin, CELL_02_LED_01},
//    {14,         15,       GPIOF,     CELL11_CS_03_Pin, CELL_03_LED_01},
//
//	// Second GPIO Expander (ID_02) - Cells 4-6
//    {15,         16,       GPIOF,     CELL11_CS_04_Pin, CELL_01_LED_01},
//    {16,         17,       GPIOF,     CELL11_CS_05_Pin, CELL_02_LED_01},
//    {17,         18,       GPIOF,     CELL11_CS_06_Pin, CELL_03_LED_01},
//	// Third GPIO Expander (ID_03) - Cells 7-9
//    {18,         19,       GPIOF,     CELL11_CS_07_Pin, CELL_01_LED_01},
//    {19,         20,       GPIOG,     CELL11_CS_08_Pin, CELL_02_LED_01},
//    {20,         21,       GPIOG,     CELL11_CS_09_Pin, CELL_03_LED_01},
//
//	// Fourth GPIO Expander (ID_04) - Cells 10-12
//    {21,         22,       GPIOE,     CELL11_CS_10_Pin, CELL_01_LED_01},
//    {22,         23,       GPIOE,     CELL11_CS_11_Pin, CELL_02_LED_01},
//    {23,         24,       GPIOE,     CELL11_CS_12_Pin, CELL_03_LED_01},
//
//	//12 cell CSU voltage and current reading
//	{24,         12,       GPIOF,     CSU_12_CELLS_Pin, CELL_01_LED_01},
//
//	//11 cell CSU voltage and current reading
//	{25,         25,       GPIOE,     CSU_11_CELLS_Pin, CELL_03_LED_01}
//
//
//};


// Helper function to get INA handle from index
static INA229_Handle get_ina_handle(uint8_t index) {
    return INA229_0 + index;  // Assuming INA handles are sequential
}

//void Voltage_Sequence_Automatic(void)
//{
//
//    const float test_voltages[] = {2.0f, 2.5f, 2.8f, 3.3f, 3.4f, 3.6f, 4.0f, 4.2f};
//    const int num_voltages = sizeof(test_voltages) / sizeof(test_voltages[0]);
//    const int num_cells = sizeof(cell_configs) / sizeof(cell_configs[0]);
//
//    for(int v = 0; v < num_voltages; v++) {
//        for(int c = 0; c < num_cells; c++) {
//        	Set_voltage_and_measure(&cell_configs[c], test_voltages[v]);
//
//        	cell_voltage_read();
//
//            HAL_Delay(10);  // Delay between cells
//        }
//        HAL_Delay(10);  // Delay between voltage levels
//    }
//
//
//
//
//
//
//}

//void Set_voltage_and_measure(const Cell_Config* cell, float voltage)
//
//{
//    // Get the actual handles from the configuration
//    INA229_Handle ina = get_ina_handle(cell->ina_index);
//
//    // Only set voltage and control LED for cells 0-11 (skip CSU cells 12 and 13) COZ this is for the slave board reading
//    if (cell->cell_id != 24 && cell->cell_id != 25)
//
//    {
//
//    // Set voltage for the cell
//    Set_Output_Voltage(cell->cell_id, voltage);
//
//    // Turn on LED
//
//    Set_LED_status(cell->cell_id, OFF);
//
//    }
//    HAL_Delay(10);
//    // Read voltage and temperature
//    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_RESET);
//    HAL_Delay(1);
//    INA229_Readings[cell->ina_index].voltage_V = INA229_getVBUS_V(ina);
//    HAL_Delay(1);
//    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_SET);
//
////    HAL_Delay(10);
//
////    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_RESET);
////    HAL_Delay(1);
////    INA229_Readings[cell->ina_index].temperature_C = INA229_getDIETEMP_C(ina);
////    HAL_Delay(1);
////    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_SET);
//
//    // Turn off LED
//    HAL_Delay(10);
//
//    // Only turn off LED for cells 0-11 (skip CSU cells 12 and 13)
//    if (cell->cell_id != 24 && cell->cell_id != 25)
//
//    {
//
//    Set_LED_status(cell->cell_id, ON);
//
//    }
//}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	/*

	if (RxData_modbus_01[0] == SLAVE_ID)
	{
		switch (RxData_modbus_01[1]){
		case 0x03:
			readHoldingRegs();
			break;
		case 0x04:
			readInputRegs();
			break;
		case 0x01:
			readCoils();
			break;
		case 0x02:
			readInputs();
			break;
		case 0x06:
			writeSingleReg();
			break;
		case 0x10:
			writeHoldingRegs();
			break;
		case 0x05:
			writeSingleCoil();
			break;
		case 0x0F:
			writeMultiCoils();
			break;
		default:
			modbusException(ILLEGAL_FUNCTION);
			break;
		}
	}
    */

	HAL_UARTEx_ReceiveToIdle_IT(&huart1, RxData_modbus_01, sizeof(RxData_modbus_01));

	HAL_UARTEx_ReceiveToIdle_IT(&huart2, RxData_modbus_02, sizeof(RxData_modbus_02));

	HAL_UARTEx_ReceiveToIdle_IT(&huart3, RxData_modbus_03, sizeof(RxData_modbus_03));

	HAL_UARTEx_ReceiveToIdle_IT(&huart6, RxData_modbus_04, sizeof(RxData_modbus_04));



	modebus_rx_flag = 1 ;

	  flag_1 = 2 ;

//    tester_setup();

//	modebus_rx_flag = 0;


//	HAL_UART_RxCpltCallback
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    UNUSED(GPIO_Pin);
    Push_ButtonHandler(GPIO_Pin);
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
