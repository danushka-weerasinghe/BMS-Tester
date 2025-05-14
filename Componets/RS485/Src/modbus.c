/*
 * MOD_Bus.c
 *
 *  Created on: Mar 18, 2025
 *      Author: Pavan Kavinda
 */

#include "modbus.h"
#include "main.h"
//#include "usart.h"
#include "display.h"
#include "u8g2.h"
#include <string.h>

static u8g2_t u8g2;

typedef struct {
    UART_HandleTypeDef* huart;
    GPIO_TypeDef* dePort;
    uint16_t dePin;
    CircularBuffer rxBuffer;
    uint8_t tempBuffer[MODBUS_BUFFER_SIZE];
} MODBUS_Handle;

//typedef struct {
//    uint8_t buffer[MODBUS_BUFFER_SIZE];
//    uint16_t head;
//    uint16_t tail;
//} CircularBuffer;

static CircularBuffer modbusBuffers[MODBUS_CHANNEL_COUNT];
//
const char* MODBUS_ChannelToString(MODBUS_Channel channel) {
    switch (channel) {
        case MODBUS_PC: return "PC";
        case MODBUS_DC: return "DC";
        case MODBUS_CH3: return "CH3";
        case MODBUS_CH4: return "CH4";
        default: return "Unknown";
    }
}

// Matrix for storing data
static uint8_t rs485DataMatrix[MODBUS_CHANNEL_COUNT][MODBUS_BUFFER_SIZE] = {0};

// Channel configurations
static MODBUS_Handle channels[MODBUS_CHANNEL_COUNT] = {
    {&huart1, USART1_ENABLE_GPIO_Port, USART1_ENABLE_Pin},
    {&huart2, USART2_ENABLE_GPIO_Port, USART2_ENABLE_Pin},
    {&huart3, USART3_ENABLE_GPIO_Port, USART3_ENABLE_Pin},
    {&huart6, USART6_ENABLE_GPIO_Port, USART6_ENABLE_Pin}
};

//static const char* channelNames[] = {"PC", "DC", "CH3", "CH4"};

// Circular buffer operations
static void CircularBuffer_Write(CircularBuffer* cb, const uint8_t* data, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        cb->buffer[cb->head] = data[i];
        uint16_t next = (cb->head + 1) % MODBUS_BUFFER_SIZE;
        if (next == cb->tail) {
            cb->overruns++;
            cb->tail = (cb->tail + 1) % MODBUS_BUFFER_SIZE;
        }
        cb->head = next;
    }
}

//static uint16_t CircularBuffer_Read(CircularBuffer* cb, uint8_t* dest, uint16_t maxSize) {
//    uint16_t count = 0;
//    while (cb->tail != cb->head && count < maxSize) {
//        dest[count++] = cb->buffer[cb->tail];
//        cb->tail = (cb->tail + 1) % MODBUS_BUFFER_SIZE;
//    }
//    return count;
//}

void MODBUS_Init(MODBUS_Channel channel) {
    if (channel >= MODBUS_CHANNEL_COUNT) return;

    MODBUS_Handle* ch = &channels[channel];
    memset(&ch->rxBuffer, 0, sizeof(CircularBuffer));

    // Set to receive mode
    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_RESET);

    // Start reception
    HAL_UARTEx_ReceiveToIdle_IT(ch->huart, ch->tempBuffer, MODBUS_BUFFER_SIZE);
}

//void MODBUS_Init(MODBUS_Channel channel)
//{
//	MODBUS_Handle* ch = &channels[channel];
//
//    // Set to receive mode
//    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_RESET);
//
//    // Start reception
//    HAL_UARTEx_ReceiveToIdle_IT(ch->huart, rs485DataMatrix[channel], MODBUS_BUFFER_SIZE);
//}

//void MODBUS_Send(MODBUS_Channel channel, const char* msg)
//{
//	MODBUS_Handle* ch = &channels[channel];
//    uint16_t len = strlen(msg);
//
//    // Enable transmitter
//    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_SET);
//    HAL_Delay(1);
//
//    // Send data
//    HAL_UART_Transmit(ch->huart, (uint8_t*)msg, len, 1000);
//    while (__HAL_UART_GET_FLAG(ch->huart, UART_FLAG_TC) == RESET);
//
//    // Return to receive mode
//    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_RESET);
//
//    // Restart reception
//    HAL_UARTEx_ReceiveToIdle_IT(ch->huart, rs485DataMatrix[channel], MODBUS_BUFFER_SIZE);
//}

void MODBUS_Send(MODBUS_Channel channel, const uint8_t* data, uint16_t size) {
    if (channel >= MODBUS_CHANNEL_COUNT) return;

    MODBUS_Handle* ch = &channels[channel];

    // Enable transmitter
    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_SET);
    HAL_Delay(1);

    // Transmit
    HAL_UART_Transmit(ch->huart, data, size, 100);
    while(__HAL_UART_GET_FLAG(ch->huart, UART_FLAG_TC) == RESET);

    // Return to receive mode
    HAL_GPIO_WritePin(ch->dePort, ch->dePin, GPIO_PIN_RESET);
    HAL_UARTEx_ReceiveToIdle_IT(ch->huart, ch->tempBuffer, MODBUS_BUFFER_SIZE);
}

void UpdateChannelDisplay(MODBUS_Channel channel, const uint8_t *data, uint16_t size) {
    static char displayBuffer[MODBUS_CHANNEL_COUNT][64] = {0};  // Static buffer for all channels

    // Convert data to hex string
    char hexLine[32] = {0};
    for (uint16_t i = 0; i < size && i < 8; i++) {  // Limit to 8 bytes
        char hex[4];
        sprintf(hex, "%02X ", data[i]);
        strcat(hexLine, hex);
    }

    // Update this channel's buffer
    snprintf(displayBuffer[channel], sizeof(displayBuffer[channel]),
             "%s: %s", MODBUS_ChannelToString(channel), hexLine);

    // Draw all channels
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_tom_thumb_4x6_tr);  // Larger, more readable font

    for (int i = 0; i < MODBUS_CHANNEL_COUNT; i++) {
        if (strlen(displayBuffer[i]) > 0) {
            u8g2_DrawStr(&u8g2, 0, (i + 1) * 16, displayBuffer[i]);
        }
    }

    u8g2_SendBuffer(&u8g2);
}

//uint8_t MODBUS_Available(MODBUS_Channel channel)
//{
//    return strlen((char*)rs485DataMatrix[channel]) > 0;
//}
//
//uint16_t MODBUS_GetData(MODBUS_Channel channel, char* buffer)
//{
//    uint16_t size = strlen((char*)rs485DataMatrix[channel]);
//    if (size > 0) {
//        memcpy(buffer, rs485DataMatrix[channel], size);
//        memset(rs485DataMatrix[channel], 0, MODBUS_BUFFER_SIZE); // Clear the matrix row after reading
//    }
//    return size;
//}
//
//void MODBUS_GetMatrixData(uint8_t matrix[][MODBUS_BUFFER_SIZE])
//{
//    memcpy(matrix, rs485DataMatrix, sizeof(rs485DataMatrix));
//}
//
//// Update Display for a Channel
//void UpdateChannelDisplay(MODBUS_Channel channel, const uint8_t *data, uint16_t size) {
//	// Validate input size
//	    if (size == 0 || data == NULL) {
//	        return; // No data to display
//	    }
//	// Debugging: Print the updated channel data
////    printf("Updating channel %d with data: %s\n", channel, data);
//
//    // Draw updated data on the display
////    u8g2_ClearBuffer(&u8g2);
//    u8g2_SetFont(&u8g2, u8g2_font_tom_thumb_4x6_tr);
//
//    // Convert binary data to a hex string
//    char hexLine[64] = {0}; // Buffer to hold the converted hex string
//    char hexByte[4];        // Buffer to hold each byte in hex (e.g., "FF ")
//    for (uint16_t i = 0; i < size && (i * 3) < sizeof(hexLine) - 3; i++) {
//        sprintf(hexByte, "%02X ", data[i]); // Convert a single byte to hex
//        strcat(hexLine, hexByte);           // Append to the hex string
//    }
//
//    // Prepare the display content
//    char line[64]; // Extra space for "CH X: "
//    sprintf(line, "%s: %s", MODBUS_ChannelToString(channel), hexLine);
//
//    // Draw the text on the channel's row
//    u8g2_DrawStr(&u8g2, 0, (channel + 1) * 15, line);
//
//    // Update only the specific channel's area
//    UpdateDisplayArea(0, channel * 2, u8g2_GetDisplayWidth(&u8g2) / 8, 2); // Each channel is ~16px high
//}

//void UpdateChannelDisplay() {
//    u8g2_ClearBuffer(&u8g2);
//    u8g2_SetFont(&u8g2, u8g2_font_tom_thumb_4x6_tr);
//
//    for (uint8_t i = 0; i < RS485_CHANNEL_COUNT; i++) {
//        char line[64] = {0};
//        char hexByte[4];
//        for (uint8_t j = 0; j < RS485_BUFFER_SIZE; j++) {
//            sprintf(hexByte, "%02X ", rs485DataMatrix[i][j]);
//            strcat(line, hexByte);
//        }
//        u8g2_DrawStr(&u8g2, 0, (i + 1) * 15, line);
//    }
//
//    u8g2_SendBuffer(&u8g2);
//}

//void CircularBuffer_Write(CircularBuffer* cb, const uint8_t* data, uint16_t size) {
//    for (uint16_t i = 0; i < size; i++) {
//        cb->buffer[cb->head] = data[i];
//        cb->head = (cb->head + 1) % MODBUS_BUFFER_SIZE;
//        if (cb->head == cb->tail) { // Overwrite if full
//            cb->tail = (cb->tail + 1) % MODBUS_BUFFER_SIZE;
//        }
//    }
//}
//
//uint16_t CircularBuffer_Read(CircularBuffer* cb, uint8_t* dest, uint16_t size) {
//    uint16_t count = 0;
//    while (cb->tail != cb->head && count < size) {
//        dest[count++] = cb->buffer[cb->tail];
//        cb->tail = (cb->tail + 1) % MODBUS_BUFFER_SIZE;
//    }
//    return count;
//}
//
//uint16_t CircularBuffer_Available(CircularBuffer* cb) {
//    return (MODBUS_BUFFER_SIZE + cb->head - cb->tail) % MODBUS_BUFFER_SIZE;
//}
//
//void ProcessMODBUSData(MODBUS_Channel channel) {
//    CircularBuffer* cb = &modbusBuffers[channel];
//    uint8_t data[MODBUS_BUFFER_SIZE];
//    uint16_t size = CircularBuffer_Read(cb, data, sizeof(data));
//
//    if (size > 0) {
//        // Update display
//        UpdateChannelDisplay(channel, data, size);
//    }
//}
//
//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
//{
//    for (int i = 0; i < MODBUS_CHANNEL_COUNT; i++) {
//        if (huart == channels[i].huart) {
//            // Null-terminate the received data
////            if (Size < MODBUS_BUFFER_SIZE) {
////                rs485DataMatrix[i][Size] = '\0'; // Properly terminate the string
////            } else {
////                rs485DataMatrix[i][MODBUS_BUFFER_SIZE - 1] = '\0'; // Ensure termination in case of overflow
////            }
//
//            CircularBuffer_Write(&modbusBuffers[i], rs485DataMatrix[i], Size);
//
//            // Update the display for the specific channel
//            UpdateChannelDisplay(i, (char*)rs485DataMatrix[i], strlen((const char *)rs485DataMatrix[i]));
//
//            // Data is already written directly into rs485DataMatrix by HAL_UARTEx_ReceiveToIdle_IT
////            rs485DataMatrix[i][Size] = '\0'; // Null-terminate the string
//            HAL_UARTEx_ReceiveToIdle_IT(huart, rs485DataMatrix[i], MODBUS_BUFFER_SIZE);
//            break;
//        }
//    }
//}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
{
    for (int i = 0; i < MODBUS_CHANNEL_COUNT; i++) {
        if (huart == channels[i].huart) {
            // Store in circular buffer
            CircularBuffer_Write(&modbusBuffers[i], rs485DataMatrix[i], Size);

            // Update display with received data using actual size
            UpdateChannelDisplay(i, rs485DataMatrix[i], Size);

            // Restart reception
            HAL_UARTEx_ReceiveToIdle_IT(huart, rs485DataMatrix[i], MODBUS_BUFFER_SIZE);
            break;
        }
    }
}

void MODBUS_TestDisplay(void) {
    uint8_t testData[] = {0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F, 0x7A, 0x8B};
    UpdateChannelDisplay(MODBUS_PC, testData, 8);
    HAL_Delay(1000);  // Wait to see the display
}
