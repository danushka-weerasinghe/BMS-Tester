/*
 * LED_Builtin.h
 *
 *  Created on: Feb 20, 2025
 *      Author: Pavan Kavinda
 */

#ifndef LED_BUILTIN_H_
#define LED_BUILTIN_H_

#include "stm32f4xx_hal.h"

/* LED pin definitions */
typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} LED_Pin;

/* LED states */
typedef enum {
    OFF = 0,
    ON = 1
} LED_Set_State;

/* Function prototypes */
void LED_Init(void);
void LED_State(uint8_t led_number, LED_Set_State state);
void LED_Toggle(uint8_t led_number);
void LED_SetAll(LED_Set_State state);
void LED_ToggleAll(void);

#endif /* LED_BUILTIN_H_ */
