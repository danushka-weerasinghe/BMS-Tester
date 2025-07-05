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

#include "Temp_controller.h"

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



// Main function declarations
void tester_setup(void);
void Voltage_Sequence_Automatic(void);
void Set_voltage_and_measure(const Cell_Config* cell, float voltage);
void init_ina229_devices(void);

// Measurement functions (existing)
float Get_INA_Voltage(Cell_Config* cell);
float Get_INA_Current(const Cell_Config* cell);
float Get_INA_Temp(const Cell_Config* cell);

// SET functions (for Modbus function 0x3)
void Set_Output_Voltage(uint8_t id, float voltage);
void Set_Resistance(uint8_t tempCardId, uint8_t resistance);
void Set_LED_status(uint8_t id, uint8_t state);
void Set_Balance_State(uint8_t id, uint8_t balance_state);
void Open_Wire_Test(uint8_t id, uint8_t test_enable);
void Set_Daisy_Chain(uint8_t chain_state);
void Run_Automatic_Sequence(uint8_t sequence_id, uint8_t sequence_state);

// GET functions (for Modbus function 0x4) - Cell measurements
float Get_Cell_Voltage(uint8_t id);
float Get_Cell_Temperature(uint8_t id);
float Get_Cell_Current(uint8_t id);
uint8_t Get_Cell_Temp_Resistance(uint8_t id);

// GET functions - DC-CSU measurements
float Get_DC_CSU_Voltage(uint8_t id);
float Get_DC_CSU_Temperature(uint8_t id);
uint8_t Get_DC_CSU_Balance_Register(uint8_t id);
uint8_t Get_DC_CSU_Open_Wire(uint8_t id);

// GET functions - 11-CSU measurements
float Get_11_CSU_Voltage(uint8_t id);
float Get_11_CSU_Temperature(uint8_t id);
uint8_t Get_11_CSU_Balance_Register(uint8_t id);
uint8_t Get_11_CSU_Open_Wire(uint8_t id);

// GET functions - 12-CSU measurements
float Get_12_CSU_Voltage(uint8_t id);
float Get_12_CSU_Temperature(uint8_t id);
uint8_t Get_12_CSU_Balance_Register(uint8_t id);
uint8_t Get_12_CSU_Open_Wire(uint8_t id);

// Response handling functions
void Send_Modbus_Response(uint8_t function_code, uint8_t* data, uint16_t length);
void Send_Error_Response(uint8_t function_code, uint8_t error_code);

// Utility functions
bool Validate_Cell_ID(uint8_t id, uint8_t max_range);
bool Validate_Temp_Sensor_ID(uint8_t id, uint8_t max_range);
void Process_Modbus_Command(uint8_t* rx_data);

#endif /* INC_BMS_TEST_PROTOCOL_H_ */
