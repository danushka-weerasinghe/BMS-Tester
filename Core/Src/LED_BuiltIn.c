/*
 * LED_Builtin.c
 *
 *  Created on: Feb 20, 2025
 *      Author: Pavan Kavinda
 */

#include <LED_BuiltIn.h>
#include "gpio.h"

// Define LED configurations - adjust ports and pins according to your hardware
static const LED_Pin leds[9] = {
	{GPIOH, LED_09_Pin},  // LED 1
	{GPIOH, LED_08_Pin},  // LED 2
	{GPIOB, LED_01_Pin},  // LED 3
    {GPIOG, LED_02_Pin},  // LED 4
    {GPIOE, LED_03_Pin},  // LED 5
    {GPIOE, LED_04_Pin},  // LED 6
    {GPIOB, LED_05_Pin},  // LED 7
    {GPIOG, LED_06_Pin},  // LED 8
    {GPIOG, LED_07_Pin},  // LED 9


};

void LED_Init(void) {
    for(uint8_t i = 0; i < 9; i++) {
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, GPIO_PIN_RESET);
    }
}

void LED_State(uint8_t led_number, LED_Set_State state) {
    if(led_number >= 1 && led_number <= 9) {
        HAL_GPIO_WritePin(leds[led_number-1].port,
                         leds[led_number-1].pin,
                         state == ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

void LED_Toggle(uint8_t led_number) {
    if(led_number >= 1 && led_number <= 9) {
        HAL_GPIO_TogglePin(leds[led_number-1].port, leds[led_number-1].pin);
    }
}

void LED_SetAll(LED_Set_State state) {
    for(uint8_t i = 0; i < 9; i++) {
        HAL_GPIO_WritePin(leds[i].port,
                         leds[i].pin,
                         state == ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

void LED_ToggleAll(void) {
    for(uint8_t i = 0; i < 9; i++) {
        HAL_GPIO_TogglePin(leds[i].port, leds[i].pin);
    }
}

