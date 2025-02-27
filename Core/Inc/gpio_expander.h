/*
 * gpio_expander.h
 *
 *  Created on: Feb 27, 2025
 *      Author: Danushka Weerasinghe
 */

#ifndef INC_GPIO_EXPANDER_H_
#define INC_GPIO_EXPANDER_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"  /* Adjust this include as needed for your MCU */

/* Initializes a single expander (sets all 16 I/Os as outputs).
 * hi2c: pointer to the I2C handle (e.g., &hi2c2 or &hi2c3)
 * deviceAddress: 7-bit address of the expander (e.g., 0x20)
 * Returns HAL_OK on success.
 */
HAL_StatusTypeDef Expander_InitDevice(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress);

/* Initializes all known expander addresses on the given I2C bus.
 * Returns HAL_OK on success.
 */
HAL_StatusTypeDef Expander_InitAllDevices(I2C_HandleTypeDef *hi2c);

/* Sets all outputs on a specified expander high.
 * hi2c: pointer to the I2C handle.
 * deviceAddress: 7-bit address of the expander.
 * Returns HAL_OK on success.
 */
HAL_StatusTypeDef Expander_SetAllOutputsHigh(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress);

/* Sets all outputs on a specified expander low.
 * hi2c: pointer to the I2C handle.
 * deviceAddress: 7-bit address of the expander.
 * Returns HAL_OK on success.
 */
HAL_StatusTypeDef Expander_SetAllOutputsLow(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress);


#endif /* INC_GPIO_EXPANDER_H_ */
