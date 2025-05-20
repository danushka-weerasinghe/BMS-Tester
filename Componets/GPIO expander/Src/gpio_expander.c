/*
 * gpio_expander.c
 *
 *  Created on: Feb 27, 2025
 *      Author: Danushka Weerasinghe
 *  Modified for improved modularity and API clarity.
 */

#include "gpio_expander.h"
#include <stdio.h>  /* For optional printf logging */



// Structure for mapping a voltage (float) to a MUX channel (0-7)
typedef struct {
    float voltage;
    uint8_t mux_channel;
} VoltageMap;

// Define the voltage mapping for all cells (channels 0-7)
// Mapping: Channel 0 = 2.0V, 1 = 2.5V, 2 = 2.8V, 3 = 3.3V, 4 = 3.4V, 5 = 3.6V, 6 = 4.0V, 7 = 4.2V.
static VoltageMap default_map[] = {
    {2.0f, 0},
    {2.5f, 1},
    {2.8f, 2},
    {3.3f, 3},
    {3.4f, 4},
    {3.6f, 5},
    {4.0f, 6},
    {4.2f, 7}
};

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
    port0_config |= (ALERT_PIN_CELL_01_DIR   == PIN_INPUT) ? ALERT_PIN_CELL_01  : 0;
    port0_config |= (CELL_01_VOLTAGE_01_DIR  == PIN_INPUT) ? CELL_01_VOLTAGE_01 : 0;
    port0_config |= (CELL_01_VOLTAGE_02_DIR  == PIN_INPUT) ? CELL_01_VOLTAGE_02 : 0;
    port0_config |= (CELL_01_VOLTAGE_03_DIR  == PIN_INPUT) ? CELL_01_VOLTAGE_03 : 0;
    port0_config |= (CELL_01_LED_01_DIR      == PIN_INPUT) ? CELL_01_LED_01     : 0;
    port0_config |= (CELL_02_VOLTAGE_03_DIR  == PIN_INPUT) ? CELL_02_VOLTAGE_03 : 0;
    port0_config |= (CELL_02_VOLTAGE_02_DIR  == PIN_INPUT) ? CELL_02_VOLTAGE_02 : 0;
    port0_config |= (CELL_02_VOLTAGE_01_DIR  == PIN_INPUT) ? CELL_02_VOLTAGE_01 : 0;

    // For Port 1, use the corresponding direction macros if needed.
    // In this example, we build Port 1 configuration mask similarly:
    uint8_t port1_config = 0;
    port1_config |= (ALERT_PIN_CELL_02_DIR   == PIN_INPUT) ? ALERT_PIN_CELL_02  : 0;
    port1_config |= (CELL_02_LED_01_DIR      == PIN_INPUT) ? CELL_02_LED_01 	: 0;
    port1_config |= (CELL_03_LED_01_DIR      == PIN_INPUT) ? CELL_03_LED_01 	: 0;
    port1_config |= (EXPANDER_FAN_CTRL_DIR   == PIN_INPUT) ? EXPANDER_FAN_CTRL 	: 0;
    port1_config |= (CELL_03_VOLTAGE_03_DIR  == PIN_INPUT) ? CELL_03_VOLTAGE_03	: 0;
    port1_config |= (CELL_03_VOLTAGE_02_DIR  == PIN_INPUT) ? CELL_03_VOLTAGE_02 : 0;
    port1_config |= (CELL_03_VOLTAGE_01_DIR  == PIN_INPUT) ? CELL_03_VOLTAGE_01 : 0;
    port1_config |= (ALERT_PIN_CELL_03_DIR   == PIN_INPUT) ? ALERT_PIN_CELL_03 	: 0;

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


// Helper function to get the expander address and MUX control pin definitions for a given cell.
// Each cell has three control pins (S0, S1, S2) that drive your MUX select lines.
static void get_cell_control_params(CellID cell, uint8_t *expander_Address, uint16_t *v_set_pin_01, uint16_t *v_set_pin_02, uint16_t *v_set_pin_03 ,uint16_t *LED_PIN )
{
    switch(cell)
    {

    // for the cell 01 to 12
        case CELL_1:
            *expander_Address = GPIO_EXPANDER_ID_01;
            *v_set_pin_01 = CELL_01_VOLTAGE_01;
            *v_set_pin_02 = CELL_01_VOLTAGE_02;
            *v_set_pin_03 = CELL_01_VOLTAGE_03;
            *LED_PIN  =     CELL_01_LED_01;
            break;

        case CELL_2:
            *expander_Address = GPIO_EXPANDER_ID_01;
            *v_set_pin_01 = CELL_02_VOLTAGE_01;
            *v_set_pin_02 = CELL_02_VOLTAGE_02;
            *v_set_pin_03 = CELL_02_VOLTAGE_03;
            *LED_PIN  =     CELL_02_LED_01;
            break;

        case CELL_3:
            *expander_Address = GPIO_EXPANDER_ID_01;
            *v_set_pin_01 = CELL_03_VOLTAGE_01;
            *v_set_pin_02 = CELL_03_VOLTAGE_02;
            *v_set_pin_03 = CELL_03_VOLTAGE_03;
            *LED_PIN  =     CELL_03_LED_01 ;
            break;

        case CELL_4:
            *expander_Address = GPIO_EXPANDER_ID_02;
            *v_set_pin_01 = CELL_01_VOLTAGE_01;
            *v_set_pin_02 = CELL_01_VOLTAGE_02;
            *v_set_pin_03 = CELL_01_VOLTAGE_03;
            *LED_PIN  =     CELL_01_LED_01;
            break;

        case CELL_5:
            *expander_Address = GPIO_EXPANDER_ID_02;
            *v_set_pin_01 = CELL_02_VOLTAGE_01;
            *v_set_pin_02 = CELL_02_VOLTAGE_02;
            *v_set_pin_03 = CELL_02_VOLTAGE_03;
            *LED_PIN  =     CELL_02_LED_01;
            break;

        case CELL_6:
            *expander_Address = GPIO_EXPANDER_ID_02;
            *v_set_pin_01 = CELL_03_VOLTAGE_01;
            *v_set_pin_02 = CELL_03_VOLTAGE_02;
            *v_set_pin_03 = CELL_03_VOLTAGE_03;
            *LED_PIN  =     CELL_03_LED_01 ;
            break;

        case CELL_7:
            *expander_Address = GPIO_EXPANDER_ID_03;
            *v_set_pin_01 = CELL_01_VOLTAGE_01;
            *v_set_pin_02 = CELL_01_VOLTAGE_02;
            *v_set_pin_03 = CELL_01_VOLTAGE_03;
            *LED_PIN  =     CELL_01_LED_01;
            break;

        case CELL_8:
            *expander_Address = GPIO_EXPANDER_ID_03;
            *v_set_pin_01 = CELL_02_VOLTAGE_01;
            *v_set_pin_02 = CELL_02_VOLTAGE_02;
            *v_set_pin_03 = CELL_02_VOLTAGE_03;
            *LED_PIN  =     CELL_02_LED_01;
            break;

        case CELL_9:
            *expander_Address = GPIO_EXPANDER_ID_03;
            *v_set_pin_01 = CELL_03_VOLTAGE_01;
            *v_set_pin_02 = CELL_03_VOLTAGE_02;
            *v_set_pin_03 = CELL_03_VOLTAGE_03;
            *LED_PIN  =     CELL_03_LED_01 ;
            break;

        case CELL_10:
            *expander_Address = GPIO_EXPANDER_ID_04;
            *v_set_pin_01 = CELL_01_VOLTAGE_01;
            *v_set_pin_02 = CELL_01_VOLTAGE_02;
            *v_set_pin_03 = CELL_01_VOLTAGE_03;
            *LED_PIN  =     CELL_01_LED_01;
            break;

        case CELL_11:
            *expander_Address = GPIO_EXPANDER_ID_04;
            *v_set_pin_01 = CELL_02_VOLTAGE_01;
            *v_set_pin_02 = CELL_02_VOLTAGE_02;
            *v_set_pin_03 = CELL_02_VOLTAGE_03;
            *LED_PIN  =     CELL_02_LED_01;
            break;

        case CELL_12:
            *expander_Address = GPIO_EXPANDER_ID_04;
            *v_set_pin_01 = CELL_03_VOLTAGE_01;
            *v_set_pin_02 = CELL_03_VOLTAGE_02;
            *v_set_pin_03 = CELL_03_VOLTAGE_03;
            *LED_PIN  =     CELL_03_LED_01 ;
            break;

            // for the cell 12 to 24

        case CELL_13:
            *expander_Address = GPIO_EXPANDER_ID_01;
            *v_set_pin_01 = CELL_01_VOLTAGE_01;
            *v_set_pin_02 = CELL_01_VOLTAGE_02;
            *v_set_pin_03 = CELL_01_VOLTAGE_03;
            *LED_PIN  =     CELL_01_LED_01;

            break;

        case CELL_14:
            *expander_Address = GPIO_EXPANDER_ID_01;
            *v_set_pin_01 = CELL_02_VOLTAGE_01;
            *v_set_pin_02 = CELL_02_VOLTAGE_02;
            *v_set_pin_03 = CELL_02_VOLTAGE_03;
            *LED_PIN  =     CELL_02_LED_01;

            break;

        case CELL_15:
            *expander_Address = GPIO_EXPANDER_ID_01;
            *v_set_pin_01 = CELL_03_VOLTAGE_01;
            *v_set_pin_02 = CELL_03_VOLTAGE_02;
            *v_set_pin_03 = CELL_03_VOLTAGE_03;
            *LED_PIN  =     CELL_03_LED_01 ;
            break;

        case CELL_16:
            *expander_Address = GPIO_EXPANDER_ID_02;
            *v_set_pin_01 = CELL_01_VOLTAGE_01;
            *v_set_pin_02 = CELL_01_VOLTAGE_02;
            *v_set_pin_03 = CELL_01_VOLTAGE_03;
            *LED_PIN  =     CELL_01_LED_01;
            break;

        case CELL_17:
            *expander_Address = GPIO_EXPANDER_ID_02;
            *v_set_pin_01 = CELL_02_VOLTAGE_01;
            *v_set_pin_02 = CELL_02_VOLTAGE_02;
            *v_set_pin_03 = CELL_02_VOLTAGE_03;
            *LED_PIN  =     CELL_02_LED_01;
            break;

        case CELL_18:
            *expander_Address = GPIO_EXPANDER_ID_02;
            *v_set_pin_01 = CELL_03_VOLTAGE_01;
            *v_set_pin_02 = CELL_03_VOLTAGE_02;
            *v_set_pin_03 = CELL_03_VOLTAGE_03;
            *LED_PIN  =     CELL_03_LED_01 ;
            break;

        case CELL_19:
            *expander_Address = GPIO_EXPANDER_ID_03;
            *v_set_pin_01 = CELL_01_VOLTAGE_01;
            *v_set_pin_02 = CELL_01_VOLTAGE_02;
            *v_set_pin_03 = CELL_01_VOLTAGE_03;
            *LED_PIN  =     CELL_01_LED_01;
            break;

        case CELL_20:
            *expander_Address = GPIO_EXPANDER_ID_03;
            *v_set_pin_01 = CELL_02_VOLTAGE_01;
            *v_set_pin_02 = CELL_02_VOLTAGE_02;
            *v_set_pin_03 = CELL_02_VOLTAGE_03;
            *LED_PIN  =     CELL_02_LED_01;
            break;

        case CELL_21:
            *expander_Address = GPIO_EXPANDER_ID_03;
            *v_set_pin_01 = CELL_03_VOLTAGE_01;
            *v_set_pin_02 = CELL_03_VOLTAGE_02;
            *v_set_pin_03 = CELL_03_VOLTAGE_03;
            *LED_PIN  =     CELL_03_LED_01 ;
            break;

        case CELL_22:
            *expander_Address = GPIO_EXPANDER_ID_04;
            *v_set_pin_01 = CELL_01_VOLTAGE_01;
            *v_set_pin_02 = CELL_01_VOLTAGE_02;
            *v_set_pin_03 = CELL_01_VOLTAGE_03;
            *LED_PIN  =     CELL_01_LED_01;
            break;

        case CELL_23:
            *expander_Address = GPIO_EXPANDER_ID_04;
            *v_set_pin_01 = CELL_02_VOLTAGE_01;
            *v_set_pin_02 = CELL_02_VOLTAGE_02;
            *v_set_pin_03 = CELL_02_VOLTAGE_03;
            *LED_PIN  =     CELL_02_LED_01;
            break;

        case CELL_24:
            *expander_Address = GPIO_EXPANDER_ID_04;
            *v_set_pin_01 = CELL_03_VOLTAGE_01;
            *v_set_pin_02 = CELL_03_VOLTAGE_02;
            *v_set_pin_03 = CELL_03_VOLTAGE_03;
            *LED_PIN  =     CELL_03_LED_01 ;
            break;



        default:
            // Default to CELL_1 if an invalid cell is specified
            *expander_Address = GPIO_EXPANDER_ID_01;
            *v_set_pin_01 = CELL_01_VOLTAGE_01;
            *v_set_pin_02 = CELL_01_VOLTAGE_02;
            *v_set_pin_03 = CELL_01_VOLTAGE_03;
            *LED_PIN  =     CELL_01_LED_01;
            break;
    }
}

// Function to set the output voltage for a given cell by selecting the appropriate MUX channel.
// Example usage: Set_Output_Voltage(&hi2c2, CELL_1, 2.5f);
void Set_Output_Voltage( CellID cell, float voltage)
{

    // Get the correct I2C handle based on cell ID
    I2C_HandleTypeDef *target_i2c;
    if (cell <= 11) {
        target_i2c = &hi2c2;
    } else {
        target_i2c = &hi2c3;
    }

    // Look up the MUX channel for the desired voltage.
    uint8_t mux_channel = 0xFF;
    for (int i = 0; i < 8; i++) {
        if (default_map[i].voltage == voltage) {
            mux_channel = default_map[i].mux_channel;
            break;
        }
    }
    if (mux_channel == 0xFF) return; // Voltage not found

    // Convert mux_channel into its 3 select bits.This is use for the Analog mux switch
    uint8_t ANG_MUX_PIN_A = (mux_channel >> 0) & 0x01;
    uint8_t ANG_MUX_PIN_B = (mux_channel >> 1) & 0x01;
    uint8_t ANG_MUX_PIN_C = (mux_channel >> 2) & 0x01;

    // Get the expander address and control pins for this cell.
    uint8_t expander_Address;
    uint16_t v_set_pin_01, v_set_pin_02, v_set_pin_03, LED_PIN ;
    get_cell_control_params(cell, &expander_Address, &v_set_pin_01, &v_set_pin_02, &v_set_pin_03 ,&LED_PIN);

    // Set the multiplexer select lines accordingly.
    Expander_SetPinState(target_i2c, expander_Address, v_set_pin_01, ANG_MUX_PIN_A);
    Expander_SetPinState(target_i2c, expander_Address, v_set_pin_02, ANG_MUX_PIN_B);
    Expander_SetPinState(target_i2c, expander_Address, v_set_pin_03, ANG_MUX_PIN_C);
}

void Set_LED_status ( CellID cell, uint8_t LED_state)

{

    // Get the correct I2C handle based on cell ID
    I2C_HandleTypeDef *target_i2c;
    if (cell <= 11) {
        target_i2c = &hi2c2;
    } else {
        target_i2c = &hi2c3;
    }

    // Get the expander address and control pins for this cell.
    uint8_t expander_Address;
    uint16_t v_set_pin_01, v_set_pin_02, v_set_pin_03, LED_PIN;

    get_cell_control_params(cell, &expander_Address, &v_set_pin_01, &v_set_pin_02, &v_set_pin_03 ,&LED_PIN);

    // Set the multiplexer select lines accordingly.
    Expander_SetPinState(target_i2c, expander_Address, LED_PIN, LED_state);

}

