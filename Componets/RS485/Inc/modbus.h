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
#define PACKET_SIZE 8

// Channel mapping
typedef enum {
    MODBUS_PC = 0,
	MODBUS_DC,
	MODBUS_CH3,
	MODBUS_CH4
} MODBUS_Channel;

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

extern UART_HandleTypeDef huart6;

//typedef struct {
//    UART_HandleTypeDef* huart;
//    GPIO_TypeDef* dePort;
//    uint16_t dePin;
//} MODBUS_Handle;

typedef struct {
    uint8_t buffer[MODBUS_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
    uint32_t overruns;
} CircularBuffer;

// Initialize RS485 channel
void MODBUS_Init(MODBUS_Channel channel);

// Send data on specified channel
//void MODBUS_Send(MODBUS_Channel channel, const char* msg);
void MODBUS_Send(MODBUS_Channel channel, const uint8_t* data, uint16_t size);

// Check if data available
uint8_t MODBUS_Available(MODBUS_Channel channel);

// Get received data (returns length)
//uint16_t MODBUS_GetData(MODBUS_Channel channel, char* buffer);
uint16_t MODBUS_GetPacket(MODBUS_Channel channel, uint8_t* buffer);

//void MODBUS_GetMatrixData(uint8_t matrix[][MODBUS_BUFFER_SIZE]);

//void UpdateAllChannelDisplay();

void UpdateChannelDisplay(MODBUS_Channel channel, const uint8_t *data, uint16_t size);

void MODBUS_ProcessData(MODBUS_Channel channel);

void MODBUS_TestDisplay(void);

#endif /* INC_MOD_BUS_H_ */

/* Example use */

/* USER CODE BEGIN 2 */
/*

void display_lcd(const char *message)
{
	u8g2_FirstPage(&u8g2);
	    do {
	    	u8g2_ClearBuffer(&u8g2);
			u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
			u8g2_DrawStr(&u8g2, 0, 30, message);
			u8g2_SendBuffer(&u8g2);
	    } while (u8g2_NextPage(&u8g2));
}

RS485_Init(USART_3);
RS485_Init(USART_6);
*/
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
/*
while (1)
{
	  RS485_Send(USART_6, "Testing from USART_6\r\n");

      if(RS485_Available(USART_3))
      {
          char buffer[64];
          uint16_t length = RS485_GetData(USART_3, buffer);

          if(length > 0)
          {
              buffer[length] = '\0';  // Null terminate
              display_lcd(buffer);
          }
      }
*/
/* USER CODE END WHILE */
