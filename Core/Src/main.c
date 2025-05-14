/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "can.h"
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "u8g2.h"
#include "Display.h"
#include "string.h"
#include "stm32f4xx_hal.h"
#include "RTC.h"
#include "EEPROM.h"
#include "fatfs_sd.h"
#include "LED_BuiltIn.h"
#include "Switches.h"
#include "MOD_BUS.h"
#include "CAN_BUS.h"
#include "Menu.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//#define BUFFER_SIZE 256
//char RS485Buffer[BUFFER_SIZE];
//
//#define RS485_BUFFER_SIZE 64
//char buffer[RS485_BUFFER_SIZE];

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];
uint32_t Total, Free;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void StartReceiving(void);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
void display_lcd_scrolling(const char *message, uint16_t offset);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int bufsize (char *buf)
{
	int i=0;
	while (*buf++ != '\0') i++;
	return i;
}

void clear_buffer (void)
{
	for (int i=0; i<BUFFER_SIZE; i++) buffer[i] = '\0';
}

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
  MX_SPI3_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_SPI4_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_FATFS_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

//  RTC_Init(&hi2c1);


  MODBUS_Init(MODBUS_PC);
  MODBUS_Init(MODBUS_DC);
  MODBUS_Init(MODBUS_CH3);
  MODBUS_Init(MODBUS_CH4);

  LED_Init();

  Display_Init();
//  MainTitlePage();

//  Menu_Init();
//  Menu_Draw();

//  display_lcd("System Starting...");
//  HAL_Delay(1000);
//
//  // Show progress bar demo
//  for(int i = 0; i <= 81; i += 10) {
//      display_progress_bar("Loading", (float)i);
//      HAL_Delay(50);
//  }
//  HAL_Delay(500);
//  Display_Clear();

//  RTC_SetTime(00,31,9,4,21,4,25);

//  RTC_TrimByDeviation(26, 120); /*Set Time*/

//  RTC_SetTrim(0);

//  uint32_t Count = 0;

//  srand(HAL_GetTick());
//  char randomPacket[64];
//  size_t packetSize = 8;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//      for (int i = 0; i < MODBUS_CHANNEL_COUNT; i++) {
//          ProcessMODBUSData(i);
//      }
////      osDelay(50);
//
//	  UpdateChannelDisplay(); // Update display for all channels
//      HAL_Delay(100);

//	    for (size_t i = 0; i < packetSize; i++) {
//	        uint8_t randomByte = rand() % 256; // Generate a random byte (0x00 - 0xFF)
//	        sprintf(&randomPacket[i * 3], "%02X ", randomByte); // Format as "XX "
//	    }
//	    randomPacket[packetSize * 3 - 1] = '\0'; // Null-terminate the packet
//
//	    MODBUS_Send(MODBUS_PC, randomPacket, packetSize);
//        HAL_Delay(500);

//		uint8_t matrix[RS485_CHANNEL_COUNT][RS485_BUFFER_SIZE] = {0};
//		uint8_t previous_matrix[RS485_CHANNEL_COUNT][RS485_BUFFER_SIZE] = {0};
//
//	    RS485_GetMatrixData(matrix);
//
//	    for (int i = 0; i < RS485_CHANNEL_COUNT; i++) {
//	        // Check if the data for this channel has changed
//	        if (strcmp((char*)matrix[i], (char*)previous_matrix[i]) != 0) {
//	            UpdateChannelDisplay(i, (const uint8_t*)matrix[i], strlen((const char *)matrix[i])); // Update display for the channel
//	            strcpy((char*)previous_matrix[i], (char*)matrix[i]); // Update the previous state
//	        }
//	    }

	    // Draw something visible
//	    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
//	    u8g2_DrawStr(&u8g2, 0, 15, "Update Area");

	    // Update a specific area
//	    UpdateDisplayArea(0, 6, 16, 2); // Top-left corner, 16 blocks wide, 2 pages tall
//      uint8_t matrix[RS485_CHANNEL_COUNT][RS485_BUFFER_SIZE] = {0};
//      uint8_t previous_matrix[RS485_CHANNEL_COUNT][RS485_BUFFER_SIZE] = {0};
//
//      RS485_GetMatrixData(matrix);
//
//      for (int i = 0; i < RS485_CHANNEL_COUNT; i++) {
//          // Check if the data for this channel has changed
//          if (strcmp((char*)matrix[i], (char*)previous_matrix[i]) != 0) {
//              UpdateChannel(i, (char*)matrix[i]);
//              strcpy((char*)previous_matrix[i], (char*)matrix[i]); // Update the previous state
//          }
//      }

//      HAL_Delay(100); // Update every second

//	  Check_SDCard();
//	  LED_Toggle(1);
//	  HAL_Delay(100);

//	  snprintf(RS485Buffer, RS485_BUFFER_SIZE, "Testing, Count: %lu\r\n", Count);
//	  RS485_Send(RS485_CH2, RS485Buffer);
//	  Count++;
//
//      if(RS485_Available(RS485_CH1))
//      {
//          uint16_t length = RS485_GetData(RS485_CH1, buffer);
//
//          if(length > 0)
//          {
//              buffer[length] = '\0';  // Null terminate
//              display_lcd(buffer);
//          }
//      }
//      HAL_Delay(100);

//	  RTC_ReadTime();
//	  mode = DIP_GetMode();
//	  id = DIP_GetID();
//
//	  do {
//
//		  char timeStr[16];
//		  char dateStr[16];
//		  sprintf(timeStr, "%02d:%02d:%02d", time.hour, time.minute, time.second);
//		  sprintf(dateStr, "%02d/%02d/%02d", time.day, time.month, time.year);
//
//		  u8g2_ClearBuffer(&u8g2);
//		  u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
//		  u8g2_DrawStr(&u8g2, 85, 62, timeStr);
//		  u8g2_DrawStr(&u8g2, 0, 62, dateStr);
//
//		  char modeStr[16];
//		  char idStr[16];
//		  sprintf(modeStr, "MODE: %2d", mode);
//		  sprintf(idStr, "ID: %2d", id);
//
//		  u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
//		  u8g2_DrawStr(&u8g2, 5, 8, modeStr);
//		  u8g2_DrawStr(&u8g2, 80, 8, idStr);
//
//	  } while (u8g2_NextPage(&u8g2));
//
//	  HAL_Delay(500);

//	  counter++;
//	  display_lcd(&counter);

//	  Display_PowerSave(1);
//	  HAL_Delay(3000);
//	  Display_PowerSave(0);
//	  HAL_Delay(500);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
