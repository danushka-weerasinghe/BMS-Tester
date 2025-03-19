/*
 * gpio_expander.h
 *
 *  Created on: Feb 27, 2025
 *      Author: Danushka Weerasinghe
 *
 *  Modified for improved API design and modularity.
 */

#ifndef INC_GPIO_EXPANDER_H_
#define INC_GPIO_EXPANDER_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c3;

#define HIGH 1
#define LOW 0

/* I2C device addresses for GPIO expanders */
#define GPIO_EXPANDER_ID_01 0x21
#define GPIO_EXPANDER_ID_02 0x22
#define GPIO_EXPANDER_ID_03 0x23
#define GPIO_EXPANDER_ID_04 0x24

/* I2C timeout in milliseconds */
#define EXPANDER_I2C_TIMEOUT 100

/*
 * Pin Definitions:
 * The GPIO expander has 16 I/Os split into two 8-bit ports.
 * The 'pin' parameter for functions like Expander_SetPinState is a 16-bit mask:
 * - Bits [7:0] correspond to Port 0.
 * - Bits [15:8] correspond to Port 1.
 *
 * Example: To control bit 2 of Port 1, use (1U << (2 + 8)).
 */

/* Names for pins on GPIO_EXPANDER_ID_01 (Port 0) */
#define ALERT_PIN_CELL_01    (1U << 0)  // Port0, bit 0
#define CELL_01_VOLTAGE_01   (1U << 1)  // Port0, bit 1
#define CELL_01_VOLTAGE_02   (1U << 2)  // Port0, bit 2
#define CELL_01_VOLTAGE_03   (1U << 3)  // Port0, bit 3
#define CELL_01_LED_01       (1U << 4)  // Port0, bit 4
#define CELL_02_VOLTAGE_03   (1U << 5)  // Port0, bit 5
#define CELL_02_VOLTAGE_02   (1U << 6)  // Port0, bit 6
#define CELL_02_VOLTAGE_01   (1U << 7)  // Port0, bit 7

/* Names for pins on GPIO_EXPANDER_ID_02 (Port 1) */
#define ALERT_PIN_CELL_02    (1U << (0+8))  // Port0, bit 0
#define CELL_02_LED_01       (1U << (1+8))  // Port0, bit 1
#define CELL_03_LED_01       (1U << (2+8))  // Port0, bit 2
#define EXPANDER_FAN_CTRL    (1U << (3+8))  // Port0, bit 3
#define CELL_03_VOLTAGE_03   (1U << (4+8))  // Port0, bit 4
#define CELL_03_VOLTAGE_02   (1U << (5+8))  // Port0, bit 5
#define CELL_03_VOLTAGE_01   (1U << (6+8))  // Port0, bit 6
#define ALERT_PIN_CELL_03    (1U << (7+8))  // Port0, bit 7

/* Register addresses from the PCA9535 datasheet */
#define PCA9535_REG_OUTPUT_PORT0   0x02
#define PCA9535_REG_OUTPUT_PORT1   0x03
#define PCA9535_REG_CONFIG_PORT0   0x06
#define PCA9535_REG_CONFIG_PORT1   0x07

/* Public API */

/**
 * @brief Initializes all known expander devices on the given I2C bus.
 * @param hi2c Pointer to the I2C handle (e.g., &hi2c1 or &hi2c2).
 * @return HAL_StatusTypeDef HAL_OK on success, error code otherwise.
 */
HAL_StatusTypeDef Expander_InitAllDevices(I2C_HandleTypeDef *hi2c);

/**
 * @brief Sets a specific GPIO pin high or low on the specified expander.
 *
 * The 'pin' parameter is a 16-bit mask representing a single pin.
 * Bits [7:0] correspond to Port 0 and bits [15:8] correspond to Port 1.
 * For example, to set bit 2 of Port 1, pass (1U << (2 + 8)).
 *
 * @param hi2c Pointer to the I2C handle.
 * @param deviceAddress 7-bit I2C address of the expander.
 * @param pin 16-bit pin mask.
 * @param isHigh Non-zero to set the pin high; zero to set it low.
 * @return HAL_StatusTypeDef HAL_OK on success, error code otherwise.
 */
HAL_StatusTypeDef Expander_SetPinState(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress, uint16_t pin, uint8_t isHigh);

/**
 * @brief Sets all outputs to HIGH for the specified expander.
 * @param hi2c Pointer to the I2C handle.
 * @param deviceAddress 7-bit I2C address of the expander.
 * @return HAL_StatusTypeDef HAL_OK on success, error code otherwise.
 */
HAL_StatusTypeDef Expander_SetAllOutputsHigh(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress);

/**
 * @brief Sets all outputs to LOW for the specified expander.
 * @param hi2c Pointer to the I2C handle.
 * @param deviceAddress 7-bit I2C address of the expander.
 * @return HAL_StatusTypeDef HAL_OK on success, error code otherwise.
 */
HAL_StatusTypeDef Expander_SetAllOutputsLow(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress);

/* LED Control Functions */
HAL_StatusTypeDef Expander_SequentialLEDControl(I2C_HandleTypeDef *hi2c, uint8_t isHigh);


HAL_StatusTypeDef Expander_SingleLEDControl(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress, uint16_t pin, uint8_t isHigh, uint16_t delay);

void RunLEDSequence(void);

#endif /* INC_GPIO_EXPANDER_H_ */
