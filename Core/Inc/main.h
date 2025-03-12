/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#define USART2_ENABLE_Pin GPIO_PIN_1
#define USART2_ENABLE_GPIO_Port GPIOA
#define Temp_LED_Pin GPIO_PIN_6
#define Temp_LED_GPIO_Port GPIOH
#define CS_ISO_01_Pin GPIO_PIN_15
#define CS_ISO_01_GPIO_Port GPIOB
#define CS_ISO_02_Pin GPIO_PIN_10
#define CS_ISO_02_GPIO_Port GPIOD
#define USART3_ENABLE_Pin GPIO_PIN_11
#define USART3_ENABLE_GPIO_Port GPIOD
#define PUSH_BUTTON_01_Pin GPIO_PIN_12
#define PUSH_BUTTON_01_GPIO_Port GPIOD
#define PUSH_BUTTON_01_EXTI_IRQn EXTI15_10_IRQn
#define PUSH_BUTTON_02_Pin GPIO_PIN_13
#define PUSH_BUTTON_02_GPIO_Port GPIOD
#define PUSH_BUTTON_02_EXTI_IRQn EXTI15_10_IRQn
#define PUSH_BUTTON_03_Pin GPIO_PIN_14
#define PUSH_BUTTON_03_GPIO_Port GPIOD
#define PUSH_BUTTON_03_EXTI_IRQn EXTI15_10_IRQn
#define PUSH_BUTTON_04_Pin GPIO_PIN_15
#define PUSH_BUTTON_04_GPIO_Port GPIOD
#define PUSH_BUTTON_04_EXTI_IRQn EXTI15_10_IRQn
#define DISPLAY_IO_1_Pin GPIO_PIN_2
#define DISPLAY_IO_1_GPIO_Port GPIOG
#define DISPLAY_IO_2_Pin GPIO_PIN_3
#define DISPLAY_IO_2_GPIO_Port GPIOG
#define BACKLIGHT_1_Pin GPIO_PIN_4
#define BACKLIGHT_1_GPIO_Port GPIOG
#define BACKLIGHT_2_Pin GPIO_PIN_5
#define BACKLIGHT_2_GPIO_Port GPIOG
#define USART6_ENABLE_Pin GPIO_PIN_8
#define USART6_ENABLE_GPIO_Port GPIOC
#define CD_Pin GPIO_PIN_13
#define CD_GPIO_Port GPIOH
#define LED_09_Pin GPIO_PIN_14
#define LED_09_GPIO_Port GPIOH
#define LED_08_Pin GPIO_PIN_15
#define LED_08_GPIO_Port GPIOH
#define DISPLAY_CS_Pin GPIO_PIN_2
#define DISPLAY_CS_GPIO_Port GPIOD
#define SPI5_CS_02_Pin GPIO_PIN_3
#define SPI5_CS_02_GPIO_Port GPIOD
#define DIP_SWITCH_01_Pin GPIO_PIN_4
#define DIP_SWITCH_01_GPIO_Port GPIOD
#define DIP_SWITCH_02_Pin GPIO_PIN_5
#define DIP_SWITCH_02_GPIO_Port GPIOD
#define DIP_SWITCH_03_Pin GPIO_PIN_6
#define DIP_SWITCH_03_GPIO_Port GPIOD
#define DIP_SWITCH_04_Pin GPIO_PIN_7
#define DIP_SWITCH_04_GPIO_Port GPIOD
#define DIP_SWITCH_MODE_01_Pin GPIO_PIN_9
#define DIP_SWITCH_MODE_01_GPIO_Port GPIOG
#define DIP_SWITCH_MODE_02_Pin GPIO_PIN_10
#define DIP_SWITCH_MODE_02_GPIO_Port GPIOG
#define DIP_SWITCH_MODE_03_Pin GPIO_PIN_11
#define DIP_SWITCH_MODE_03_GPIO_Port GPIOG
#define DIP_SWITCH_MODE_04_Pin GPIO_PIN_12
#define DIP_SWITCH_MODE_04_GPIO_Port GPIOG
#define LED_07_Pin GPIO_PIN_13
#define LED_07_GPIO_Port GPIOG
#define LED_02_Pin GPIO_PIN_14
#define LED_02_GPIO_Port GPIOG
#define LED_06_Pin GPIO_PIN_15
#define LED_06_GPIO_Port GPIOG
#define LED_01_Pin GPIO_PIN_3
#define LED_01_GPIO_Port GPIOB
#define LED_05_Pin GPIO_PIN_4
#define LED_05_GPIO_Port GPIOB
#define USART1_ENABLE_Pin GPIO_PIN_5
#define USART1_ENABLE_GPIO_Port GPIOB
#define LED_04_Pin GPIO_PIN_0
#define LED_04_GPIO_Port GPIOE
#define LED_03_Pin GPIO_PIN_1
#define LED_03_GPIO_Port GPIOE
#define RS485_YELLOW_LED_02_Pin GPIO_PIN_4
#define RS485_YELLOW_LED_02_GPIO_Port GPIOI
#define RS485_GREEN_LED_02_Pin GPIO_PIN_5
#define RS485_GREEN_LED_02_GPIO_Port GPIOI
#define RS485_YELLOW_LED_01_Pin GPIO_PIN_6
#define RS485_YELLOW_LED_01_GPIO_Port GPIOI
#define RS485_GREEN_LED_01_Pin GPIO_PIN_7
#define RS485_GREEN_LED_01_GPIO_Port GPIOI

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
