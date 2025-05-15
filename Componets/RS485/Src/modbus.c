/*
 * MOD_Bus.c
 *
 *  Created on: Mar 18, 2025
 *      Author: Pavan Kavinda
 */

#include "modbus.h"
#include "u8g2.h"
#include <string.h>
#include <stdio.h>
#include "main.h"

static u8g2_t u8g2;

typedef struct {
    UART_HandleTypeDef* huart;
    GPIO_TypeDef* dePort;
    uint16_t dePin;
    uint8_t rxBuffer[MODBUS_BUFFER_SIZE];
    uint16_t rxSize;
} MODBUS_Handle;

static MODBUS_Handle modbusChannels[MODBUS_CHANNEL_COUNT] = {
    {&huart1, USART1_ENABLE_GPIO_Port, USART1_ENABLE_Pin},
    {&huart2, USART2_ENABLE_GPIO_Port, USART2_ENABLE_Pin},
    {&huart3, USART3_ENABLE_GPIO_Port, USART3_ENABLE_Pin},
    {&huart6, USART6_ENABLE_GPIO_Port, USART6_ENABLE_Pin}
};

void MODBUS_Init(MODBUS_Channel channel) {
    if (channel >= MODBUS_CHANNEL_COUNT) return;

    MODBUS_Handle* ch = &modbusChannels[channel];

    // Set DE/RE to receive mode
    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_RESET);

    // Start reception
    HAL_UARTEx_ReceiveToIdle_IT(ch->huart, ch->rxBuffer, MODBUS_BUFFER_SIZE);
}

void MODBUS_Send(MODBUS_Channel channel, const uint8_t* data, uint16_t size) {
    if (channel >= MODBUS_CHANNEL_COUNT || size == 0) return;

    MODBUS_Handle* ch = &modbusChannels[channel];

    // Set DE/RE to transmit mode
    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_SET);
    HAL_Delay(1);

    // Send data
    HAL_UART_Transmit(ch->huart, (uint8_t*)data, size, 100);
    while (__HAL_UART_GET_FLAG(ch->huart, UART_FLAG_TC) == RESET);

    // Return to receive mode
    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_RESET);
    HAL_UARTEx_ReceiveToIdle_IT(ch->huart, ch->rxBuffer, MODBUS_BUFFER_SIZE);
}

void MODBUS_UpdateDisplay(MODBUS_Channel channel, const uint8_t* data, uint16_t size) {
    char buffer[64] = {0};
    char hex[4];
    snprintf(buffer, sizeof(buffer), "CH%d: ", channel + 1);

    for (uint16_t i = 0; i < size && i < MODBUS_BUFFER_SIZE; i++) {
        snprintf(hex, sizeof(hex), "%02X ", data[i]);
        strcat(buffer, hex);
    }

    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
    u8g2_DrawStr(&u8g2, 0, (channel + 1) * 10, buffer);
    u8g2_SendBuffer(&u8g2);
}

void MODBUS_ProcessData(MODBUS_Channel channel) {
    // Placeholder: Implement user-defined logic here
}

// Callback for receiving data
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
    for (int i = 0; i < MODBUS_CHANNEL_COUNT; i++) {
        if (huart == modbusChannels[i].huart) {
            MODBUS_Handle* ch = &modbusChannels[i];
            ch->rxSize = Size;

            // Optional: Process received data or update display
            MODBUS_UpdateDisplay(i, ch->rxBuffer, Size);

            // Restart reception
            HAL_UARTEx_ReceiveToIdle_IT(huart, ch->rxBuffer, MODBUS_BUFFER_SIZE);
            break;
        }
    }
}
