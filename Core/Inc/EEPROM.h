/*
 * EEPROM.h
 *
 *  Created on: Mar 3, 2025
 *      Author: Pavan Kavinda
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

/* EEPROM Configuration */
#define EEPROM_ADDR      0xA8    // M24M02 7-bit address
#define EEPROM_SIZE      0x7FFF  // 256 KB max address
#define PAGE_SIZE        256     // Page size
#define BLOCK_SIZE       8       // Data block size
#define EMPTY_BYTE      0xFF    // Value of empty memory

/* Function Prototypes */
HAL_StatusTypeDef EEPROM_Init(void);
HAL_StatusTypeDef EEPROM_WriteData(uint8_t *data);
HAL_StatusTypeDef EEPROM_ReadData(uint8_t *buffer);
uint16_t EEPROM_GetCurrentAddr(void);
uint32_t EEPROM_GetUsedSpace(void);

#endif /* INC_EEPROM_H_ */

/* Example use */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/*
CAN_RxHeaderTypeDef RxHeader;
uint8_t receivedData[8];
CAN_HandleTypeDef *hcan;
uint8_t message[8] = {0xA1, 0xB2, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
*/
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
/*
EEPROM_Init();
*/
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
/*
while (1)
{
uint32_t used_space = EEPROM_GetUsedSpace();
float usage_percent = ((float)used_space / EEPROM_SIZE) * 100;

char usageStr[32];
sprintf(usageStr, "Memory: %luB(%0.1f%%)", used_space, usage_percent);

u8g2_DrawStr(&u8g2, 0, 50, usageStr);
*/
/* USER CODE END WHILE */
