/*
 * MOD_Bus.h
 *
 *  Created on: Mar 18, 2025
 *      Author: pavank
 */

#ifndef INC_MOD_BUS_H_
#define INC_MOD_BUS_H_

#include "stm32f4xx_hal.h"

// Channel mapping
typedef enum {
    RS485_CH1 = 0,  // USART1
    RS485_CH2,      // USART2
    RS485_CH3,      // USART3
    RS485_CH6       // USART6
} RS485_Channel;

// Initialize RS485 channel
void RS485_Init(RS485_Channel channel);

// Send data on specified channel
void RS485_Send(RS485_Channel channel, const char* msg);

// Check if data available
uint8_t RS485_Available(RS485_Channel channel);

// Get received data (returns length)
uint16_t RS485_GetData(RS485_Channel channel, char* buffer);

#endif /* INC_MOD_BUS_H_ */
