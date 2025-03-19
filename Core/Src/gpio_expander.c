/*
 * gpio_expander.c
 *
 *  Created on: Feb 27, 2025
 *      Author: Danushka Weerasinghe
 *  Modified for improved modularity and API clarity.
 */

#include "gpio_expander.h"
#include <stdio.h>  /* For optional printf logging */




/**
 * @brief Initializes the pin directions on the expander using the defined direction macros.
 *
 * For Port 0, each bit is set to 1 if the corresponding pin is configured as an input,
 * and 0 if it is an output. In this example, Port 1 is assumed to be all outputs (0x00).
 *
 * @param hi2c          Pointer to the I2C handle.
 * @param deviceAddress I2C address of the expander.
 * @return HAL_StatusTypeDef HAL_OK if successful, or an error code.
 */
HAL_StatusTypeDef Expander_InitPinDirections(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress)
{
    HAL_StatusTypeDef status;
    uint8_t port0_config = 0;

    // Build Port 0 configuration mask using the defined direction macros:
    port0_config |= (ALERT_PIN_CELL_01_DIR == PIN_INPUT) ? ALERT_PIN_CELL_01 : 0;
    port0_config |= (CELL_01_VOLTAGE_01_DIR == PIN_INPUT) ? CELL_01_VOLTAGE_01 : 0;
    port0_config |= (CELL_01_VOLTAGE_02_DIR == PIN_INPUT) ? CELL_01_VOLTAGE_02 : 0;
    port0_config |= (CELL_01_VOLTAGE_03_DIR == PIN_INPUT) ? CELL_01_VOLTAGE_03 : 0;
    port0_config |= (CELL_01_LED_01_DIR       == PIN_INPUT) ? CELL_01_LED_01       : 0;
    port0_config |= (CELL_02_VOLTAGE_03_DIR == PIN_INPUT) ? CELL_02_VOLTAGE_03 : 0;
    port0_config |= (CELL_02_VOLTAGE_02_DIR == PIN_INPUT) ? CELL_02_VOLTAGE_02 : 0;
    port0_config |= (CELL_02_VOLTAGE_01_DIR == PIN_INPUT) ? CELL_02_VOLTAGE_01 : 0;

    // For Port 1, use the corresponding direction macros if needed.
    // In this example, we build Port 1 configuration mask similarly:
    uint8_t port1_config = 0;
    port1_config |= (ALERT_PIN_CELL_02_DIR   == PIN_INPUT) ? (1U << 0) : 0;
    port1_config |= (CELL_02_LED_01_DIR      == PIN_INPUT) ? (1U << 1) : 0;
    port1_config |= (CELL_03_LED_01_DIR      == PIN_INPUT) ? (1U << 2) : 0;
    port1_config |= (EXPANDER_FAN_CTRL_DIR   == PIN_INPUT) ? (1U << 3) : 0;
    port1_config |= (CELL_03_VOLTAGE_03_DIR  == PIN_INPUT) ? (1U << 4) : 0;
    port1_config |= (CELL_03_VOLTAGE_02_DIR  == PIN_INPUT) ? (1U << 5) : 0;
    port1_config |= (CELL_03_VOLTAGE_01_DIR  == PIN_INPUT) ? (1U << 6) : 0;
    port1_config |= (ALERT_PIN_CELL_03_DIR   == PIN_INPUT) ? (1U << 7) : 0;

    // Write configuration to Port 0 register.
    status = HAL_I2C_Mem_Write(hi2c,
                               (uint16_t)(deviceAddress << 1),
                               PCA9535_REG_CONFIG_PORT0,
                               I2C_MEMADD_SIZE_8BIT,
                               &port0_config,
                               1,
                               EXPANDER_I2C_TIMEOUT);
    if (status != HAL_OK)
    {
        return status;
    }

    // Write configuration to Port 1 register.
    status = HAL_I2C_Mem_Write(hi2c,
                               (uint16_t)(deviceAddress << 1),
                               PCA9535_REG_CONFIG_PORT1,
                               I2C_MEMADD_SIZE_8BIT,
                               &port1_config,
                               1,
                               EXPANDER_I2C_TIMEOUT);
    return status;
}

/**
 * @brief Sets the output state (high or low) of a single pin.
 *
 * This function assumes that the pin is already configured as an output.
 *
 * @param hi2c          Pointer to the I2C handle.
 * @param deviceAddress I2C address of the expander.
 * @param pin           16-bit mask representing the pin.
 * @param state         HIGH or LOW.
 * @return HAL_StatusTypeDef HAL_OK if successful, or an error code.
 */
HAL_StatusTypeDef Expander_SetPinState(I2C_HandleTypeDef *hi2c,
                                       uint8_t deviceAddress,
                                       uint16_t pin,
                                       uint8_t state)
{
    HAL_StatusTypeDef status;
    uint8_t regAddress;
    uint8_t currentOutput;

    // Determine which port the pin belongs to.
    if ((pin & 0xFF00) == 0)
    {
        // Pin belongs to Port 0.
        regAddress = PCA9535_REG_OUTPUT_PORT0;
        status = HAL_I2C_Mem_Read(hi2c,
                                  (uint16_t)(deviceAddress << 1),
                                  regAddress,
                                  I2C_MEMADD_SIZE_8BIT,
                                  &currentOutput,
                                  1,
                                  EXPANDER_I2C_TIMEOUT);
        if (status != HAL_OK) {
            return status;
        }
        if (state == HIGH) {
            currentOutput |= (uint8_t)pin;
        } else {
            currentOutput &= ~(uint8_t)pin;
        }
        status = HAL_I2C_Mem_Write(hi2c,
                                   (uint16_t)(deviceAddress << 1),
                                   regAddress,
                                   I2C_MEMADD_SIZE_8BIT,
                                   &currentOutput,
                                   1,
                                   EXPANDER_I2C_TIMEOUT);
        return status;
    }
    else if ((pin & 0x00FF) == 0)
    {
        // Pin belongs to Port 1.
        regAddress = PCA9535_REG_OUTPUT_PORT1;
        status = HAL_I2C_Mem_Read(hi2c,
                                  (uint16_t)(deviceAddress << 1),
                                  regAddress,
                                  I2C_MEMADD_SIZE_8BIT,
                                  &currentOutput,
                                  1,
                                  EXPANDER_I2C_TIMEOUT);
        if (status != HAL_OK) {
            return status;
        }
        uint8_t pin_mask = (uint8_t)(pin >> 8);
        if (state == HIGH) {
            currentOutput |= pin_mask;
        } else {
            currentOutput &= ~pin_mask;
        }
        status = HAL_I2C_Mem_Write(hi2c,
                                   (uint16_t)(deviceAddress << 1),
                                   regAddress,
                                   I2C_MEMADD_SIZE_8BIT,
                                   &currentOutput,
                                   1,
                                   EXPANDER_I2C_TIMEOUT);
        return status;
    }
    else {
        // Invalid pin mask.
        return HAL_ERROR;
    }
}

/**
 * @brief Initializes all known expander devices on the given I2C bus.
 *
 * This function calls Expander_InitPinDirections for each known expander address.
 *
 * @param hi2c Pointer to the I2C handle.
 * @return HAL_StatusTypeDef HAL_OK if all devices are initialized successfully, or an error code.
 */
HAL_StatusTypeDef Expander_InitAllDevices(I2C_HandleTypeDef *hi2c)
{
    HAL_StatusTypeDef status;

    // List of expander addresses.
    uint8_t addresses[] = {
        GPIO_EXPANDER_ID_01,
        GPIO_EXPANDER_ID_02,
        GPIO_EXPANDER_ID_03,
        GPIO_EXPANDER_ID_04
    };

    size_t count = sizeof(addresses) / sizeof(addresses[0]);
    for (size_t i = 0; i < count; i++)
    {
        status = Expander_InitPinDirections(hi2c, addresses[i]);
        if (status != HAL_OK)
        {
            return status;
        }
    }
    return HAL_OK;
}

