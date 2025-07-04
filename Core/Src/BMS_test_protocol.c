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

#include "Temp_controller.h"

#include "config.h"

// Add definitions for variables declared in header

extern uint8_t RxData_modbus_01[16];
extern uint8_t TxData_modbus_01[16];

extern uint8_t RxData_modbus_02[256];
extern int8_t TxData_modbus_02[256];

extern uint8_t RxData_modbus_03[256];
extern uint8_t TxData_modbus_03[256];

extern uint8_t RxData_modbus_04[256];
extern uint8_t TxData_modbus_04[256];

extern UART_HandleTypeDef huart1;

extern flag_1 ;


void tester_setup(void)
{
    if(RxData_modbus_01[0] == 0x7)  // Slave ID check
    {
    	flag_1 = 0 ;

        if(RxData_modbus_01[1] == 0x3)  // SET function (0x3)
        {
            uint8_t number_of_cells_and_temps = RxData_modbus_01[1];
            uint8_t number_of_status = RxData_modbus_01[2];
            uint8_t commnd = RxData_modbus_01[3];

            uint8_t id_LED = RxData_modbus_01[6];
            uint8_t LED_State = RxData_modbus_01[7];
            uint8_t time = RxData_modbus_01[8];
            uint8_t temp_set = RxData_modbus_01[9];
            uint8_t temp_get = RxData_modbus_01[10];
            uint8_t iso_spi_reading_cell = RxData_modbus_01[11];

            // Declare variables at function scope to avoid scope issues
            uint8_t id = 0;
            float volt = 0.0;  // Changed to float to handle decimal values
            uint8_t tempCardId = 0;
            uint8_t res = 0;

            switch (RxData_modbus_01[2])  // Function code from table
            {
                case 0x01:  // Voltage function (Range: 1-23, Value: 1-8)
                    id = RxData_modbus_01[3];  // Cell ID (1-23)
                    uint8_t volt_code = RxData_modbus_01[4];  // Voltage code (1-8)

                    // Validate range (1-23 for cell ID)
                    if (id >= 0 && id <= 23)
                    {
                        switch (volt_code)
                        {
                            case 0x01:
                                volt = 2.0;
                                break;
                            case 0x02:
                                volt = 2.5;
                                break;
                            case 0x03:
                                volt = 2.8;
                                break;
                            case 0x04:
                                volt = 3.3;
                                break;
                            case 0x05:
                                volt = 3.4;
                                break;
                            case 0x06:
                                volt = 3.6;
                                break;
                            case 0x07:
                                volt = 4.0;
                                break;
                            case 0x08:
                                volt = 4.2;
                                break;
                            default:
                                volt = 0.0;
                                break;
                        }
                        Set_Output_Voltage(id, volt);
                    }
                    break;

                case 0x02:  // Temperature function (Range: 1-6, Value: 0-256)
                    tempCardId = RxData_modbus_01[3];  // Temp card ID (1-6)
                    res = RxData_modbus_01[4];  // Resistance value (0-256)

                    // Validate range (1-6 for temp card ID)
                    if (tempCardId >= 1 && tempCardId <= 6)
                    {
                        Set_Resistance(tempCardId, res);
                        Set_LED_status(id_LED, LED_State);
                        HAL_Delay(1000);
                    }
                    break;

                case 0x03:  // Balance function (Range: 1-23, Value: 1/0)
                    id = RxData_modbus_01[3];  // Cell ID (1-23)
                    uint8_t balance_state = RxData_modbus_01[4];  // 1 = on, 0 = off

                    // Validate range and value
                    if (id >= 1 && id <= 23 && (balance_state == 0 || balance_state == 1))
                    {
                        Set_Balance_State(id, balance_state);
                    }
                    break;

                case 0x04:  // Open wire test (Range: 1-24, Value: 1/0)
                    id = RxData_modbus_01[3];  // Wire ID (1-24)
                    uint8_t test_enable = RxData_modbus_01[4];  // 1 = enable, 0 = disable

                    // Validate range and value
                    if (id >= 1 && id <= 24 && (test_enable == 0 || test_enable == 1))
                    {
                        Open_Wire_Test(id, test_enable);
                    }
                    break;

                case 0x05:  // Daisy chain function (Range: 1, Value: 1/0)
                    uint8_t chain_state = RxData_modbus_01[4];  // 1 = enable, 0 = disable

                    if (chain_state == 0 || chain_state == 1)
                    {
                        Set_Daisy_Chain(chain_state);
                    }
                    break;

                case 0x06:  // Delay function
                    uint16_t delay_time = (RxData_modbus_01[3] << 8) | RxData_modbus_01[4];  // Combine bytes for delay time
                    HAL_Delay(delay_time);
                    break;

                case 0x07:  // Cell LED function
                    id = RxData_modbus_01[3];  // LED ID
                    uint8_t led_state = RxData_modbus_01[4];  // LED state

                    Set_LED_status(id, led_state);
                    break;

                case 0x08:  // Automatic sequence function
                    uint8_t sequence_id = RxData_modbus_01[3];  // Sequence ID
                    uint8_t sequence_state = RxData_modbus_01[4];  // Sequence state

                    Run_Automatic_Sequence(sequence_id, sequence_state);
                    Voltage_Sequence_Automatic();
                    break;

                default:
                    // Handle unknown function code
                    break;
            }

            // setting_test_arrangement[]={};  // Fixed typo in comment
        }
        else if(RxData_modbus_01[1] == 0x4)  // GET function (0x4)
        {
        	flag_1 = 2 ;
            uint8_t function_code = RxData_modbus_01[2];
            uint8_t id = RxData_modbus_01[3];

            switch (function_code)
            {
                case 0x01:  // Cell-voltage (Range: 1-23, Value: 2.0-4.2V)
                    if (id >= 0 && id <= 23)
                    {
//                    	Voltage_Sequence_Automatic();
                    	float get_voltage = Get_INA_Voltage(&cell_configs[id]);
                    	uint16_t voltage_scaled = (uint16_t)(get_voltage * 10000); // Scale as needed

                    	TxData_modbus_01[0] = 0x07;  // slave address
                    	TxData_modbus_01[1] = voltage_scaled >> 8;

                    	TxData_modbus_01[2] = voltage_scaled & 0xFF;
                    	TxData_modbus_01[3] = 0;
                    	//The coil address will be 00000000 00000000 = 0 + 1 = 1

                    	TxData_modbus_01[4] = 0;  // force data high
                    	TxData_modbus_01[5] = 0;  // force data low

                    	uint16_t crc = crc16(TxData_modbus_01, 6);
                    	TxData_modbus_01[6] = crc&0xFF;   // CRC LOW
                    	TxData_modbus_01[7] = (crc>>8)&0xFF;  // CRC HIGH

                    	sendData(TxData_modbus_01,6);



                    }
                    break;

                case 0x02:  // Cell-temp (Range: 1-6, Value: -20 to 100°C)
                    if (id >= 0 && id <= 23)
                    {

						float get_Temp = Get_Cell_Temperature(&cell_configs[id]);
						uint16_t Temp_scaled = (uint16_t)(get_Temp * 10000); // Scale as needed

						TxData_modbus_01[0] = 0x07;  // slave address
						TxData_modbus_01[1] = Temp_scaled >> 8;

						TxData_modbus_01[2] = Temp_scaled & 0xFF;
						TxData_modbus_01[3] = 0;
													//The coil address will be 00000000 00000000 = 0 + 1 = 1

						TxData_modbus_01[4] = 0;  // force data high
						TxData_modbus_01[5] = 0;  // force data low

						uint16_t crc = crc16(TxData_modbus_01, 6);
						TxData_modbus_01[6] = crc&0xFF;   // CRC LOW
						TxData_modbus_01[7] = (crc>>8)&0xFF;  // CRC HIGH

						sendData(TxData_modbus_01,6);
                    }
                    break;

                case 0x03:  // Cell-current (Range: 1-23, Value: 1/0)
                    if (id >= 1 && id <= 23)
                    {


						float get_current = Get_Cell_Current(&cell_configs[id]);
						uint16_t Current_scaled = (uint16_t)(get_current * 10000); // Scale as needed

						TxData_modbus_01[0] = 0x07;  // slave address
						TxData_modbus_01[1] = Current_scaled >> 8;

						TxData_modbus_01[2] = Current_scaled & 0xFF;
						TxData_modbus_01[3] = 0;
													//The coil address will be 00000000 00000000 = 0 + 1 = 1

						TxData_modbus_01[4] = 0;  // force data high
						TxData_modbus_01[5] = 0;  // force data low

						uint16_t crc = crc16(TxData_modbus_01, 6);
						TxData_modbus_01[6] = crc&0xFF;   // CRC LOW
						TxData_modbus_01[7] = (crc>>8)&0xFF;  // CRC HIGH

						sendData(TxData_modbus_01,6);
                    }
                    break;

                case 0x04:  // Cell-Temp_res (Range: 1-24, Value: 1/0)
                    if (id >= 1 && id <= 24)
                    {
                        Get_Cell_Temp_Resistance(id);

						float get_Temp = Get_Cell_Temperature(&cell_configs[id]);
						uint16_t Temp_scaled = (uint16_t)(get_Temp * 10000); // Scale as needed

						TxData_modbus_01[0] = 0x07;  // slave address
						TxData_modbus_01[1] = Temp_scaled >> 8;

						TxData_modbus_01[2] = Temp_scaled & 0xFF;
						TxData_modbus_01[3] = 0;
													//The coil address will be 00000000 00000000 = 0 + 1 = 1

						TxData_modbus_01[4] = 0;  // force data high
						TxData_modbus_01[5] = 0;  // force data low

						uint16_t crc = crc16(TxData_modbus_01, 6);
						TxData_modbus_01[6] = crc&0xFF;   // CRC LOW
						TxData_modbus_01[7] = (crc>>8)&0xFF;  // CRC HIGH

						sendData(TxData_modbus_01,6);
                    }
                    break;

                case 0x05:  // DC-CSU-Volt (Range: 1-23, Value: 2.0-4.2V)
                    if (id >= 1 && id <= 23)
                    {
                        Get_DC_CSU_Voltage(id);

						float get_Temp = Get_Cell_Temperature(&cell_configs[id]);
						uint16_t Temp_scaled = (uint16_t)(get_Temp * 10000); // Scale as needed

						TxData_modbus_01[0] = 0x07;  // slave address
						TxData_modbus_01[1] = Temp_scaled >> 8;

						TxData_modbus_01[2] = Temp_scaled & 0xFF;
						TxData_modbus_01[3] = 0;
													//The coil address will be 00000000 00000000 = 0 + 1 = 1

						TxData_modbus_01[4] = 0;  // force data high
						TxData_modbus_01[5] = 0;  // force data low

						uint16_t crc = crc16(TxData_modbus_01, 6);
						TxData_modbus_01[6] = crc&0xFF;   // CRC LOW
						TxData_modbus_01[7] = (crc>>8)&0xFF;  // CRC HIGH

						sendData(TxData_modbus_01,6);
                    }
                    break;

                case 0x06:  // DC-CSU-Temp (Range: 1-6, Value: -20 to 100°C)
                    if (id >= 1 && id <= 6)
                    {
                        Get_DC_CSU_Temperature(id);
                    }
                    break;

                case 0x07:  // DC-CSU-Balance_reg (Range: 1-23, Value: 1/0)
                    if (id >= 1 && id <= 23)
                    {
                        Get_DC_CSU_Balance_Register(id);
                    }
                    break;

                case 0x08:  // DC-CSU-OW (Range: 1-24, Value: 1/0)
                    if (id >= 1 && id <= 24)
                    {
                        Get_DC_CSU_Open_Wire(id);
                    }
                    break;

                case 0x09:  // 11-CSU-Volt (Range: 1-23, Value: 2.0-4.2V)
                    if (id >= 1 && id <= 23)
                    {
                        Get_11_CSU_Voltage(id);
                    }
                    break;

                case 0x0A:  // 11-CSU-Temp (Range: 1-6, Value: -20 to 100°C)
                    if (id >= 1 && id <= 6)
                    {
                        Get_11_CSU_Temperature(id);
                    }
                    break;

                case 0x0B:  // 11-CSU-Balance_reg (Range: 1-23, Value: 1/0)
                    if (id >= 1 && id <= 23)
                    {
                        Get_11_CSU_Balance_Register(id);
                    }
                    break;

                case 0x0C:  // 11-CSU-OW (Range: 1-24, Value: 1/0)
                    if (id >= 1 && id <= 24)
                    {
                        Get_11_CSU_Open_Wire(id);
                    }
                    break;

                case 0x0D:  // 12-CSU-Volt (Range: 1-23, Value: 2.0-4.2V)
                    if (id >= 1 && id <= 23)
                    {
                        Get_12_CSU_Voltage(id);
                    }
                    break;

                case 0x0E:  // 12-CSU-Temp (Range: 1-6, Value: -20 to 100°C)
                    if (id >= 1 && id <= 6)
                    {
                        Get_12_CSU_Temperature(id);
                    }
                    break;

                case 0x0F:  // 12-CSU-Balance_reg (Range: 1-23, Value: 1/0)
                    if (id >= 1 && id <= 23)
                    {
                        Get_12_CSU_Balance_Register(id);
                    }
                    break;

                case 0x10:  // 12-CSU-OW (Range: 1-24, Value: 1/0)
                    if (id >= 1 && id <= 24)
                    {
                        Get_12_CSU_Open_Wire(id);
                    }
                    break;

                default:
                    // Handle unknown GET function code
                    break;
            }
        }
    }
    else
    {
        // Handle case when RxData_modbus_01[0] != 7
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
    INA229_Readings[cell->ina_index].temperature_C = INA229_getVBUS_V(ina);
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
    INA229_Readings[cell->ina_index].voltage_V = INA229_getDIETEMP_C(ina);
    HAL_Delay(1);
    HAL_GPIO_WritePin(cell->gpio, cell->cs_pin, GPIO_PIN_SET);

	Set_LED_status(cell->cell_id, ON);

	return INA229_Readings[cell->ina_index].voltage_V;
}

uint8_t Get_Cell_Temp_Resistance(uint8_t id)
{

}


float Get_Cell_Voltage(uint8_t id)
{

}
float Get_Cell_Temperature(uint8_t id)
{

}
float Get_Cell_Current(uint8_t id)
{

}


void Set_Balance_State(uint8_t id, uint8_t balance_state)
{

}
void Open_Wire_Test(uint8_t id, uint8_t test_enable)
{

}
void Set_Daisy_Chain(uint8_t chain_state)
{

}
void Run_Automatic_Sequence(uint8_t sequence_id, uint8_t sequence_state)
{

}

// GET functions - DC-CSU measurements
float Get_DC_CSU_Voltage(uint8_t id)
{

}
float Get_DC_CSU_Temperature(uint8_t id)
{

}
uint8_t Get_DC_CSU_Balance_Register(uint8_t id)
{

}
uint8_t Get_DC_CSU_Open_Wire(uint8_t id)
{

}

// GET functions - 11-CSU measurements
float Get_11_CSU_Voltage(uint8_t id)
{

}
float Get_11_CSU_Temperature(uint8_t id)
{

}
uint8_t Get_11_CSU_Balance_Register(uint8_t id)
{

}
uint8_t Get_11_CSU_Open_Wire(uint8_t id)
{

}

// GET functions - 12-CSU measurements
float Get_12_CSU_Voltage(uint8_t id)
{

}
float Get_12_CSU_Temperature(uint8_t id)
{

}
uint8_t Get_12_CSU_Balance_Register(uint8_t id)
{

}
uint8_t Get_12_CSU_Open_Wire(uint8_t id)
{

}
