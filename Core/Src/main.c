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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "u8g2.h"
#include "display.h"
#include "string.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "RTC.h"
#include "EEPROM.h"
#include "fatfs.h"
#include "fatfs_sd.h"
#include "LED_BuiltIn.h"
#include "Switches.h"
#include "MOD_BUS.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SD_CARD_PRESENT()  (HAL_GPIO_ReadPin(CD_GPIO_Port, CD_Pin) == GPIO_PIN_RESET)
//#define POPUP_TIMEOUT    1000
#define SD_CHECK_INTERVAL  50

//#define SD_CHECK_INTERVAL   500   // Check every 500ms
//#define CD_INSERTED        GPIO_PIN_RESET
//#define CD_REMOVED         GPIO_PIN_SET

//#define RS485_BUFFER_SIZE 64
//#define TEST_MSG "RS485\r\n"

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//uint8_t rx_buffer[RS485_BUFFER_SIZE];
//uint8_t rx_complete = 0;

extern u8g2_t u8g2;

//CAN_FilterTypeDef sFilterConfig;
//
//CAN_TxHeaderTypeDef TxHeader;
//CAN_RxHeaderTypeDef RxHeader;
//uint8_t TxData[8];
//uint8_t RxData[8];
//uint32_t TxMailbox;

//char msg[50];

//uint8_t a, r;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void display_lcd(const char *message);
void display_progress_bar(const char *message, float percentage);
void SD_CardHandler(void);
void display_popup(const char *message);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];  // to store strings..

//int i=0;

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

//uint16_t readValue;

CAN_TxHeaderTypeDef TxHeader;
uint32_t TxMailbox;
uint8_t TxData[8];

//uint8_t TxCAN[8];
//uint8_t RxCAN[8];
//char msg[50];

//int datacheck = 0;

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if (GPIO_Pin == PUSH_BUTTON_01_Pin)
//	{
//		TxCAN[0] = 0x11;
//		TxCAN[1] = 0x22;
//		TxCAN[2] = 0x33;
//		TxCAN[3] = 0x44;
//		TxCAN[4] = 0x55;
//		TxCAN[5] = 0x66;
//		TxCAN[6] = 0x77;
//		TxCAN[7] = 0x88;
////		TxCAN[7] = 200;
//		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxCAN, &TxMailbox);
//		HAL_GPIO_TogglePin(LED_07_GPIO_Port, LED_07_Pin);
//	}
//}

//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
//{
//	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxCAN);
//	if (RxHeader.DLC == 2)
//	{
//		datacheck = 1;
//	}
//}

//int indx = 0;

//void RS485_Send(uint8_t *data, uint16_t len)
//{
//    HAL_GPIO_WritePin(USART1_ENABLE_GPIO_Port, USART1_ENABLE_Pin, GPIO_PIN_SET);
//    HAL_Delay(1);
//    HAL_UART_Transmit(&huart1, data, len, 1000);
//    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);
//    HAL_GPIO_WritePin(USART1_ENABLE_GPIO_Port, USART1_ENABLE_Pin, GPIO_PIN_RESET);
//}
//
//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//{
//    if(huart->Instance == USART1)
//    {
//        rx_complete = 1;
//        HAL_UARTEx_ReceiveToIdle_IT(&huart1, rx_buffer, RS485_BUFFER_SIZE);
//    }
//}

/*int8_t TxData[16];
char data[] = "Hello";
uint8_t RxData[16];

void sendData (int8_t *data)
{
	HAL_GPIO_WritePin(USART1_ENABLE_GPIO_Port, USART6_ENABLE_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart1, data, strlen(data), 1000);
	HAL_GPIO_WritePin(USART1_ENABLE_GPIO_Port, USART1_ENABLE_Pin, GPIO_PIN_RESET);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	HAL_UARTEx_ReceiveToIdle_IT(&huart1, RxData, 16);
}*/

int BCD2DEC(uint8_t val) {
    return (int)((val / 16 * 10) + (val % 16));
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
  /* USER CODE BEGIN 2 */

  LED_Init();

  EEPROM_Init();

  RS485_Init(RS485_CH1);
  RS485_Init(RS485_CH2);
  RS485_Init(RS485_CH3);
  RS485_Init(RS485_CH6);

//  fresult = f_mount(&fs, "/", 1);

//  	if (fresult != FR_OK) send_uart ("ERROR!!! in mounting SD CARD...\n\n");
//  	else send_uart("SD CARD mounted successfully...\n\n");


  	/*************** Card capacity details ********************/

  	/* Check free space */
//  	f_getfree("", &fre_clust, &pfs);

//  	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
//  	sprintf (buffer, "Total Size: \t%lu\n",total);
//  	send_uart(buffer);
//  	clear_buffer();
//  	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
//  	sprintf (buffer, "Free Space: \t%lu\n\n",free_space);
//  	send_uart(buffer);
//  	clear_buffer();

//    fresult = f_mount(&fs, "/", 1);
//    	if (fresult != FR_OK) display_lcd("ERROR!");
//    	else display_lcd("SD CARD OK!");

  /************* The following operation is using PUTS and GETS *********************/

//    	/* Open file to write/ create a file if it doesn't exist */
//        fresult = f_open(&fil, "file1.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
//
//    	/* Writing text */
//    	f_puts("This data is from the FILE1.txt. And it was written using ...f_puts... ", &fil);
//
//    	/* Close file */
//    	fresult = f_close(&fil);
//
//    	if (fresult == FR_OK)send_uart ("File1.txt created and the data is written \n");
//
//    	/* Open file to read */
//    	fresult = f_open(&fil, "file1.txt", FA_READ);
//
//    	/* Read string from the file */
//    	f_gets(buffer, f_size(&fil), &fil);
//
////    	send_uart("File1.txt is opened and it contains the data as shown below\n");
////    	send_uart(buffer);
////    	send_uart("\n\n");
//
//    	/* Close file */
//    	f_close(&fil);
//
////    	clear_buffer();

//  HAL_CAN_Start(&hcan1);
//  TxHeader.DLC = 8;
//  TxHeader.IDE = CAN_ID_STD;
//  TxHeader.RTR = CAN_RTR_DATA;
//  TxHeader.StdId = 0x60;

//  TxData[0] = 0x11;
//  TxData[1] = 0x22;
//  TxData[2] = 0x33;
//  TxData[3] = 0x44;
//  TxData[4] = 0x55;
//  TxData[5] = 0x66;
//  TxData[6] = 0x77;
//  TxData[7] = 0x88;
//
//      if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
//      {
//          Error_Handler(); // Handle transmission error
//      }


//  sFilterConfig.FilterFIFOAssignment=CAN_FILTER_FIFO0;
//  sFilterConfig.FilterIdHigh=0x245<<5;
//  sFilterConfig.FilterIdLow=0;
//  sFilterConfig.FilterMaskIdHigh=0;
//  sFilterConfig.FilterMaskIdLow=0;
//  sFilterConfig.FilterScale=CAN_FILTERSCALE_32BIT;
//  sFilterConfig.FilterActivation=ENABLE;
//
//  HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
//  HAL_CAN_Start(&hcan1);
//  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

//  canFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
//  canFilterConfig.FilterFIFOAssignment = CAN
//

//  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

//  TxHeader.DLC = 8; /* DATA LENGTH */
//  TxHeader.IDE = CAN_ID_STD;
//  TxHeader.RTR = CAN_RTR_DATA;
//  TxHeader.StdId = 0x429;

//  HAL_GPIO_WritePin(USART1_ENABLE_GPIO_Port, USART1_ENABLE_Pin, GPIO_PIN_RESET);
//  HAL_UARTEx_ReceiveToIdle_IT(&huart1, rx_buffer, RS485_BUFFER_SIZE);

//  HAL_UARTEx_ReceiveToIdle_IT(&huart6, RxData, 16);

  u8g2_Setup_st7565_64128n_f(&u8g2, U8G2_R2, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);

  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);

  u8g2_ClearDisplay(&u8g2);
  u8g2_SetContrast(&u8g2, 100);
  u8g2_FirstPage(&u8g2);
  u8g2_ClearDisplay(&u8g2);

  u8g2_FirstPage(&u8g2);
  HAL_Delay(500);
  MainTitlePage();
  u8g2_ClearBuffer(&u8g2);

  LED_SetAll(ON);
  HAL_Delay(500);
  LED_SetAll(OFF);

//  LED_State(1, ON);

  RTC_Init(&hi2c1);
  RTC_TimeLapseInit();

//  display_lcd(message);

/////  RTC_SetTime(00,29,12,5,03,03,25);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  RS485_Send(RS485_CH2, "Testing from USART2\r\n");

      if(RS485_Available(RS485_CH3))
      {
          char buffer[64];
          uint16_t length = RS485_GetData(RS485_CH3, buffer);

          if(length > 0)
          {
              buffer[length] = '\0';  // Null terminate
              display_lcd(buffer);
          }
      }

	  HAL_Delay(1000);

//	  SD_CardHandler();
//	  HAL_Delay(10);

/*	    RS485_Send((uint8_t*)TEST_MSG, strlen(TEST_MSG));

	    // Check if we received any data
	    if(rx_complete)
	    {
	        // Echo received data to LCD
	        display_lcd((char*)rx_buffer);
	        rx_complete = 0;
	    }

	    HAL_Delay(1000);
*/
//      HAL_GPIO_WritePin(GPIOH, Temp_LED_Pin, RESET);
//      HAL_Delay(1000);
//      HAL_GPIO_WritePin(GPIOH, Temp_LED_Pin, SET);
//      HAL_Delay(1000);

//	  	  RTC_ReadTime();

//	  	  display_lcd(buffer);

//		  RTC_TimeLapseCheck();
//
//		  uint8_t read_buffer[7];
//		  if (EEPROM_ReadData(read_buffer) == HAL_OK) {
//
//			  char timeStr[32];
//			  sprintf(timeStr, "%02d:%02d:%02d %02d/%02d/%02d",
//					  BCD2DEC(read_buffer[2]),  // Hour
//					  BCD2DEC(read_buffer[1]),  // Minute
//					  BCD2DEC(read_buffer[0]),  // Second
//					  BCD2DEC(read_buffer[4]),  // Day
//					  BCD2DEC(read_buffer[5]),  // Month
//					  BCD2DEC(read_buffer[6])); // Year
//
//			  u8g2_ClearBuffer(&u8g2);
//			  u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
//			  u8g2_DrawStr(&u8g2, 20, 30, timeStr);
//			  u8g2_SendBuffer(&u8g2);
//		  }

//	  mode = DIP_GetMode();
//	  id = DIP_GetID();
//
//	  do {
//
//		  char timeStr[16];
//		  char dateStr[16];
//		  sprintf(timeStr, "%02d:%02d:%02d", time.hour, time.minute, time.second);
//	      sprintf(dateStr, "%02d/%02d/%02d", time.day, time.month, time.year);
//
//	      u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
//		  u8g2_DrawStr(&u8g2, 85, 62, timeStr);
//		  u8g2_DrawStr(&u8g2, 0, 62, dateStr);
//
//		  	  char modeStr[16];
//	          char idStr[16];
//	          sprintf(modeStr, "MODE: %d", mode);
//	          sprintf(idStr, "ID: %d", id);
//
//	          u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
//	          u8g2_DrawStr(&u8g2, 5, 8, modeStr);
//	          u8g2_DrawStr(&u8g2, 80, 8, idStr);
//
////	          uint32_t used_space = EEPROM_GetUsedSpace();
////	          float usage_percent = ((float)used_space / EEPROM_SIZE) * 100;
////
////	          char usageStr[32];
////	          sprintf(usageStr, "Memory: %luB(%0.1f%%)", used_space, usage_percent);
////
////	          u8g2_DrawStr(&u8g2, 0, 50, usageStr);
//
//	          HAL_Delay(10);
//
//	      } while (u8g2_NextPage(&u8g2));

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */



//	  if (count > 10){
//	  		    TxData[0]= 4;
//	  		    TxData[1]= 14;
//	  		    TxData[2]= 24;
//	  		    TxData[3]= 34;
//	  		    TxData[4]= 44;
//	  		    TxData[5]= 54;
//	  		    TxData[6]= 64;
//	  		    TxData[7]= 104;
//	  	  } else if (count <= 10){
//	  		    TxData[0]= 5;
//	  		    TxData[1]= 15;
//	  		    TxData[2]= 25;
//	  		    TxData[3]= 35;
//	  		    TxData[4]= 45;
//	  		    TxData[5]= 55;
//	  		    TxData[6]= 65;
//	  		    TxData[7]= 105;
//	  	  }
//	  	  count++;
//	  	      HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
//	  	      HAL_GPIO_TogglePin(LED_07_GPIO_Port, LED_07_Pin);
//	  	      HAL_Delay(1000);

//	  if (datacheck)
//	  {
//		  for (int i=0; i<RxCAN[1]; i++)
//		  {
//			  HAL_GPIO_TogglePin(GPIOB, LED_01_Pin);
//			  HAL_Delay(RxCAN[0]);
//		  }
//		  datacheck = 0;
//
//		  TxCAN[0] = 200;
//		  TxCAN[1] = 40;
//
//		  HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxCAN, &TxMailbox);
//	  }

//	  sprintf(TxData, "1,2,3,4,5,", indx++);
//	  sendData(TxData);
//	  HAL_Delay(1000);

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

void SD_CardHandler(void)
{
    static uint8_t last_state = 0xFF;  // Invalid initial state
    static uint32_t last_check = 0;
    uint32_t current_time = HAL_GetTick();

    // Only check periodically
    if(current_time - last_check >= SD_CHECK_INTERVAL) {
        uint8_t current_state = SD_CARD_PRESENT();

        if(current_state != last_state) {
            HAL_Delay(50);  // Debounce
            current_state = SD_CARD_PRESENT();  // Check again after debounce

            if(current_state) {
                LED_State(1, ON);
                fresult = f_mount(&fs, "/", 1);
                if(fresult == FR_OK) {
                    f_getfree("", &fre_clust, &pfs);
                    total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
                    free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
                    uint32_t used_space = total - free_space;
                    float usage_percent = ((float)used_space / total) * 100;
                    sprintf(buffer, "SD In: %.1f%% Used", usage_percent);
                } else {
                    sprintf(buffer, "Mount Error!");
                    LED_State(1, OFF);
                }
            } else {
                LED_State(1, OFF);
                f_mount(NULL, "/", 0);
                sprintf(buffer, "SD Removed!");
            }

            display_lcd(buffer);
            last_state = current_state;
        }

        last_check = current_time;
    }
}

void display_lcd(const char *message)
{
	u8g2_FirstPage(&u8g2);
	    do {
	    	u8g2_ClearBuffer(&u8g2);
			u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
			u8g2_DrawStr(&u8g2, 0, 30, message);
			u8g2_SendBuffer(&u8g2);
	    } while (u8g2_NextPage(&u8g2));
}

void display_popup(const char* message) {
    u8g2_ClearBuffer(&u8g2);

    // Draw popup box
    u8g2_DrawFrame(&u8g2, 10, 10, 108, 44);  // Border
    u8g2_DrawBox(&u8g2, 10, 10, 108, 14);    // Header background

    // Draw header text in white
    u8g2_SetDrawColor(&u8g2, 0);  // White text
    u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
    u8g2_DrawStr(&u8g2, 14, 21, "SD Card");

    // Draw message in black
    u8g2_SetDrawColor(&u8g2, 1);  // Black text
    u8g2_DrawStr(&u8g2, 14, 40, message);

    u8g2_SendBuffer(&u8g2);
}

void display_progress_bar(const char *message, float percentage)
{
    u8g2_ClearBuffer(&u8g2);

    // Display message at top
    u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
    u8g2_DrawStr(&u8g2, 10, 20, message);

    // Draw progress bar frame
    u8g2_DrawFrame(&u8g2, 10, 30, 108, 10);

    // Calculate and draw fill bar
    uint8_t fill_width = (uint8_t)((percentage / 100.0f) * 106);
    if (fill_width > 0) {
        u8g2_DrawBox(&u8g2, 11, 31, fill_width, 8);
    }

    // Display percentage text below
    char percent_str[10];
    sprintf(percent_str, "%.1f%%", percentage);
    u8g2_DrawStr(&u8g2, 40, 50, percent_str);

    u8g2_SendBuffer(&u8g2);
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
