/*
 * BMS_test_protocol.h
 *
 *  Created on: May 15, 2025
 *      Author: Danushka Weerasighe
 */

#ifndef INC_BMS_TEST_PROTOCOL_H_
#define INC_BMS_TEST_PROTOCOL_H_


#include <stdint.h>
#include "main.h"


// Test Step Types
typedef enum {
    VOLTAGE_SET      = 0x00,
	TEMP_SET   		 = 0x01,  // Constant Current Charge
    INA_READING 	 = 0x02,  // Constant Voltage Charge
    ISOSPI_READING 	 = 0x03
} Test_StepType;

//uint8_t rs485_1 = modebus_recive[0];
//uint8_t rs485_2 = modebus_recive[1];
//uint8_t rs485_3 = modebus_recive[2];

void set_test_type();

#endif /* INC_BMS_TEST_PROTOCOL_H_ */
