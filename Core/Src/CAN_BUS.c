/*
 * CAN_BUS.c
 *
 *  Created on: Mar 18, 2025
 *      Author: pavank
 */

#include "CAN_BUS.h"
#include "can.h"
#include <string.h>

static uint8_t receivedFlag = 0;
static CAN_RxHeaderTypeDef header;
static uint8_t receivedData[8];
static CAN_HandleTypeDef *receivedCAN = NULL;

// Static function prototypes (for internal use)
static void CAN_Filter_Config(void);

static void CAN_Filter_Config(void) {
    CAN_FilterTypeDef sFilterConfig;

    // Configure filter for CAN1
    sFilterConfig.FilterBank = 0;                 // Filter bank for CAN1
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14; // Start CAN2 filter bank

    if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
        Error_Handler();
    }

    // Configure filter for CAN2
    sFilterConfig.FilterBank = 14; // Filter bank for CAN2
    if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) {
        Error_Handler();
    }
}

void CAN_Init(void) {
    // Configure CAN filters
    CAN_Filter_Config();

    // Start the CAN modules
    if (HAL_CAN_Start(&hcan1) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_CAN_Start(&hcan2) != HAL_OK) {
        Error_Handler();
    }

    // Activate CAN notifications
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
        Error_Handler();
    }
}

void CAN_Send_Message(CAN_HandleTypeDef *hcan, uint8_t data[8]) {
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;

    // Configure CAN Tx Header
    TxHeader.StdId = 0x123;                // Standard Identifier (11-bit)
    TxHeader.ExtId = 0x00;                 // Extended Identifier (ignored for standard ID)
    TxHeader.IDE = CAN_ID_STD;             // Standard ID
    TxHeader.RTR = CAN_RTR_DATA;           // Data Frame
    TxHeader.DLC = 8;                      // Data Length Code (0-8 bytes)
    TxHeader.TransmitGlobalTime = DISABLE; // Disable timestamp

    // Transmit the message
    if (HAL_CAN_AddTxMessage(hcan, &TxHeader, data, &TxMailbox) != HAL_OK) {
        // Transmission Error
        Error_Handler();
    }

    // Optionally, wait for the transmission to complete
    while (HAL_CAN_IsTxMessagePending(hcan, TxMailbox)) {
        // Wait for the message to be sent
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, receivedData) == HAL_OK) {
        receivedFlag = 1; // Set flag to indicate new data received
        receivedCAN = hcan; // Store the CAN instance (CAN1 or CAN2)
    }
}

uint8_t CAN_Get_Data(CAN_RxHeaderTypeDef *rxHeader, uint8_t *buffer, CAN_HandleTypeDef **hcan) {
    if (receivedFlag) {  // Check if new data is available
        memcpy(buffer, receivedData, sizeof(receivedData));  // Copy received data
        *rxHeader = header; // Copy header information
        *hcan = receivedCAN; // Pass the CAN instance
        receivedFlag = 0;   // Reset the flag
        return 1;           // Indicate new data was received
    }
    return 0; // No new data
}
