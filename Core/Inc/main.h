/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CELL12_CS_01_Pin GPIO_PIN_2
#define CELL12_CS_01_GPIO_Port GPIOE
#define CELL12_CS_02_Pin GPIO_PIN_3
#define CELL12_CS_02_GPIO_Port GPIOE
#define CELL12_CS_03_Pin GPIO_PIN_4
#define CELL12_CS_03_GPIO_Port GPIOE
#define CELL12_CS_04_Pin GPIO_PIN_5
#define CELL12_CS_04_GPIO_Port GPIOE
#define CELL12_CS_05_Pin GPIO_PIN_6
#define CELL12_CS_05_GPIO_Port GPIOE
#define CELL12_CS_06_Pin GPIO_PIN_8
#define CELL12_CS_06_GPIO_Port GPIOI
#define CELL12_CS_07_Pin GPIO_PIN_13
#define CELL12_CS_07_GPIO_Port GPIOC
#define CELL12_CS_08_Pin GPIO_PIN_9
#define CELL12_CS_08_GPIO_Port GPIOI
#define CELL12_CS_10_Pin GPIO_PIN_10
#define CELL12_CS_10_GPIO_Port GPIOI
#define CELL12_CS_09_Pin GPIO_PIN_11
#define CELL12_CS_09_GPIO_Port GPIOI
#define CELL12_CS_11_Pin GPIO_PIN_0
#define CELL12_CS_11_GPIO_Port GPIOF
#define CELL12_CS_12_Pin GPIO_PIN_1
#define CELL12_CS_12_GPIO_Port GPIOF
#define GPIO_01_Pin GPIO_PIN_2
#define GPIO_01_GPIO_Port GPIOF
#define CELL12_TEMP_03_CS_Pin GPIO_PIN_6
#define CELL12_TEMP_03_CS_GPIO_Port GPIOF
#define CELL12_TEMP_03_LED_Pin GPIO_PIN_8
#define CELL12_TEMP_03_LED_GPIO_Port GPIOF
#define CELL12_TEMP_02_CS_Pin GPIO_PIN_10
#define CELL12_TEMP_02_CS_GPIO_Port GPIOF
#define CELL12_TEMP_02_LED_Pin GPIO_PIN_0
#define CELL12_TEMP_02_LED_GPIO_Port GPIOC
#define CELL12_TEMP_01_CS_Pin GPIO_PIN_1
#define CELL12_TEMP_01_CS_GPIO_Port GPIOC
#define CELL12_TEMP_01_LED_Pin GPIO_PIN_0
#define CELL12_TEMP_01_LED_GPIO_Port GPIOA
#define CELL11_TEMP_03_CS_Pin GPIO_PIN_11
#define CELL11_TEMP_03_CS_GPIO_Port GPIOB
#define CELL11_TEMP_03_LED_Pin GPIO_PIN_6
#define CELL11_TEMP_03_LED_GPIO_Port GPIOH
#define CELL11_TEMP_02_CS_Pin GPIO_PIN_9
#define CELL11_TEMP_02_CS_GPIO_Port GPIOH
#define CELL11_TEMP_02_LED_Pin GPIO_PIN_10
#define CELL11_TEMP_02_LED_GPIO_Port GPIOH
#define CELL11_TEMP_01_CS_Pin GPIO_PIN_11
#define CELL11_TEMP_01_CS_GPIO_Port GPIOH
#define CELL11_TEMP_01_LED_Pin GPIO_PIN_12
#define CELL11_TEMP_01_LED_GPIO_Port GPIOH
#define DISPLAY_IO_1_Pin GPIO_PIN_2
#define DISPLAY_IO_1_GPIO_Port GPIOG
#define DISPLAY_IO_2_Pin GPIO_PIN_3
#define DISPLAY_IO_2_GPIO_Port GPIOG
#define BACKLIGHT_1_Pin GPIO_PIN_4
#define BACKLIGHT_1_GPIO_Port GPIOG
#define BACKLIGHT_2_Pin GPIO_PIN_5
#define BACKLIGHT_2_GPIO_Port GPIOG
#define LED_01_Pin GPIO_PIN_14
#define LED_01_GPIO_Port GPIOH
#define LED_02_Pin GPIO_PIN_15
#define LED_02_GPIO_Port GPIOH
#define DISPLAY_CS_Pin GPIO_PIN_2
#define DISPLAY_CS_GPIO_Port GPIOD
#define DISPLAY_CSD3_Pin GPIO_PIN_3
#define DISPLAY_CSD3_GPIO_Port GPIOD
#define LED_09_Pin GPIO_PIN_13
#define LED_09_GPIO_Port GPIOG
#define LED_04_Pin GPIO_PIN_14
#define LED_04_GPIO_Port GPIOG
#define LED_08_Pin GPIO_PIN_15
#define LED_08_GPIO_Port GPIOG
#define LED_03_Pin GPIO_PIN_3
#define LED_03_GPIO_Port GPIOB
#define LED_07_Pin GPIO_PIN_4
#define LED_07_GPIO_Port GPIOB
#define LED_06_Pin GPIO_PIN_0
#define LED_06_GPIO_Port GPIOE
#define LED_05_Pin GPIO_PIN_1
#define LED_05_GPIO_Port GPIOE
#define LED_DC_Y_Pin GPIO_PIN_4
#define LED_DC_Y_GPIO_Port GPIOI
#define LED_DC_G_Pin GPIO_PIN_5
#define LED_DC_G_GPIO_Port GPIOI
#define LED_PC_Y_Pin GPIO_PIN_6
#define LED_PC_Y_GPIO_Port GPIOI
#define LED_PC_G_Pin GPIO_PIN_7
#define LED_PC_G_GPIO_Port GPIOI

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
