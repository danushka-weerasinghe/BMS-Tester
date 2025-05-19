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

#define PIN_INPUT   1
#define PIN_OUTPUT  0

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
#define ALERT_PIN_CELL_01    (1U << 0)  // Port0, bit 0  //This pin used to Relay control for open wire Testing
#define CELL_01_VOLTAGE_01   (1U << 1)  // Port0, bit 1
#define CELL_01_VOLTAGE_02   (1U << 2)  // Port0, bit 2
#define CELL_01_VOLTAGE_03   (1U << 3)  // Port0, bit 3
#define CELL_01_LED_01       (1U << 4)  // Port0, bit 4
#define CELL_02_VOLTAGE_03   (1U << 5)  // Port0, bit 5
#define CELL_02_VOLTAGE_02   (1U << 6)  // Port0, bit 6
#define CELL_02_VOLTAGE_01   (1U << 7)  // Port0, bit 7

/* Names for pins on GPIO_EXPANDER_ID_02 (Port 1) */
#define ALERT_PIN_CELL_02    (1U << (0+8))  // Port0, bit 0  //This pin used to Relay control for open wire Testing
#define CELL_02_LED_01       (1U << (1+8))  // Port0, bit 1
#define CELL_03_LED_01       (1U << (2+8))  // Port0, bit 2
#define EXPANDER_FAN_CTRL    (1U << (3+8))  // Port0, bit 3  //This pin is not populate
#define CELL_03_VOLTAGE_03   (1U << (4+8))  // Port0, bit 4
#define CELL_03_VOLTAGE_02   (1U << (5+8))  // Port0, bit 5
#define CELL_03_VOLTAGE_01   (1U << (6+8))  // Port0, bit 6
#define ALERT_PIN_CELL_03    (1U << (7+8))  // Port0, bit 7  //This pin used to Relay control for open wire Testing


/* Direction macros for each pin  (Port 0) */
#define ALERT_PIN_CELL_01_DIR    PIN_INPUT
#define CELL_01_VOLTAGE_01_DIR   PIN_OUTPUT
#define CELL_01_VOLTAGE_02_DIR   PIN_OUTPUT
#define CELL_01_VOLTAGE_03_DIR   PIN_OUTPUT
#define CELL_01_LED_01_DIR       PIN_OUTPUT
#define CELL_02_VOLTAGE_03_DIR   PIN_OUTPUT
#define CELL_02_VOLTAGE_02_DIR   PIN_OUTPUT
#define CELL_02_VOLTAGE_01_DIR   PIN_OUTPUT

/* Direction macros for each pin  (Port 1) */
#define ALERT_PIN_CELL_02_DIR    PIN_INPUT
#define CELL_02_LED_01_DIR       PIN_OUTPUT
#define CELL_03_LED_01_DIR       PIN_OUTPUT
#define EXPANDER_FAN_CTRL_DIR    PIN_INPUT
#define CELL_03_VOLTAGE_03_DIR   PIN_OUTPUT
#define CELL_03_VOLTAGE_02_DIR   PIN_OUTPUT
#define CELL_03_VOLTAGE_01_DIR   PIN_OUTPUT
#define ALERT_PIN_CELL_03_DIR    PIN_OUTPUT


/* Register addresses from the PCA9535 datasheet */
#define PCA9535_REG_OUTPUT_PORT0   0x02
#define PCA9535_REG_OUTPUT_PORT1   0x03
#define PCA9535_REG_CONFIG_PORT0   0x06
#define PCA9535_REG_CONFIG_PORT1   0x07





// Define an enumeration for all 24 cells.
typedef enum {
    CELL_1 = 0,
    CELL_2,
    CELL_3,
    CELL_4,
    CELL_5,
    CELL_6,
    CELL_7,
    CELL_8,
    CELL_9,
    CELL_10,
    CELL_11,
    CELL_12,
    CELL_13,
    CELL_14,
    CELL_15,
    CELL_16,
    CELL_17,
    CELL_18,
    CELL_19,
    CELL_20,
    CELL_21,
    CELL_22,
    CELL_23,
    CELL_24,
    MAX_CELLS
} CellID;


// Define an enumeration for all 24 cells.
typedef enum {
    CELL_1_LED = 0,
    CELL_2_LED,
    CELL_3_LED,
    CELL_4_LED,
    CELL_5_LED,
    CELL_6_LED,
    CELL_7_LED,
    CELL_8_LED,
    CELL_9_LED,
    CELL_10_LED,
    CELL_11_LED,
    CELL_12_LED,
    CELL_13_LED,
    CELL_14_LED,
    CELL_15_LED,
    CELL_16_LED,
    CELL_17_LED,
    CELL_18_LED,
    CELL_19_LED,
    CELL_20_LED,
    CELL_21_LED,
    CELL_22_LED,
    CELL_23_LED,
    CELL_24_LED
} LEDID;


/* Public API */

/**
 * @brief Initializes all known expander devices on the given I2C bus.
 * @param hi2c Pointer to the I2C handle (e.g., &hi2c1 or &hi2c2).
 * @return HAL_StatusTypeDef HAL_OK on success, error code otherwise.
 */
HAL_StatusTypeDef Expander_InitAllDevices(I2C_HandleTypeDef *hi2c);

// Function prototypes:

/**
 * @brief Initializes the pin directions on the expander using the defined direction macros.
 *
 * This function builds the configuration mask for Port 0 from the pin direction macros.
 * For Port 0, each bit is set to 1 if the corresponding pin is configured as an input, and 0 if output.
 * In this example, Port 1 is assumed to be all outputs (0x00).
 *
 * @param hi2c          Pointer to the I2C handle.
 * @param deviceAddress I2C address of the expander.
 * @return HAL_StatusTypeDef HAL_OK if successful, or an error code.
 */
HAL_StatusTypeDef Expander_InitPinDirections(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress);


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
                                      uint8_t state);

// Function prototype to set output voltage for a cell.
void Set_Output_Voltage( CellID cell, float voltage);

// Function prototype to set LED on and off
void Set_LED_status ( CellID cell, uint8_t state);


#endif /* INC_GPIO_EXPANDER_H_ */
