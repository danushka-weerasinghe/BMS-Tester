/*
 * BMS_test_protocol.h
 *
 *  Created on: May 15, 2025
 *      Author: Danushka Weerasighe
 */

#ifndef INC_BMS_TEST_PROTOCOL_H_
#define INC_BMS_TEST_PROTOCOL_H_


#include "main.h"
#include <stdbool.h>
#include <string.h>


#define RS485_MSG_START     0x7E
#define RS485_MSG_END       0x7F
#define RS485_MAX_MSG_SIZE  32
#define RS485_BUFFER_SIZE   256

// Define configuration structure using constant values
typedef struct {
    uint8_t cell_id;      // Cell ID as a number instead of enum
    uint8_t ina_index;    // Index of INA229 (0-23)
    GPIO_TypeDef* gpio;   // GPIO port (this is actually a constant address)
    uint16_t cs_pin;      // Chip select pin
    uint16_t led_pin;     // LED pin
} Cell_Config;





typedef struct {
    uint8_t data[RS485_MAX_MSG_SIZE];
    uint16_t length;
    bool complete;
} RS485_Message;

extern RS485_Message current_message;
extern bool receiving_message;

extern uint8_t rs485_rx_buffer[RS485_BUFFER_SIZE];
extern uint8_t rs485_tx_buffer[RS485_BUFFER_SIZE];
extern uint16_t rs485_rx_index;

extern const Cell_Config cell_configs[];
extern INA229_Handle ina229_devices[];

extern int count_1;

void RS485_ProcessByte(uint8_t byte);
void RS485_ProcessMessage(void);

// Function declarations


void Voltage_Sequence_Automatic(void);

void Set_voltage_and_measure(const Cell_Config* cell, float voltage);

void init_ina229_devices(void);

float Get_INA_Voltage(Cell_Config* cell);

float Get_INA_Current(const Cell_Config* cell);

float Get_INA_Temp(const Cell_Config* cell);

#endif /* INC_BMS_TEST_PROTOCOL_H_ */
