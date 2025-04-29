/*
 * button_led.h
 *
 *  Created on: Feb 28, 2025
 *      Author: Pavan Kavinda
 */

#ifndef INC_BUTTON_LED_H_
#define INC_BUTTON_LED_H_

#include "stm32f4xx_hal.h"

extern uint8_t mode;
extern uint8_t id;

extern GPIO_TypeDef* DIP_SWITCH_PORTS[];
extern const uint16_t DIP_SWITCH_PINS[];

extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c3;

// Enum for LED states
typedef enum {
    Low = 0,
    High,
} LED_Set_State;

// Enum for LEDs
typedef enum {
    LED_1 = 1,
    LED_2,
    LED_3,
    LED_4,
    LED_5,
    LED_6,
    LED_7,
    LED_8,
    LED_9,
    NUM_LEDS // Total number of LEDs
} LED_ID;

typedef enum {
    PC_G,	/* PC Green */
    PC_Y,	/* PC Yellow */
    DC_G,	/* Daisy Chain Green */
    DC_Y,	/* Daisy Chain Yellow */
    NUM_COMM_LEDS // Total number of communication LEDs
} CommLED_ID;

typedef enum {			/* Active low LEDs */
    CELL12_TEMP_01,
    CELL12_TEMP_02,
    CELL12_TEMP_03,
    CELL11_TEMP_01,
    CELL11_TEMP_02,
    CELL11_TEMP_03,
    NUM_TEMP_LEDS // Total number of temperature LEDs
} TempLED_ID;

typedef enum {
    CELL12_CELL1,
    CELL12_CELL2,
    CELL12_CELL3,
    CELL12_CELL4,
    CELL12_CELL5,
    CELL12_CELL6,
    CELL12_CELL7,
    CELL12_CELL8,
    CELL12_CELL9,
    CELL12_CELL10,
    CELL12_CELL11,
    CELL12_CELL12,
    CELL11_CELL1,
    CELL11_CELL2,
    CELL11_CELL3,
    CELL11_CELL4,
    CELL11_CELL5,
    CELL11_CELL6,
    CELL11_CELL7,
    CELL11_CELL8,
    CELL11_CELL9,
    CELL11_CELL10,
    CELL11_CELL11,
    CELL11_CELL12,
	NUM_CELL_LEDS
} CellLED_ID;

// LED pin and port structure for GPIO LEDs
typedef struct {
    GPIO_TypeDef* port; // GPIO Port
    uint16_t pin;       // GPIO Pin
} LED_Config;

// I2C handle, expander ID and pin structure for GPIO Expanded LEDs
typedef struct {
    I2C_HandleTypeDef *i2c_handle;  // I2C handle
    uint8_t expander_id;           // GPIO expander ID
    uint16_t pin;                  // Pin on the GPIO expander
} Cell_LED_Config;

void Push_ButtonHandler(uint16_t GPIO_Pin);

uint8_t DIP_GetMode(void);   /* Returns mode value (0-15) from GPIOG */
uint8_t DIP_GetID(void);     // Returns ID value (0-15) from GPIOD */

// Functions for LED control
void LED_Init(void);
void LED_Set(LED_ID led, LED_Set_State state);
void LED_Toggle(LED_ID led);
void LED_ToggleAll(void);
void LED_RS485(CommLED_ID led, LED_Set_State state);
void LED_ToggleRS485(CommLED_ID led);
void LED_Temp(TempLED_ID led, LED_Set_State state);
void LED_ToggleTemp(TempLED_ID led);
void LED_Cell(CellLED_ID led, LED_Set_State state);
void LED_ToggleCell(CellLED_ID led);
void LED_SetAll(LED_Set_State state);
void LED_Test_All(void);

#endif /* INC_BUTTON_LED_H_ */
