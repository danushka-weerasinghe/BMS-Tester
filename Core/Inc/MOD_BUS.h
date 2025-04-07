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
