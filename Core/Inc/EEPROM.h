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
