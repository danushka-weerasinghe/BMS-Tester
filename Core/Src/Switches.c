/*
 * Switches.c
 *
 *  Created on: Feb 27, 2025
 *      Author: Pavan Kavinda
 */

#include <Switches.h>
#include "gpio.h"
#include "LED_BuiltIn.h"

uint8_t mode;
uint8_t id;
uint32_t currentTime;
uint32_t previousTime;

// External pin definitions
extern GPIO_TypeDef* DIP_SWITCH_PORTS[];
extern const uint16_t DIP_SWITCH_PINS[];

const uint16_t BUTTON_PINS[] = {PUSH_BUTTON_01_Pin, PUSH_BUTTON_02_Pin, PUSH_BUTTON_03_Pin, PUSH_BUTTON_04_Pin};

void Push_ButtonHandler(uint16_t GPIO_Pin)
{
    currentTime = HAL_GetTick();

    for(int j = 0; j < 4; j++) {
        if((GPIO_Pin == BUTTON_PINS[j]) && (currentTime - previousTime > 100)) {
            LED_Toggle(j+1);  // Keep your existing LED toggle
            previousTime = currentTime;
            break;
        }
    }
}

uint8_t DIP_GetMode(void)
{
    mode = 0;

    /* Read MODE switches from GPIOG (first 4 switches) */
    if(HAL_GPIO_ReadPin(GPIOG, DIP_SWITCH_MODE_01_Pin) == GPIO_PIN_RESET) mode |= 0x01;
    if(HAL_GPIO_ReadPin(GPIOG, DIP_SWITCH_MODE_02_Pin) == GPIO_PIN_RESET) mode |= 0x02;
    if(HAL_GPIO_ReadPin(GPIOG, DIP_SWITCH_MODE_03_Pin) == GPIO_PIN_RESET) mode |= 0x04;
    if(HAL_GPIO_ReadPin(GPIOG, DIP_SWITCH_MODE_04_Pin) == GPIO_PIN_RESET) mode |= 0x08;

    return mode;
}

uint8_t DIP_GetID(void)
{
    id = 0;

    /* Read ID switches from GPIOD (last 4 switches) */
    if(HAL_GPIO_ReadPin(GPIOD, DIP_SWITCH_01_Pin) == GPIO_PIN_RESET) id |= 0x01;
    if(HAL_GPIO_ReadPin(GPIOD, DIP_SWITCH_02_Pin) == GPIO_PIN_RESET) id |= 0x02;
    if(HAL_GPIO_ReadPin(GPIOD, DIP_SWITCH_03_Pin) == GPIO_PIN_RESET) id |= 0x04;
    if(HAL_GPIO_ReadPin(GPIOD, DIP_SWITCH_04_Pin) == GPIO_PIN_RESET) id |= 0x08;

    return id;
}

//uint8_t DIP_GetAll(void)
//{
//    uint8_t all = 0;
//
//    // Combine MODE and ID values
//    all = (DIP_GetMode() << 4) | DIP_GetID();
//    return all;
//}
