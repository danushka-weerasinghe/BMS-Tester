/*
 * MOD_Bus.c
 *
 *  Created on: Mar 18, 2025
 *      Author: pavank
 */

#include <MOD_BUS.h>
#include <string.h>
#include "usart.h"

#define RS485_BUFFER_SIZE 64

typedef struct {
    UART_HandleTypeDef* huart;
    GPIO_TypeDef* dePort;
    uint16_t dePin;
    uint8_t rxBuffer[RS485_BUFFER_SIZE];
    uint16_t rxSize;
    uint8_t dataReady;
} RS485_Handle;

// Channel configurations
static RS485_Handle channels[4] = {
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
    HAL_UARTEx_ReceiveToIdle_IT(ch->huart, ch->rxBuffer, RS485_BUFFER_SIZE);
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
    while(__HAL_UART_GET_FLAG(ch->huart, UART_FLAG_TC) == RESET);

    // Return to receive mode
    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_RESET);

    // Restart reception
    HAL_UARTEx_ReceiveToIdle_IT(ch->huart, ch->rxBuffer, RS485_BUFFER_SIZE);
}

uint8_t RS485_Available(RS485_Channel channel)
{
    return channels[channel].dataReady;
}

uint16_t RS485_GetData(RS485_Channel channel, char* buffer)
{
    RS485_Handle* ch = &channels[channel];
    uint16_t size = 0;

    if(ch->dataReady) {
        // Clear destination buffer first
        memset(buffer, 0, RS485_BUFFER_SIZE);
        // Copy received data
        memcpy(buffer, ch->rxBuffer, ch->rxSize);
        size = ch->rxSize;
        // Clear the receive buffer
        memset(ch->rxBuffer, 0, RS485_BUFFER_SIZE);
        ch->dataReady = 0;
    }

    return size;

//    RS485_Handle* ch = &channels[channel];
//    uint16_t size = 0;
//
//    if(ch->dataReady) {
//        memcpy(buffer, ch->rxBuffer, ch->rxSize);
//        size = ch->rxSize;
//        ch->dataReady = 0;
//    }
//
//    return size;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    for(int i = 0; i < 4; i++) {
        if(huart == channels[i].huart) {
            channels[i].rxSize = Size;
            channels[i].dataReady = 1;
            HAL_UARTEx_ReceiveToIdle_IT(huart, channels[i].rxBuffer, RS485_BUFFER_SIZE);
            break;
        }
    }
}
