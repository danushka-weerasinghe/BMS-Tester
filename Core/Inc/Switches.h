/*
 * Switches.h
 *
 *  Created on: Feb 27, 2025
 *      Author: pavank
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

#include "stm32f4xx_hal.h"

extern uint8_t mode;
extern uint8_t id;

void Push_ButtonHandler(uint16_t GPIO_Pin);

/* External function declaration from LED library */
//extern void LED_Toggle(uint8_t led_number);

//void DIP_Init(void);
uint8_t DIP_GetMode(void);   /* Returns mode value (0-15) from GPIOG */
uint8_t DIP_GetID(void);     // Returns ID value (0-15) from GPIOD */
//uint8_t DIP_GetAll(void);    // Returns all 8 bits combined */

#endif /* SWITCHES_H_ */
