/*
 * MOD_Bus.h
 *
 *  Created on: Mar 18, 2025
 *      Author: Pavan Kavinda
 */

#ifndef INC_MOD_BUS_H_
#define INC_MOD_BUS_H_

#include "stm32f4xx_hal.h"

#define MODBUS_BUFFER_SIZE 8
#define MODBUS_CHANNEL_COUNT 4

// Channel identifiers
typedef enum {
    MODBUS_PC = 0,
    MODBUS_DC,
    MODBUS_CH3,
    MODBUS_CH4
} MODBUS_Channel;

extern UART_HandleTypeDef huart1, huart2, huart3, huart6;

// Initialize the specified MODBUS channel
void MODBUS_Init(MODBUS_Channel channel);

// Send data over the specified MODBUS channel
void MODBUS_Send(MODBUS_Channel channel, const uint8_t* data, uint16_t size);

// Update display with channel data
void MODBUS_UpdateDisplay(MODBUS_Channel channel, const uint8_t* data, uint16_t size);

// Process received data (placeholder for user-defined processing)
void MODBUS_ProcessData(MODBUS_Channel channel);

#endif /* INC_MOD_BUS_H_ */
