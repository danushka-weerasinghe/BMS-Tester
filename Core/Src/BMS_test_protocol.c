/*
 * BMS_test_protocol.c
 *
 *  Created on: May 15, 2025
 *      Author: Danushka Weerasighe
 *
 */

#include "main.h"
#include "INA229.h"

#include "BMS_test_protocol.h"

#include "gpio_expander.h"

#include "mcu.h"

#include "config.h"

// Add definitions for variables declared in header
RS485_Message current_message = {0};
bool receiving_message = false;
uint8_t rs485_rx_buffer[RS485_BUFFER_SIZE];
uint8_t rs485_tx_buffer[RS485_BUFFER_SIZE];
uint16_t rs485_rx_index = 0;
int count_1 = 0;

extern uint8_t Rx_Data[16];
extern uint8_t Tx_Data[16];

void RS485_ProcessByte(uint8_t byte) {
    if (byte == RS485_MSG_START) {
        receiving_message = true;
        current_message.length = 0;
        current_message.complete = false;
    }
    else if (receiving_message) {
        if (byte == RS485_MSG_END) {
            receiving_message = false;
            current_message.complete = true;
        }
        else if (current_message.length < RS485_MAX_MSG_SIZE) {
            current_message.data[current_message.length++] = byte;
        }
    }
}

void RS485_ProcessMessage(void) {
    count_1 = 1;
    HAL_Delay(1000);

    if (Rx_Data[0] == 0x04) {
        count_1 = 2;
        HAL_Delay(1000);

        uint8_t num_sequences = Rx_Data[1];
        int current_pos = 1;
        current_message.length = 10 ;

        for (int i=0;i<current_message.length;i++){

//        for (uint8_t seq = 0; seq < num_sequences && current_pos < current_message.length; seq++) {
//            uint8_t header = current_message.data[current_pos++];
//            uint8_t command = current_message.data[current_pos++];
//            int id = current_message.data[current_pos++];
//            float volt = (float)current_message.data[current_pos++];
//            int id_LED = current_message.data[current_pos++];
//            int LED_State = current_message.data[current_pos++];
//            int time = current_message.data[current_pos++];

          int  header = Rx_Data[2] ;
            int command = Rx_Data[3];

            int id = 4 ;
            float volt = 2;

            if (Rx_Data[0] == 0x04) {
                switch (Rx_Data[1]) {
                    case 0x01:
                        Set_Output_Voltage(id, volt);
                        count_1 = 3;
                        HAL_Delay(1000);

                        break;
                    case 0x02:
//                        Set_LED_status(id_LED, LED_State);
                        count_1 = 4;
                        HAL_Delay(1000);
                        break;
                    case 0x03:
//                        HAL_Delay(time);
                        break;
                    case 0x04: {
                        Get_INA_Voltage(&cell_configs[id]);
                        // You'll need to store float properly – not just one byte
                        break;
                    }
                }
            }
        }
//        }
    }
}


void init_ina229_devices(void) {
    ina229_devices[0] = INA229_0;
    ina229_devices[1] = INA229_1;
    ina229_devices[2] = INA229_2;
    ina229_devices[3] = INA229_3;
    ina229_devices[4] = INA229_4;
    ina229_devices[5] = INA229_5;
    ina229_devices[6] = INA229_6;
    ina229_devices[7] = INA229_7;
    ina229_devices[8] = INA229_8;
    ina229_devices[9] = INA229_9;
    ina229_devices[10] = INA229_10;
    ina229_devices[11] = INA229_11;
    ina229_devices[12] = INA229_12;
    ina229_devices[13] = INA229_13;
    ina229_devices[14] = INA229_14;
    ina229_devices[15] = INA229_15;
    ina229_devices[16] = INA229_16;
    ina229_devices[17] = INA229_17;
    ina229_devices[18] = INA229_18;
    ina229_devices[19] = INA229_19;
    ina229_devices[20] = INA229_20;
    ina229_devices[21] = INA229_21;
    ina229_devices[22] = INA229_22;
    ina229_devices[23] = INA229_23;
    ina229_devices[24] = INA229_24;
    ina229_devices[25] = INA229_25;
}





// Helper macros to make the table more readable
#define I2C2_BUS 2
#define I2C3_BUS 3

// Define the configurations table
 const Cell_Config cell_configs[] = {
		// First GPIO Expander (ID_01) - Cells 1-3

		// FOR THE FIRST 12 CELLS
    // Cell ID, INA idx, GPIO port,  CS pin,           LED pin
    { 0,          0,       GPIOE,     CELL12_CS_01_Pin, CELL_01_LED_01},
    { 1,          1,       GPIOE,     CELL12_CS_02_Pin, CELL_02_LED_01},
    { 2,          2,       GPIOE,     CELL12_CS_03_Pin, CELL_03_LED_01},

	// Second GPIO Expander (ID_02) - Cells 4-6
    { 3,          3,       GPIOE,     CELL12_CS_04_Pin, CELL_01_LED_01},
    { 4,          4,       GPIOE,     CELL12_CS_05_Pin, CELL_02_LED_01},
    { 5,          5,       GPIOI,     CELL12_CS_06_Pin, CELL_03_LED_01},
	// Third GPIO Expander (ID_03) - Cells 7-9
    { 6,          6,       GPIOC,     CELL12_CS_07_Pin, CELL_01_LED_01},
    { 7,          7,       GPIOI,     CELL12_CS_08_Pin, CELL_02_LED_01},
    { 8,          8,       GPIOI,     CELL12_CS_09_Pin, CELL_03_LED_01},

	// Fourth GPIO Expander (ID_04) - Cells 10-12
    { 9,          9,       GPIOI,     CELL12_CS_10_Pin, CELL_01_LED_01},
    {10,         10,       GPIOF,     CELL12_CS_11_Pin, CELL_02_LED_01},
    {11,         11,       GPIOF,     CELL12_CS_12_Pin, CELL_03_LED_01},

	// FOR THE SECOND 12 CELLS
    // Cell ID, I2C bus, INA idx, GPIO port,  CS pin,           LED pin
    {12,         13,       GPIOB,     CELL11_CS_01_Pin, CELL_01_LED_01},
    {13,         14,       GPIOB,     CELL11_CS_02_Pin, CELL_02_LED_01},
    {14,         15,       GPIOF,     CELL11_CS_03_Pin, CELL_03_LED_01},

	// Second GPIO Expander (ID_02) - Cells 4-6
    {15,         16,       GPIOF,     CELL11_CS_04_Pin, CELL_01_LED_01},
    {16,         17,       GPIOF,     CELL11_CS_05_Pin, CELL_02_LED_01},
    {17,         18,       GPIOF,     CELL11_CS_06_Pin, CELL_03_LED_01},
	// Third GPIO Expander (ID_03) - Cells 7-9
    {18,         19,       GPIOF,     CELL11_CS_07_Pin, CELL_01_LED_01},
    {19,         20,       GPIOG,     CELL11_CS_08_Pin, CELL_02_LED_01},
    {20,         21,       GPIOG,     CELL11_CS_09_Pin, CELL_03_LED_01},

	// Fourth GPIO Expander (ID_04) - Cells 10-12
    {21,         22,       GPIOE,     CELL11_CS_10_Pin, CELL_01_LED_01},
    {22,         23,       GPIOE,     CELL11_CS_11_Pin, CELL_02_LED_01},
    {23,         24,       GPIOE,     CELL11_CS_12_Pin, CELL_03_LED_01},

	//12 cell CSU voltage and current reading
	{24,         12,       GPIOF,     CSU_12_CELLS_Pin, CELL_01_LED_01},

	//11 cell CSU voltage and current reading
	{25,         25,       GPIOE,     CSU_11_CELLS_Pin, CELL_03_LED_01}


};


// Helper function to get INA handle from index
static INA229_Handle get_ina_handle(uint8_t index) {
    return INA229_0 + index;  // Assuming INA handles are sequential
}

void Voltage_Sequence_Automatic(void)
{

    const float test_voltages[] = {2.0f, 2.5f, 2.8f, 3.3f, 3.4f, 3.6f, 4.0f, 4.2f};
    const int num_voltages = sizeof(test_voltages) / sizeof(test_voltages[0]);
    const int num_cells = sizeof(cell_configs) / sizeof(cell_configs[0]);

    for(int v = 0; v < num_voltages; v++) {
        for(int c = 0; c < num_cells; c++) {
        	Set_voltage_and_measure(&cell_configs[c], test_voltages[v]);

        	cell_voltage_read();

            HAL_Delay(10);  // Delay between cells
        }
        HAL_Delay(10);  // Delay between voltage levels
    }






}

void Set_voltage_and_measure(const Cell_Config* cell, float voltage)

{
    // Get the actual handles from the configuration
    INA229_Handle ina = get_ina_handle(cell->ina_index);

    // Only set voltage and control LED for cells 0-11 (skip CSU cells 12 and 13) COZ this is for the slave board reading
    if (cell->cell_id != 24 && cell->cell_id != 25)

    {

    // Set voltage for the cell
    Set_Output_Voltage(cell->cell_id, voltage);

    // Turn on LED

    Set_LED_status(cell->cell_id, OFF);

    }
    HAL_Delay(10);
    // Read voltage and temperature
    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    INA229_Readings[cell->ina_index].voltage_V = INA229_getVBUS_V(ina);
    HAL_Delay(1);
    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_SET);

    HAL_Delay(10);

    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    INA229_Readings[cell->ina_index].temperature_C = INA229_getDIETEMP_C(ina);
    HAL_Delay(1);
    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_SET);

    // Turn off LED
    HAL_Delay(10);

    // Only turn off LED for cells 0-11 (skip CSU cells 12 and 13)
    if (cell->cell_id != 24 && cell->cell_id != 25)

    {

    Set_LED_status(cell->cell_id, ON);

    }
}


float Get_INA_Voltage(Cell_Config* cell)
{

    // Get the actual handles from the configuration
    INA229_Handle ina = get_ina_handle(cell->ina_index);

	Set_LED_status(cell->cell_id, OFF);

    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    INA229_Readings[cell->ina_index].temperature_C = INA229_getDIETEMP_C(ina);
    HAL_Delay(1);
    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_SET);

	Set_LED_status(cell->cell_id, ON);

	return INA229_Readings[cell->ina_index].voltage_V;
}


float Get_INA_Current(const Cell_Config* cell)
{

    // Get the actual handles from the configuration
    INA229_Handle ina = get_ina_handle(cell->ina_index);

	Set_LED_status(cell->cell_id, OFF);

    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    INA229_Readings[cell->ina_index].voltage_V = INA229_getVBUS_V(ina);
    HAL_Delay(1);
    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_SET);

	Set_LED_status(cell->cell_id, ON);

	return INA229_Readings[cell->ina_index].voltage_V;
}

float Get_INA_Temp(const Cell_Config* cell)
{

    // Get the actual handles from the configuration
    INA229_Handle ina = get_ina_handle(cell->ina_index);

	Set_LED_status(cell->cell_id, OFF);

    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    INA229_Readings[cell->ina_index].voltage_V = INA229_getVBUS_V(ina);
    HAL_Delay(1);
    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_SET);

	Set_LED_status(cell->cell_id, ON);

	return INA229_Readings[cell->ina_index].voltage_V;
}
