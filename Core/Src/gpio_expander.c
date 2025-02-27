/*
 * gpio_expander.c
 *
 *  Created on: Feb 27, 2025
 *      Author: Danushka Weerasinghe
 */


#include "gpio_expander.h"
#include <stdio.h>  /* For optional printf logging */

/* Register addresses per NCA9535 datasheet */
#define PCA9535_REG_OUTPUT_PORT0   0x02
#define PCA9535_REG_OUTPUT_PORT1   0x03
#define PCA9535_REG_CONFIG_PORT0   0x06
#define PCA9535_REG_CONFIG_PORT1   0x07

/* List of known expander addresses */
static const uint8_t expanderAddresses[] = { 0x20, 0x21, 0x24, 0x25 };
static const size_t expanderCount = sizeof(expanderAddresses) / sizeof(expanderAddresses[0]);

/* Local helper functions */
static HAL_StatusTypeDef Expander_SetConfigOutputs(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress);
static HAL_StatusTypeDef Expander_SetOutputs(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress, uint8_t out0, uint8_t out1);

/* Initializes a single expander by configuring all its 16 I/Os as outputs */
HAL_StatusTypeDef Expander_InitDevice(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress)
{
    HAL_StatusTypeDef status;
    status = Expander_SetConfigOutputs(hi2c, deviceAddress);
    if (status != HAL_OK)
    {
        printf("Expander 0x%02X: Config failed (status: %d)\n", deviceAddress, status);
        return status;
    }
    else
    {
        printf("Expander 0x%02X: Configured as outputs\n", deviceAddress);
    }
    return status;
}

/* Initializes all known expanders on the given I2C bus */
HAL_StatusTypeDef Expander_InitAllDevices(I2C_HandleTypeDef *hi2c)
{
    HAL_StatusTypeDef status = HAL_OK;
    size_t i;
    for (i = 0; i < expanderCount; i++)
    {
        uint8_t addr = expanderAddresses[i];
        status = Expander_InitDevice(hi2c, addr);
        if (status != HAL_OK)
        {
            return status;
        }
    }
    return HAL_OK;
}

/* Sets all outputs to HIGH for the specified expander */
HAL_StatusTypeDef Expander_SetAllOutputsHigh(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress)
{
    return Expander_SetOutputs(hi2c, deviceAddress, 0xFF, 0xFF);
}

/* Sets all outputs to LOW for the specified expander */
HAL_StatusTypeDef Expander_SetAllOutputsLow(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress)
{
    return Expander_SetOutputs(hi2c, deviceAddress, 0x00, 0x00);
}

/*---------------------------------------------------------------------------
 * Local helper functions
 *--------------------------------------------------------------------------*/

/* Configures both configuration registers to set all 16 I/Os as outputs */
static HAL_StatusTypeDef Expander_SetConfigOutputs(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress)
{
    uint8_t configValue = 0x00;  // 0 => output mode
    HAL_StatusTypeDef status;

    /* Configure Port 0 */
    status = HAL_I2C_Mem_Write(hi2c,
                               (uint16_t)(deviceAddress << 1),
                               PCA9535_REG_CONFIG_PORT0,
                               I2C_MEMADD_SIZE_8BIT,
                               &configValue,
                               1,
                               100);
    if (status != HAL_OK)
    {
        return status;
    }

    /* Configure Port 1 */
    status = HAL_I2C_Mem_Write(hi2c,
                               (uint16_t)(deviceAddress << 1),
                               PCA9535_REG_CONFIG_PORT1,
                               I2C_MEMADD_SIZE_8BIT,
                               &configValue,
                               1,
                               100);
    return status;
}

/* Writes output values to registers 0x02 and 0x03 for the specified expander */
static HAL_StatusTypeDef Expander_SetOutputs(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress, uint8_t out0, uint8_t out1)
{
    HAL_StatusTypeDef status;
    /* Write to Output Port 0 */
    status = HAL_I2C_Mem_Write(hi2c,
                               (uint16_t)(deviceAddress << 1),
                               PCA9535_REG_OUTPUT_PORT0,
                               I2C_MEMADD_SIZE_8BIT,
                               &out0,
                               1,
                               100);
    if (status != HAL_OK)
    {
        printf("Expander 0x%02X: Failed to set Port 0 (status: %d)\n", deviceAddress, status);
        return status;
    }
    /* Write to Output Port 1 */
    status = HAL_I2C_Mem_Write(hi2c,
                               (uint16_t)(deviceAddress << 1),
                               PCA9535_REG_OUTPUT_PORT1,
                               I2C_MEMADD_SIZE_8BIT,
                               &out1,
                               1,
                               100);
    if (status == HAL_OK)
    {
        printf("Expander 0x%02X: Outputs set to 0x%02X, 0x%02X\n", deviceAddress, out0, out1);
    }
    else
    {
        printf("Expander 0x%02X: Failed to set Port 1 (status: %d)\n", deviceAddress, status);
    }
    return status;
}
