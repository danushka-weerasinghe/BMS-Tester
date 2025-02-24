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
#define SD_CS_Pin GPIO_PIN_3
#define SD_CS_GPIO_Port GPIOD
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
