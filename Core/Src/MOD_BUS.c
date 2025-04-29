/*
 * MOD_Bus.c
 *
 *  Created on: Mar 18, 2025
 *      Author: pavank
 */

#include <MOD_BUS.h>
#include <string.h>
#include "usart.h"

typedef struct {
    UART_HandleTypeDef* huart;
    GPIO_TypeDef* dePort;
    uint16_t dePin;
} RS485_Handle;

// Matrix for storing data
static uint8_t rs485DataMatrix[RS485_CHANNEL_COUNT][RS485_BUFFER_SIZE] = {0};

// Channel configurations
static RS485_Handle channels[RS485_CHANNEL_COUNT] = {
    {&huart1, USART1_ENABLE_GPIO_Port, USART1_ENABLE_Pin},
    {&huart2, USART2_ENABLE_GPIO_Port, USART2_ENABLE_Pin},
    {&huart3, USART3_ENABLE_GPIO_Port, USART3_ENABLE_Pin},
    {&huart6, USART6_ENABLE_GPIO_Port, USART6_ENABLE_Pin}
};

void RS485_Init(RS485_Channel channel)
{
    RS485_Handle* ch = &channels[channel];

    // Set to receive mode
    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_RESET);

    // Start reception
    HAL_UARTEx_ReceiveToIdle_IT(ch->huart, rs485DataMatrix[channel], RS485_BUFFER_SIZE);
}

void RS485_Send(RS485_Channel channel, const char* msg)
{
    RS485_Handle* ch = &channels[channel];
    uint16_t len = strlen(msg);

    // Enable transmitter
    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_SET);
    HAL_Delay(1);

    // Send data
    HAL_UART_Transmit(ch->huart, (uint8_t*)msg, len, 1000);
    while (__HAL_UART_GET_FLAG(ch->huart, UART_FLAG_TC) == RESET);

    // Return to receive mode
    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_RESET);

    // Restart reception
    HAL_UARTEx_ReceiveToIdle_IT(ch->huart, rs485DataMatrix[channel], RS485_BUFFER_SIZE);
}

uint8_t RS485_Available(RS485_Channel channel)
{
    return strlen((char*)rs485DataMatrix[channel]) > 0;
}

uint16_t RS485_GetData(RS485_Channel channel, char* buffer)
{
    uint16_t size = strlen((char*)rs485DataMatrix[channel]);
    if (size > 0) {
        memcpy(buffer, rs485DataMatrix[channel], size);
        memset(rs485DataMatrix[channel], 0, RS485_BUFFER_SIZE); // Clear the matrix row after reading
    }
    return size;
}

void RS485_GetMatrixData(uint8_t matrix[][RS485_BUFFER_SIZE])
{
    memcpy(matrix, rs485DataMatrix, sizeof(rs485DataMatrix));
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
{
    for (int i = 0; i < RS485_CHANNEL_COUNT; i++) {
        if (huart == channels[i].huart) {
            // Data is already written directly into rs485DataMatrix by HAL_UARTEx_ReceiveToIdle_IT
            rs485DataMatrix[i][Size] = '\0'; // Null-terminate the string
            HAL_UARTEx_ReceiveToIdle_IT(huart, rs485DataMatrix[i], RS485_BUFFER_SIZE);
            break;
        }
    }
}
