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
#include "fatfs_sd.h"
#include "LED_BuiltIn.h"
#include "Switches.h"
#include "MOD_BUS.h"
#include "CAN_BUS.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SD_CARD_PRESENT()  (HAL_GPIO_ReadPin(CD_GPIO_Port, CD_Pin) == GPIO_PIN_RESET)
//#define POPUP_TIMEOUT    1000


#define SD_CHECK_INTERVAL   500   // Check every 500ms
#define CD_INSERTED        GPIO_PIN_RESET
#define CD_REMOVED         GPIO_PIN_SET

//#define SD_CHECK_INTERVAL   500   // Check every 500ms
//#define CD_INSERTED        GPIO_PIN_SET
//#define CD_REMOVED         GPIO_PIN_RESET

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

volatile uint8_t sd_card_present = 0;
//volatile uint8_t sd_card_last_state = 0;
//volatile uint32_t sd_last_debounce_time = 0;
//volatile uint32_t sd_last_check_time = 0;
//volatile uint8_t sd_state_changed = 0;

extern u8g2_t u8g2;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void display_lcd(const char *message);
void display_progress_bar(const char *message, float percentage);
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

  u8g2_ClearDisplay(&u8g2);
  u8g2_SetContrast(&u8g2, 100);

//  LED_State(1, ON);

  RTC_Init(&hi2c1);
  RTC_TimeLapseInit();

  /*

  sd_card_present = (HAL_GPIO_ReadPin(CD_GPIO_Port, CD_Pin) == GPIO_PIN_RESET);
  if(sd_card_present) {
      fresult = f_mount(&fs, "/", 1);
      if(fresult == FR_OK) {
          f_getfree("", &fre_clust, &pfs);
          total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
          free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
          sprintf(buffer, "SD Card OK!");
          LED_State(1, ON);
      } else {
          sprintf(buffer, "Mount Error!");
          LED_State(2, ON);
      }
  } else {
      sprintf(buffer, "No SD Card!");
      LED_State(3, ON);
  }
  */

//  display_lcd(message);

/////  RTC_SetTime(00,29,12,5,03,03,25);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  RTC_ReadTime();

//	  SD_CardHandler();

	  /*
	    static uint32_t lastCheckTime = 0;
	    uint32_t currentTime = HAL_GetTick();

	    if(currentTime - lastCheckTime >= 1000) {  // Check every second
	        SD_CardHandler();
	        display_lcd(buffer);
	        lastCheckTime = currentTime;
	    }
	   */

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
	do{
		u8g2_ClearBuffer(&u8g2);
		u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
		u8g2_DrawStr(&u8g2, 0, 30, message);
		u8g2_SendBuffer(&u8g2);
	} while (u8g2_NextPage(&u8g2));
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
