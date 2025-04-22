/*
 * EEPROM.c
 *
 *  Created on: Mar 3, 2025
 *      Author: Pavan Kavinda
 */

#include "EEPROM.h"
#include <string.h>

/* Private variables */
static uint16_t current_address = 0x0000;
extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef EEPROM_Init(void)
{
    current_address = 0;
    return HAL_OK;
}

HAL_StatusTypeDef EEPROM_WriteData(uint8_t *data)
{
    uint8_t buffer[BLOCK_SIZE + 2];
    buffer[0] = (uint8_t)((current_address >> 8) & 0xFF);
    buffer[1] = (uint8_t)(current_address & 0xFF);
    memcpy(&buffer[2], data, BLOCK_SIZE);

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, EEPROM_ADDR,
                                                      buffer, BLOCK_SIZE + 2, HAL_MAX_DELAY);

    if (status == HAL_OK) {
        current_address += BLOCK_SIZE;
        if (current_address >= EEPROM_SIZE)
            current_address = 0;
        HAL_Delay(5); // Write cycle delay
    }

    return status;
}

HAL_StatusTypeDef EEPROM_ReadData(uint8_t *buffer)
{
    uint8_t addr_buffer[2];
    addr_buffer[0] = (uint8_t)((current_address >> 8) & 0xFF);
    addr_buffer[1] = (uint8_t)(current_address & 0xFF);

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, EEPROM_ADDR,
                                                      addr_buffer, 2, HAL_MAX_DELAY);
    if (status != HAL_OK)
        return status;

    return HAL_I2C_Master_Receive(&hi2c1, EEPROM_ADDR, buffer, BLOCK_SIZE, HAL_MAX_DELAY);
}

uint16_t EEPROM_GetCurrentAddr(void)
{
    return current_address;
}

uint32_t EEPROM_GetUsedSpace(void)
{
    uint32_t used_blocks;

    // Calculate used blocks based on current address
    if (current_address == 0) {
        // If we're at the start, either EEPROM is empty or just wrapped around
        uint8_t check_buffer[BLOCK_SIZE];
        if (EEPROM_ReadData(check_buffer) == HAL_OK) {
            // Check if first block contains data
            if (check_buffer[0] != EMPTY_BYTE) {
                used_blocks = EEPROM_SIZE / BLOCK_SIZE; // EEPROM is full
            } else {
                used_blocks = 0; // EEPROM is empty
            }
        }
    } else {
        // Current address indicates number of used blocks
        used_blocks = current_address / BLOCK_SIZE;
    }

    return used_blocks * BLOCK_SIZE; // Return used space in bytes
}
