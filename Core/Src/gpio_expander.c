/*
 * gpio_expander.c
 *
 *  Created on: Feb 27, 2025
 *      Author: Danushka Weerasinghe
 *  Modified for improved modularity and API clarity.
 */

#include "gpio_expander.h"
#include <stdio.h>  /* For optional printf logging */

/* Static list of known expander addresses */
static const uint8_t expanderAddresses[] = {
    GPIO_EXPANDER_ID_01,
    GPIO_EXPANDER_ID_02,
    GPIO_EXPANDER_ID_03,
    GPIO_EXPANDER_ID_04
};
static const size_t expanderCount = sizeof(expanderAddresses) / sizeof(expanderAddresses[0]);

/* Local helper function prototypes */
static HAL_StatusTypeDef Expander_InitDevice(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress);
static HAL_StatusTypeDef Expander_SetConfigOutputs(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress);
static HAL_StatusTypeDef Expander_SetOutputs(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress, uint8_t out0, uint8_t out1);

/* Initializes a single expander by configuring all its 16 I/Os as outputs */
static HAL_StatusTypeDef Expander_InitDevice(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress)
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
    for (size_t i = 0; i < expanderCount; i++)
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
                               EXPANDER_I2C_TIMEOUT);
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
                               EXPANDER_I2C_TIMEOUT);
    return status;
}

/* Writes the new output states to both output registers */
static HAL_StatusTypeDef Expander_SetOutputs(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress, uint8_t out0, uint8_t out1)
{
    HAL_StatusTypeDef status;

    /* Write to Port 0 */
    status = HAL_I2C_Mem_Write(hi2c,
                               (uint16_t)(deviceAddress << 1),
                               PCA9535_REG_OUTPUT_PORT0,
                               I2C_MEMADD_SIZE_8BIT,
                               &out0,
                               1,
                               EXPANDER_I2C_TIMEOUT);
    if (status != HAL_OK)
    {
        return status;
    }

    /* Write to Port 1 */
    status = HAL_I2C_Mem_Write(hi2c,
                               (uint16_t)(deviceAddress << 1),
                               PCA9535_REG_OUTPUT_PORT1,
                               I2C_MEMADD_SIZE_8BIT,
                               &out1,
                               1,
                               EXPANDER_I2C_TIMEOUT);
    return status;
}

/* Sets a specific GPIO pin high or low on the specified expander.
   It is assumed that 'pin' has only one bit set, and that bit is either in the lower
   byte (Port 0) or the upper byte (Port 1). */
HAL_StatusTypeDef Expander_SetPinState(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress, uint16_t pin, uint8_t isHigh)
{
    HAL_StatusTypeDef status;
    uint8_t port0, port1;
    uint8_t currentPort0, currentPort1;

    /* Read current output states */
    status = HAL_I2C_Mem_Read(hi2c,
                              (uint16_t)(deviceAddress << 1),
                              PCA9535_REG_OUTPUT_PORT0,
                              I2C_MEMADD_SIZE_8BIT,
                              &currentPort0,
                              1,
                              EXPANDER_I2C_TIMEOUT);
    if (status != HAL_OK)
    {
        return status;
    }
    status = HAL_I2C_Mem_Read(hi2c,
                              (uint16_t)(deviceAddress << 1),
                              PCA9535_REG_OUTPUT_PORT1,
                              I2C_MEMADD_SIZE_8BIT,
                              &currentPort1,
                              1,
                              EXPANDER_I2C_TIMEOUT);
    if (status != HAL_OK)
    {
        return status;
    }

    /* Update the appropriate port based on the pin mask.
       It is assumed that only one port's bit is set in 'pin'. */
    if ((pin & 0xFF00) == 0) // Pin belongs to Port 0
    {
        if (isHigh)
        {
            port0 = currentPort0 | (uint8_t)pin;
        }
        else
        {
            port0 = currentPort0 & ~(uint8_t)pin;
        }
        port1 = currentPort1; // unchanged
    }
    else if ((pin & 0x00FF) == 0) // Pin belongs to Port 1
    {
        if (isHigh)
        {
            port1 = currentPort1 | (uint8_t)(pin >> 8);
        }
        else
        {
            port1 = currentPort1 & ~(uint8_t)(pin >> 8);
        }
        port0 = currentPort0; // unchanged
    }
    else
    {
        /* Invalid pin mask: bits set for both ports */
        return HAL_ERROR;
    }

    /* Write updated outputs */
    status = Expander_SetOutputs(hi2c, deviceAddress, port0, port1);
    return status;
}


HAL_StatusTypeDef Expander_SingleLEDControl(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress, uint16_t pin, uint8_t isHigh, uint16_t delay)
{
    HAL_StatusTypeDef status;

    status = Expander_SetPinState(hi2c, deviceAddress, pin, isHigh);
    if (status != HAL_OK) {
        printf("Failed to set LED state: device 0x%02X, pin 0x%04X\n", deviceAddress, pin);
        return status;
    }

    if (delay > 0) {
        HAL_Delay(delay);
    }

    return HAL_OK;
}

// Optional: Sequential control of all LEDs
HAL_StatusTypeDef Expander_SequentialLEDControl(I2C_HandleTypeDef *hi2c, uint8_t isHigh)
{
    HAL_StatusTypeDef status;
    const uint16_t delay = 100; // 100ms between each LED

    // Iterate through all expanders
    for (size_t i = 0; i < expanderCount; i++)
    {
        uint8_t addr = expanderAddresses[i];

        // Control CELL_01_LED_01
        status = Expander_SingleLEDControl(hi2c, addr, CELL_01_LED_01, isHigh, delay);
        if (status != HAL_OK) return status;

        // Control CELL_02_LED_01
        status = Expander_SingleLEDControl(hi2c, addr, CELL_02_LED_01, isHigh, delay);
        if (status != HAL_OK) return status;

        // Control CELL_03_LED_01
        status = Expander_SingleLEDControl(hi2c, addr, CELL_03_LED_01, isHigh, delay);
        if (status != HAL_OK) return status;
    }

    return HAL_OK;
}

void RunLEDSequence(void)
{
    // Array of LED configurations (device address and pin)
    static const struct {
        I2C_HandleTypeDef* i2c;
        uint8_t address;
        uint16_t pin;
    } leds[] = {
        {&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01},


        {&hi2c2, GPIO_EXPANDER_ID_01, CELL_02_LED_01},


        {&hi2c2, GPIO_EXPANDER_ID_01, CELL_03_LED_01},


        {&hi2c2, GPIO_EXPANDER_ID_02, CELL_01_LED_01},


        {&hi2c2, GPIO_EXPANDER_ID_02, CELL_02_LED_01},


        {&hi2c2, GPIO_EXPANDER_ID_02, CELL_03_LED_01},


        {&hi2c2, GPIO_EXPANDER_ID_03, CELL_01_LED_01},


		{&hi2c2, GPIO_EXPANDER_ID_03, CELL_02_LED_01},


		{&hi2c2, GPIO_EXPANDER_ID_03, CELL_03_LED_01},


		{&hi2c2, GPIO_EXPANDER_ID_04, CELL_01_LED_01},


		{&hi2c2, GPIO_EXPANDER_ID_04, CELL_02_LED_01},


        {&hi2c2, GPIO_EXPANDER_ID_04, CELL_03_LED_01},
        {&hi2c3, GPIO_EXPANDER_ID_04, CELL_03_LED_01},

		{&hi2c3, GPIO_EXPANDER_ID_04, CELL_02_LED_01},
		{&hi2c3, GPIO_EXPANDER_ID_04, CELL_01_LED_01},
		{&hi2c3, GPIO_EXPANDER_ID_03, CELL_03_LED_01},
		{&hi2c3, GPIO_EXPANDER_ID_03, CELL_02_LED_01},
		{&hi2c3, GPIO_EXPANDER_ID_03, CELL_01_LED_01},
		{&hi2c3, GPIO_EXPANDER_ID_02, CELL_03_LED_01},
		{&hi2c3, GPIO_EXPANDER_ID_02, CELL_02_LED_01},
		{&hi2c3, GPIO_EXPANDER_ID_02, CELL_01_LED_01},
		{&hi2c3, GPIO_EXPANDER_ID_01, CELL_03_LED_01},
		{&hi2c3, GPIO_EXPANDER_ID_01, CELL_02_LED_01},
        {&hi2c3, GPIO_EXPANDER_ID_01, CELL_01_LED_01}
    };

    const size_t led_count = sizeof(leds) / sizeof(leds[0]);

    // Turn each LED on and off sequentially
    for (size_t i = 0; i < led_count; i++) {
        // Turn on current LED
        Expander_SingleLEDControl(leds[i].i2c, leds[i].address, leds[i].pin, 1, 500);

        // Turn off previous LED (if not first LED)
        if (i > 0) {
            Expander_SingleLEDControl(leds[i-1].i2c, leds[i-1].address, leds[i-1].pin, 0, 0);
        }
    }

    // Turn off last LED
    Expander_SingleLEDControl(leds[led_count-1].i2c, leds[led_count-1].address, leds[led_count-1].pin, 0, 500);
}


