/*
 * CAN_BUS.h
 *
 *  Created on: Mar 18, 2025
 *      Author: pavank
 */

#ifndef INC_CAN_BUS_H_
#define INC_CAN_BUS_H_

#include "stm32f4xx_hal.h"
#include "u8g2.h"

// Function Prototypes
void CAN_Init(void);
void CAN_Send_Message(CAN_HandleTypeDef *hcan, uint8_t data[8]);
uint8_t CAN_Get_Data(CAN_RxHeaderTypeDef *rxHeader, uint8_t *buffer, CAN_HandleTypeDef **hcan);

#endif /* INC_CAN_BUS_H_ */

/* Example use */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/*
CAN_RxHeaderTypeDef RxHeader;
uint8_t receivedData[8];
CAN_HandleTypeDef *hcan;
uint8_t message[8] = {0xA1, 0xB2, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
*/
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
/*
CAN_Init();
*/
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
/*
while (1)
{
CAN_Send_Message(&hcan1, message);
HAL_Delay(10);

if (CAN_Get_Data(&RxHeader, receivedData, &hcan)) {

    // Display the received message on the LCD
    u8g2_FirstPage(&u8g2);
    do {
        u8g2_ClearBuffer(&u8g2);
        u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);

        // Display CAN instance and message ID
        char line[64];
        snprintf(line, sizeof(line), "CAN%d MSG ID: %08X",
                 (hcan == &hcan1 ? 1 : 2),
				   (unsigned int)RxHeader.StdId);
        u8g2_DrawStr(&u8g2, 0, 15, line);

        // Display CAN data bytes
        snprintf(line, sizeof(line), "%02X %02X %02X %02X %02X %02X %02X %02X",
                 receivedData[0], receivedData[1], receivedData[2], receivedData[3],
                 receivedData[4], receivedData[5], receivedData[6], receivedData[7]);
        u8g2_DrawStr(&u8g2, 0, 30, line);

        u8g2_SendBuffer(&u8g2);
    } while (u8g2_NextPage(&u8g2));
}
*/
/* USER CODE END WHILE */
