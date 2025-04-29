/*
 * LTC68xx_API.c
 *
 *  Created on: Apr 29, 2025
 *      Author: Danushka Weerasinghe
 */

#include "LTC68xx_API.h"

extern SPI_HandleTypeDef hspi4;
//--------------------------------------------------------------//

// Configuration Constants

const uint8_t ADC_OPT = ADC_OPT_DISABLED; //!< ADC Mode option bit
const uint8_t ADC_CONVERSION_MODE = MD_7KHZ_3KHZ; //!< ADC ModeMD_27KHZ_14KHZ;/
const uint8_t ADC_DCP = DCP_DISABLED; //!< Discharge Permitted
const uint8_t CELL_CH_TO_CONVERT = CELL_CH_ALL; //!< Channel Selection for ADC conversion
const uint8_t AUX_CH_TO_CONVERT = AUX_CH_ALL; //!< Channel Selection for ADC conversion
const uint8_t STAT_CH_TO_CONVERT = STAT_CH_ALL; //!< Channel Selection for ADC conversion
const uint8_t SEL_ALL_REG = REG_ALL; //!< Register Selection
const uint8_t SEL_REG_A = REG_1; //!< Register Selection
const uint8_t SEL_REG_B = REG_2; //!< Register Selection
const uint16_t OV_THRESHOLD = 41000; //!< Over voltage threshold ADC Code. LSB = 0.0001 ---(4.1V)
const uint16_t UV_THRESHOLD = 30000; //!< Under voltage threshold ADC Code. LSB = 0.0001 ---(3V)

// Global Variables
int resistance[6] = {10, 10, 10, 10, 10, 10};
cell_asic BMS_IC[TOTAL_IC];
setup_data_sv setup_data;
uint8_t TxData[8];
uint32_t TxMailbox;

// Configuration Variables
unsigned char REF_ON = true; //!< Reference Powered Up Bit
unsigned char ADCOPT = false; //!< ADC Mode option bit
unsigned char GPIOBITS_A[5] = { false, false, true, true, true }; //!< GPIO Pin Control // Gpio 1,2,3,4,5
uint16_t UV = UV_THRESHOLD; //!< Under-voltage Comparison Voltage
uint16_t OV = OV_THRESHOLD; //!< Over-voltage Comparison Voltage
unsigned char DCCBITS_A[12] = { false, false, false, false, false, false, false, false,false, false, false, false }; //!< Discharge cell switch //Dcc 1,2,3,4,5,6,7,8,9,10,11,12
unsigned char DCTOBITS[4] = { true, false, true, false }; //!< Discharge time value // Dcto 0,1,2,3 // Programed for 4 min

/*
 * error data
 */
int8_t error_c = 0;
int8_t error_cell = 0;
int8_t error_aux = 0;
int8_t error_stat = 0;
int8_t adc_overlap_error = 0;
int8_t error_aux_dr = 0;
int8_t error_stat_dr = 0;
int16_t crc_error_count_gl = 0;


// Cell Balancing Variables
/*
 * Cell balancing data
 */
volatile uint8_t cell_balancing_enable = 0;
volatile uint8_t cell_balancing_enable_int = 0;
volatile uint8_t balancing_timer = 0;


/*
 * Highest & lowest data
 */

uint8_t highest_board_t = 0;
uint8_t highest_board_t_id = 0xFF;
uint8_t lowest_board_t = 0xFF;
uint8_t lowest_board_t_id = 0xFF;

int8_t highest_t = 0, lowest_t = 0xFF;
uint8_t highest_t_id = 0xFF;
uint8_t lowest_t_id = 0xFF;

uint16_t lowest_cell_id = 0xFFFF;
uint16_t highest_cell_id = 0xFFFF;
uint16_t lowest_cell_voltage = 0xFFFF;
uint16_t highest_cell_voltage = 0;

uint16_t total_pack_voltage = 0;

uint16_t globle_loweset_v = dCOV_THRESHOLD;

uint8_t env_board_t = 0;
uint8_t pwr_board_t = 0;




// Temperature Lookup Table
const signed char TemperatureDataTable[] = { 100, 98, 93, 91, 87, 85, 83,
		81, 80, 78, 77, 76, 74, 73, 72, 71, 70, 69, 68, 67, 67, 66, 65, 64, 63,
		63, 62, 61, 60, 60, 59, 58, 58, 57, 56, 56, 55, 54, 54, 53, 53, 52, 51,
		51, 50, 50, 49, 49, 48, 47, 47, 46, 46, 45, 45, 44, 44, 43, 43, 42, 42,
		41, 41, 40, 40, 39, 39, 38, 38, 37, 37, 36, 36, 36, 35, 35, 34, 34, 33,
		33, 32, 32, 32, 31, 31, 30, 30, 29, 29, 29, 28, 28, 27, 27, 27, 26, 26,
		25, 25, 24, 24, 24, 23, 23, 23, 22, 22, 21, 21, 21, 20, 20, 19, 19, 19,
		18, 18, 18, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 12, 12,
		12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5,
		4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, -1, -1, -1, -2, -2, -2, -3,
		-3, -3, -4, -4, -4, -5, -5, -5, -5, -6, -6, -6, -7, -7, -7, -8, -8, -8,
		-9, -9, -9, -10, -10, -10, -10, -11, -11, -11, -12, -12, -12, -13, -13,
		-13, -13, -14, -14, -14, -15, -15, -15, -16, -16, -16, -16, -25 };

void cs_low() {

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_SET);// Enable Pack, /CS asserted


}

void cs_high() {

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_SET);// Disable Pack, /CS deasserted


}

void delay_time_us(uint32_t time_delay_val) {
	uint32_t time_delay_count;
	uint32_t time_delay_val_temp = time_delay_val * 9;
	for (time_delay_count = 0; time_delay_count < time_delay_val_temp;
			time_delay_count++) {
		__NOP();
	}
}

/*
 * @brief Function Name:  spi_write_reg_analog .
 * @brief Description  :  Writes just 1 byte into the SPI BUS.
 * @param parameters   :  data
 * @return Value       :  return the data RX buffer
 */
uint8_t spi_write_reg_analog(uint8_t data_tx) {
	uint8_t data_rx = 0;
	if (HAL_SPI_TransmitReceive(&hspi4, &data_tx, &data_rx, 1, 20) != HAL_OK) {
		Error_Handler();
	}
	return data_rx;
}
/*
 * @brief Function Name:  spi_write_array
 * @brief Description  :  Writes an array of bytes out of the SPI port
 * @param parameters   :  Data length, data to transmit
 */

void spi_write_array(uint8_t len, // Option: Number of bytes to be written on the SPI port
		uint8_t data[] //Array of bytes to be written on the SPI port
		) {
	uint8_t len_t;
	for (len_t = 0; len_t < len; len_t++) {
		if (HAL_SPI_Transmit(&hspi4, &data[len_t], 1, 20) != HAL_OK) {
			/* Transfer error in transmission process */
			Error_Handler();
		}
	}

}
/*
 * @brief Function Name:  spi_write_read .
 * @brief Description  :  Writes and read a set number of bytes using the SPI port.
 * @param parameters   :  TX data array, TX data length, RX data pointer, number of bytes to be read from the SPI port
 * @return Value       :  return the data RX buffer
 */
uint8_t rx_len_local=0;
HAL_StatusTypeDef status_spi_out = HAL_OK;
HAL_StatusTypeDef status_spi_in = HAL_OK;

void spi_write_read(uint8_t tx_Data[], //array of data to be written on SPI port
		uint8_t tx_len, //length of the tx data array
		uint8_t *rx_data, //Input: array that will store the data read by the SPI port
		uint8_t rx_len //Option: number of bytes to be read from the SPI port
		) {
	status_spi_out = HAL_SPI_Transmit(&hspi4, tx_Data, tx_len, 20);
	rx_len_local = rx_len;
	if (status_spi_out != HAL_OK) {

		Error_Handler();
	}
	status_spi_in = HAL_SPI_Receive(&hspi4, rx_data, rx_len, 20);
	if (status_spi_in != HAL_OK) {

		Error_Handler();
	}

}

/*
 * @brief Function Name:  spi_read_byte .
 * @brief Description  :  Read just 1 byte using the SPI port
 * @param parameters   :  TX data array, TX data length, RX data pointer, number of bytes to be read from the SPI port
 * @return Value       :  return the data RX buffer
 */

uint8_t spi_read_byte(uint8_t tx_dat) {
	uint8_t data;
	if (HAL_SPI_TransmitReceive(&hspi4, &tx_dat, &data, 1, 20) != HAL_OK) {
		Error_Handler();
	}
	return (data);
}

//---------------------------------------------------------------------------------------------------------------------------//

/**
 * @brief Function Name: temparature_data_read.
 * @brief Description  : Read all LTC6811-1 temperature sensors
 */

void temparature_data_read()
{
   LTC6811_clraux();
   int8_t error = 0;
   int32_t time = 0;
   uint8_t i_1;
   uint32_t conv_time = 0;
   //-----------------------
   wakeup_sleep(TOTAL_IC);
   LTC6811_adax(ADC_CONVERSION_MODE, AUX_CH_ALL);
   conv_time = LTC6811_pollAdc();
   time += conv_time / 1000;
   //-----------------------
   wakeup_sleep(TOTAL_IC);
   error = LTC6811_rdaux(SEL_ALL_REG, TOTAL_IC, BMS_IC); // Set to read back all aux registers

   if (error == -1)
   {
       for (i_1 = 0; i_1 < TEMP_PER_IC; i_1++)
       {
           BMS_IC[0].heat.temp[i_1] = TemperatureDataTable[123];
           BMS_IC[1].heat.temp[i_1] = TemperatureDataTable[123];
       }
   }
   else
   {
       for (i_1 = 0; i_1 < TEMP_PER_IC; i_1++)
       {
           BMS_IC[0].heat.temp[i_1] = tempDataTableRead(
                   BMS_IC[0].aux.a_codes[i_1]);
           BMS_IC[1].heat.temp[i_1] = tempDataTableRead(
                               BMS_IC[1].aux.a_codes[i_1]);
       }
   }
}

/*
 * @Function : tempDataTableRead
 * @Description : per calculated temperature values for voltage readings
 */
signed char tempDataTableRead(uint16_t tempe_v_in)
{
    signed char temperature_val_tb;
    int temp_val_tp;
    temp_val_tp = (tempe_v_in >> 7) - 0x12;//0x30;
    if (temp_val_tp < 0)
    {
        temperature_val_tb = 105;
    }
    else if (temp_val_tp > 222)
    {
        temperature_val_tb = -25;
    }
    else
    {
        temperature_val_tb = TemperatureDataTable[temp_val_tp];
    }
    return temperature_val_tb;
}

int8_t cell_voltage_read(void) {
	int8_t error = 0;
	wakeup_sleep(TOTAL_IC);
	LTC6811_adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
	 LTC6811_pollAdc();
	wakeup_sleep(TOTAL_IC);
	error = LTC6811_rdcv(SEL_ALL_REG, TOTAL_IC, BMS_IC);
//	check_error(error);
	uint8_t i_1, current_ic_1;
	if (error == -1) {
		for (current_ic_1 = 0; current_ic_1 < TOTAL_IC; current_ic_1++) {
			for (i_1 = 0; i_1 < BMS_IC[0].ic_reg.cell_channels; i_1++) {
				BMS_IC[current_ic_1].cells.c_codes[i_1] = 0;
			}
		}
	}
	//cell_data_print(error, conv_time);
	return error;
}

/**
 * @brief Function Name: aux_data_read.
 * @brief Description  : Read all LTC6811-1 auxiliary voltages
 */
uint8_t aux_data_read(void) {
	int8_t error = 0;
	uint8_t i_1, current_ic_1;
	wakeup_sleep(TOTAL_IC);
	LTC6811_adax(ADC_CONVERSION_MODE, AUX_CH_TO_CONVERT);
	LTC6811_pollAdc();
	//----------------------------
	wakeup_sleep(TOTAL_IC);
	error = LTC6811_rdaux(SEL_ALL_REG, TOTAL_IC, BMS_IC); // Set to read back all aux registers
	if (error == -1) {

		for (current_ic_1 = 0; current_ic_1 < TOTAL_IC; current_ic_1++) {
			for (i_1 = 0; i_1 < 6; i_1++) {
				BMS_IC[current_ic_1].aux.a_codes[i_1] = 0xFFFF;
			}
		}
	}
	return error;
}

/**
 * @brief Function Name: stat_data_read.
 * @brief Description  : Read all LTC6811-1 cell status register data
 */
int8_t stat_data_read(void) {
	int8_t error = 0;
	wakeup_sleep(TOTAL_IC);
	LTC6811_adstat(ADC_CONVERSION_MODE, STAT_CH_TO_CONVERT);
	LTC6811_pollAdc();
	//----------------------------
	wakeup_sleep(TOTAL_IC);
	error = LTC6811_rdstat(SEL_ALL_REG, TOTAL_IC, BMS_IC); // Set to read back all stat registers
	return (error);
}

/*
 * @brief Function Name : adc_test_data_read
 * @brief Description	: ADC test whether is there ADC error
 */
int8_t adc_test_data_read() {
	wakeup_sleep(TOTAL_IC);
	error_cell = LTC6811_run_cell_adc_st(CELL, TOTAL_IC, BMS_IC,
			ADC_CONVERSION_MODE, ADCOPT);

	wakeup_sleep(TOTAL_IC);
	error_aux = LTC6811_run_cell_adc_st(AUX, TOTAL_IC, BMS_IC,
			ADC_CONVERSION_MODE, ADCOPT);

	wakeup_sleep(TOTAL_IC);
	error_stat = LTC6811_run_cell_adc_st(STAT, TOTAL_IC, BMS_IC,
			ADC_CONVERSION_MODE, ADCOPT);
	return (error_cell + error_aux + error_stat);
}

/*
 * @brief Function Name : adc_overlap_data_read
 * @brief Description	: return the number of errors in overlap test
 * 							if errors == 0 then pass
 */
int8_t adc_overlap_data_read() {
	int8_t error = 0;
	wakeup_sleep(TOTAL_IC);
	error = (int8_t) LTC6811_run_adc_overlap(TOTAL_IC, BMS_IC);
	return (error);
}

/*
 * @brief Function Name : digital_redundancy_check
 * @brief Description	: ADC redundancy check
 */
int8_t digital_redundancy_check() {
	wakeup_sleep(TOTAL_IC);
	error_aux_dr = LTC6811_run_adc_redundancy_st(ADC_CONVERSION_MODE, AUX,
	TOTAL_IC, BMS_IC);
	wakeup_sleep(TOTAL_IC);
	error_stat_dr = LTC6811_run_adc_redundancy_st(ADC_CONVERSION_MODE, STAT,
	TOTAL_IC, BMS_IC);
	return (error_aux_dr + error_stat_dr);
}

/*
 * @brief Function Name : crc_error_read
 * @brief Description	: return the number of errors in CRC check
 */
uint16_t crc_error_read() {
	uint16_t error_count = 0;
	uint8_t current_ic = 0;
	for (current_ic = 0; current_ic < TOTAL_IC; current_ic++) {
		error_count += BMS_IC[current_ic].crc_count.pec_count;
	}
	return (error_count);
}

/*
 * @brief Function Name : open_wire_check
 * @brief Description	: return the open wire connection in cells
 */
void open_wire_check() {
	wakeup_sleep(TOTAL_IC);
	LTC681x_run_openwire_multi(TOTAL_IC, BMS_IC);
}

/*
 * @brief Function Name : open_wire_gpio_check
 * @brief Description	: return the open wire connection in GPIO
 */
void open_wire_gpio_check() {
	wakeup_sleep(TOTAL_IC);
	LTC681x_run_gpio_openwire(TOTAL_IC, BMS_IC);
}

/*
 *  ____________________________________________________________________________
 * |REG  |RD/WR |BIT 7  |BIT 6  |BIT 5  |BIT 4  |BIT 3  |BIT 2  |BIT 1  |BIT 0  |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 * |CFGR0|RD/WR |GPIO5  |GPIO4  |GPIO3  |GPIO2  |GPIO1  |REFON  | DTEN  |ADCOPT |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 * |CFGR1|RD/WR |VUV[7] |VUV[6] |VUV[5] |VUV[4] |VUV[3] |VUV[2] |VUV[1] |VUV[0] |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 * |CFGR2|RD/WR |VOV[3] |VOV[2] |VOV[1] |VOV[0] |VUV[11]|VUV[10]|VUV[9] |VUV[8] |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 * |CFGR3|RD/WR |VOV[11]|VOV[10]|VOV[9] |VOV[8] |VOV[7] |VOV[6] |VOV[5] |VOV[4] |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 * |CFGR4|RD/WR |DCC8   |DCC7   |DCC6   |DCC5   |DCC4   |DCC3   |DCC2   |DCC1   |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 * |CFGR5|RD/WR |DCTO[3]|DCTO[2]|DCTO[1]|DCTO[0]|DCC12  |DCC11  |DCC10  |DCC9   |
 * |_____|______|_______|_______|_______|_______|_______|_______|_______|_______|
 */

int8_t config_data_read() {
	int8_t error = 0;
	wakeup_sleep(TOTAL_IC);
	error = LTC6811_rdcfg(TOTAL_IC, BMS_IC);
	return (error);
}

/*
 * BMS_IC[current_ic].pwm.rx_data[i] >> i = 0 to 5
 * 4 bits represent 1 cell duty cycle
 * F is 100%
 */
void pwm_reg_read() {
	wakeup_idle(TOTAL_IC);
	LTC6811_rdpwm(TOTAL_IC, 0, BMS_IC);
}

/*
 * S CTRL register - used for serial pulsing through s pins
 * 4 BITs - 0000 for de-asserted
 *          1xxx for asserted
 *          others - pulses according to the number represent
 * BMS_IC[current_ic].sctrl.rx_data[i] >> i 0 to 5
 */
int8_t s_control_reg_read() {
	uint8_t streg = 0;
	int8_t error = 0;
	wakeup_idle(TOTAL_IC);
	error = LTC6811_rdsctrl(TOTAL_IC, streg, BMS_IC);

	return (error);
}

/*
 * Cell balancing
 */
#define  CELL_BALANCING_EN
#define  cell_bal_algo_2
#ifdef CELL_BALANCING_EN
void cell_balance_LTC() {
	int discharge_cells = 0;
	uint8_t local_ic_dis = 0;
	uint8_t local_cell_dis = 0;
	if (cell_balancing_enable) {
#ifdef cell_bal_algo_1
        for (local_ic_dis = 0; local_ic_dis < TOTAL_IC; local_ic_dis++)
        {
            discharge_cells = 0;
            if (highest_cell_voltage - lowest_cell_voltage
                    >= setup_data.balancing_max_vgap)
            {
                for (local_cell_dis = 0;
                        local_cell_dis < BMS_IC[local_ic_dis].ic_reg.cell_channels;
                        local_cell_dis++)
                {

                    if ((highest_cell_voltage
                            - BMS_IC[local_ic_dis].cells.c_codes[local_cell_dis])
                            < /*setup_data.balancing_max_vgap*/dCELL_BALANCING_THRESHOLD)
                    {
                        discharge_cells = discharge_cells
                                | (1 << local_cell_dis);
                    }
                }
            }
            LTC6811_set_discharge(discharge_cells, local_ic_dis, BMS_IC);
        }
        wakeup_sleep(TOTAL_IC);
        LTC6811_wrcfg(TOTAL_IC, BMS_IC);
#endif
#ifdef cell_bal_algo_2
		for (local_ic_dis = 0; local_ic_dis < TOTAL_IC; local_ic_dis++) {
			for (local_cell_dis = 0;
					local_cell_dis < BMS_IC[0].ic_reg.cell_channels;
					local_cell_dis++) {
				if (BMS_IC[local_ic_dis].cells.c_codes[local_cell_dis]
						- lowest_cell_voltage > setup_data.balancing_max_vgap) {
					discharge_cells = discharge_cells
							| (0x01 << local_cell_dis);
				}
			}
			LTC6811_set_discharge(discharge_cells, local_ic_dis, BMS_IC);
		}
		wakeup_sleep(TOTAL_IC);
		LTC6811_wrcfg(TOTAL_IC, BMS_IC);
#endif
	} else {
		for (local_ic_dis = 0; local_ic_dis < TOTAL_IC; local_ic_dis++) {
			LTC6811_set_discharge(0, local_ic_dis, BMS_IC);
		}
		wakeup_sleep(TOTAL_IC);
		LTC6811_wrcfg(TOTAL_IC, BMS_IC);
	}
}
#endif

/*
 * Function name : analyze_cell_data
 * Description : # sort highest and lowest variables
 *               # sort according cell positions and temperature
 *                 sensor positions
 *      Temperature id start with 1
 *      Cell id start with 1
 *      temperature initialize value - (-25C)
 */
void analyze_cell_data()
{
    uint8_t current_ic_1 = 0;
    uint8_t current_cell_1 = 0;
    uint8_t current_temp_1 = 0;
    highest_cell_voltage = 0;
    lowest_cell_voltage = 0xFFFF;
    lowest_cell_id = 0;
    highest_cell_id = 0;
    total_pack_voltage = 0;

    highest_t = TemperatureDataTable[123];
    lowest_t = TemperatureDataTable[0];
    highest_t_id = 0;
    lowest_t_id = 0;

    highest_board_t = 0;
    highest_board_t_id = 0xFF;
    lowest_board_t = 0xFF;
    lowest_board_t_id = 0xFF;
//highest and lowest cell voltages
    for (current_ic_1 = 0; current_ic_1 < TOTAL_IC; current_ic_1++)
    {
        BMS_IC[current_ic_1].cells.max_cell_v_local = 0;
        BMS_IC[current_ic_1].cells.min_cell_v_local = 0xFFFF;
        BMS_IC[current_ic_1].cells.max_cell_id_local = 0;
        BMS_IC[current_ic_1].cells.min_cell_id_local = 0;

        BMS_IC[current_ic_1].heat.highest_t_local = TemperatureDataTable[123];
        BMS_IC[current_ic_1].heat.lowest_t_local = TemperatureDataTable[0];
        BMS_IC[current_ic_1].heat.highest_t_id_local = 0;
        BMS_IC[current_ic_1].heat.lowest_t_id_local = 0;

        for (current_cell_1 = 0; current_cell_1 < CELLS_PER_IC;
                current_cell_1++)
        {

            if (BMS_IC[current_ic_1].cells.max_cell_v_local
                    < BMS_IC[current_ic_1].cells.c_codes[current_cell_1])
            {
                BMS_IC[current_ic_1].cells.max_cell_v_local =
                        BMS_IC[current_ic_1].cells.c_codes[current_cell_1];
                BMS_IC[current_ic_1].cells.max_cell_id_local = current_cell_1;
            }
            //sorting for lowest voltage cell
            if (BMS_IC[current_ic_1].cells.min_cell_v_local
                    > BMS_IC[current_ic_1].cells.c_codes[current_cell_1])
            {
                BMS_IC[current_ic_1].cells.min_cell_v_local =
                        BMS_IC[current_ic_1].cells.c_codes[current_cell_1];
                BMS_IC[current_ic_1].cells.min_cell_id_local = current_cell_1;
            }
        }
        //temperature
        for (current_temp_1 = 0; current_temp_1 < TEMP_PER_IC; current_temp_1++)
        {
            if (BMS_IC[current_ic_1].heat.highest_t_local
                    < BMS_IC[current_ic_1].heat.temp[current_temp_1])
            {
                BMS_IC[current_ic_1].heat.highest_t_local =
                        BMS_IC[current_ic_1].heat.temp[current_temp_1];
                BMS_IC[current_ic_1].heat.highest_t_id_local = current_temp_1;
            }
            if (BMS_IC[current_ic_1].heat.lowest_t_local
                    > BMS_IC[current_ic_1].heat.temp[current_temp_1])
            {
                BMS_IC[current_ic_1].heat.lowest_t_local =
                        BMS_IC[current_ic_1].heat.temp[current_temp_1];
                BMS_IC[current_ic_1].heat.lowest_t_id_local = current_temp_1;
            }
        }
    }
    highest_t = BMS_IC[0].heat.highest_t_local;
    highest_t_id = BMS_IC[0].heat.highest_t_id_local + 1;
    lowest_t = BMS_IC[0].heat.lowest_t_local;
    lowest_t_id = BMS_IC[0].heat.lowest_t_id_local + 1;
	total_pack_voltage = BMS_IC[0].stat.stat_codes[0];
}

/*
 * Function name 	: make_data_packet(uint8_t data_buffer_local[])
 * Description 		: Make USART data packet
 *
 *
 */
/*
 *  ___________________________________________________
 * |Item |Content                            |hex bytes|
 * |_____|___________________________________|_________|
 * |0    |Start           0x55               |1        |
 * |_____|___________________________________|_________|
 * |1    |Function code   0x46               |1        |
 * |_____|___________________________________|_________|
 * |2    |Length          0x46               |1        |
 * |_____|___________________________________|_________|
 * |3    |Cell quantity                      |1        |
 * |_____|___________________________________|_________|
 * |4    |Voltage value of the 1st cell (mV) |2        |
 * |     |___________________________________|_________|
 * |     |Voltage value of the 2nd cell (mV) |2        |
 * |     |___________________________________|_________|
 * |     |...                                |2        |
 * |     |___________________________________|_________|
 * |     |Voltage value of the Mth cell (mV) |2        |
 * |_____|___________________________________|_________|
 * |5    |Temperature value quantity N=6     |1        |
 * |_____|___________________________________|_________|
 * |6    |Temperature of the 1st cell(0.1C)  |2        |
 * |     |___________________________________|_________|
 * |     |Temperature of the 2nd cell (0.1C) |2        |
 * |     |___________________________________|_________|
 * |     |Temperature of the 3rd cell (0.1C) |2        |
 * |     |___________________________________|_________|
 * |     |Temperature of the 4th cell (0.1C) |2        |
 * |     |___________________________________|_________|
 * |     |Ambient temperature (0.1C)         |2        |
 * |     |___________________________________|_________|
 * |     |BMS temperature (0.1C)             |2        |
 * |_____|___________________________________|_________|
 * |7    |Charge and discharge current(0.01A)|2        |
 * |_____|___________________________________|_________|
 * |8    |Total voltage (0.01V)              |2        |
 * |_____|___________________________________|_________|
 * |9    |Remaining capacity (0.01Ah)        |2        |
 * |_____|___________________________________|_________|
 * |10   |Customize P=10                     |1        |
 * |_____|___________________________________|_________|
 * |11   |Battery capacity (0.01Ah)          |2        |
 * |_____|___________________________________|_________|
 * |12   |Cycle life                         |2        |
 * |_____|___________________________________|_________|
 * |13   |Voltage of the port(0.01V)         |2        |
 * |_____|___________________________________|_________|
 * |14   |Discharge                          |0        |
 * |     |___________________________________|_________|
 * |     |Charge                             |1        |
 * |     |___________________________________|_________|
 * |     |Float charge                       |2        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |3        |
 * |     |___________________________________|_________|
 * |     |Standby                            |4        |
 * |     |___________________________________|_________|
 * |     |Power off                          |5        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |6        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |7        |
 * |_____|___________________________________|_________|
 * |15   |Discharge on/off status            |0        |
 * |     |___________________________________|_________|
 * |     |Charge on/off status               |1        |
 * |     |___________________________________|_________|
 * |     |Current limiting on/off status     |2        |
 * |     |___________________________________|_________|
 * |     |Heating on/off status              |3        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |4        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |5        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |6        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |7        |
 * |_____|___________________________________|_________|
 * |16   |Warning status 1                   |0        |
 * |     |___________________________________|_________|
 * |     |Warning status 2                   |1        |
 * |     |___________________________________|_________|
 * |     |Warning status 3                   |2        |
 * |     |___________________________________|_________|
 * |     |Warning status 4                   |3        |
 * |     |___________________________________|_________|
 * |     |Warning status 5                   |4        |
 * |     |___________________________________|_________|
 * |     |Warning status 6                   |5        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |6        |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |7        |
 * |_____|___________________________________|_________|
 * |17   |Voltage sensor failure             |0        |
 * |     |___________________________________|_________|
 * |     |Temperature sensor failure         |1        |
 * |     |___________________________________|_________|
 * |     |Current sensor failure             |2        |
 * |     |___________________________________|_________|
 * |     |Button failure                     |3        |
 * |     |___________________________________|_________|
 * |     |Voltage difference failure         |4        |
 * |     |___________________________________|_________|
 * |     |Charge switch failure              |5        |
 * |     |___________________________________|_________|
 * |     |Discharge switch failure           |6        |
 * |     |___________________________________|_________|
 * |     |Current limiting failure           |7        |
 * |_____|___________________________________|_________|
 * |18   |Over V warning of individual cell  |0 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over V trip of individual cell     |1 1-bit  |
 * |     |___________________________________|_________|
 * |     |Low V warning of individual cell   |2 1-bit  |
 * |     |___________________________________|_________|
 * |     |Low V trip of individual cell      |3 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over V warning of pack voltage     |4 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over V trip of pack voltage        |5 1-bit  |
 * |     |___________________________________|_________|
 * |     |Low V warning of pack voltage      |6 1-bit  |
 * |     |___________________________________|_________|
 * |     |Low V trip of pack voltage         |7 1-bit  |
 * |_____|___________________________________|_________|
 * |19   |Charge high temperature warning    |0 1-bit  |
 * |     |___________________________________|_________|
 * |     |Charge high temperature trip       |1 1-bit  |
 * |     |___________________________________|_________|
 * |     |Charge low temperature warning     |2 1-bit  |
 * |     |___________________________________|_________|
 * |     |Charge low temperature trip        |3 1-bit  |
 * |     |___________________________________|_________|
 * |     |Discharge high temperature warning |4 1-bit  |
 * |     |___________________________________|_________|
 * |     |Discharge high temperature trip    |5 1-bit  |
 * |     |___________________________________|_________|
 * |     |Discharge low temperature warning  |6 1-bit  |
 * |     |___________________________________|_________|
 * |     |Discharge low temperature trip     |7 1-bit  |
 * |_____|___________________________________|_________|
 * |20   |Ambient high temperature warning   |0 1-bit  |
 * |     |___________________________________|_________|
 * |     |Ambient high temperature trip      |1 1-bit  |
 * |     |___________________________________|_________|
 * |     |Ambient low temperature warning    |2 1-bit  |
 * |     |___________________________________|_________|
 * |     |Ambient low temperature trip       |3 1-bit  |
 * |     |___________________________________|_________|
 * |     |High temperature warning (PCB)     |4 1-bit  |
 * |     |___________________________________|_________|
 * |     |High temperature trip (PCB)        |5 1-bit  |
 * |     |___________________________________|_________|
 * |     |Heating                            |6 1-bit  |
 * |     |___________________________________|_________|
 * |     |Reserved bits                      |7 1-bit  |
 * |_____|___________________________________|_________|
 * |21   |Over current warning (Charge)      |0 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over current trip (Charge)         |1 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over current warning (Discharge)   |2 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over current trip (Discharge)      |3 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over current trip (Transient)      |4 1-bit  |
 * |     |___________________________________|_________|
 * |     |Short circuit trip (Output current)|5 1-bit  |
 * |     |___________________________________|_________|
 * |     |Over current lock (Transient)      |6 1-bit  |
 * |     |___________________________________|_________|
 * |     |Short circuit lock (Output current)|7 1-bit  |
 * |_____|___________________________________|_________|
 * |22   |High voltage trip (Charge)         |0 1-bit  |
 * |     |___________________________________|_________|
 * |     |Intermittent power supply waiting  |1 1-bit  |
 * |     |___________________________________|_________|
 * |     |Remaining capacity warning         |2 1-bit  |
 * |     |___________________________________|_________|
 * |     |Remaining capacity trip            |3 1-bit  |
 * |     |___________________________________|_________|
 * |     |Stop charging warning of low cell  |4 1-bit  |
 * |     |__voltage__________________________|_________|
 * |     |Reverse polarity protection(Output)|5 1-bit  |
 * |     |___________________________________|_________|
 * |     |Output connection failure          |6 1-bit  |
 * |     |___________________________________|_________|
 * |     |Internal bit                       |7 1-bit  |
 * |_____|___________________________________|_________|
 * |23   |Reserved                           |1        |
 * |_____|___________________________________|_________|
 * |24   |Reserved                           |1        |
 * |_____|___________________________________|_________|
 * |25   |CRC                                |2        |
 * |_____|___________________________________|_________|
 * |26   |END                                |1        |
 * |_____|___________________________________|_________|
 *
 */

//void make_data_packet(uint8_t data_buffer_local[]){
//	data_buffer_local[0] = 0x55;
//	data_buffer_local[1] = 0x46;
//	data_buffer_local[2] = 0x46;
//	//Cells voltage measurement
//	data_buffer_local[3] = CELLS_PER_BMS;
//	data_buffer_local[4] = (BMS_IC[0].cells.c_codes[0]/10)>>8;
//	data_buffer_local[5] = (BMS_IC[0].cells.c_codes[0]/10) & 0xFF;
//	data_buffer_local[6] = (BMS_IC[0].cells.c_codes[1]/10)>>8;
//	data_buffer_local[7] = (BMS_IC[0].cells.c_codes[1]/10) & 0xFF;
//	data_buffer_local[8] = (BMS_IC[0].cells.c_codes[2]/10)>>8;
//	data_buffer_local[9] = (BMS_IC[0].cells.c_codes[2]/10) & 0xFF;
//	data_buffer_local[10] = (BMS_IC[0].cells.c_codes[3]/10)>>8;
//	data_buffer_local[11] = (BMS_IC[0].cells.c_codes[3]/10) & 0xFF;
//	data_buffer_local[12] = (BMS_IC[0].cells.c_codes[4]/10)>>8;
//	data_buffer_local[13] = (BMS_IC[0].cells.c_codes[4]/10) & 0xFF;
//	data_buffer_local[14] = (BMS_IC[0].cells.c_codes[5]/10)>>8;
//	data_buffer_local[15] = (BMS_IC[0].cells.c_codes[5]/10) & 0xFF;
//	data_buffer_local[16] = (BMS_IC[0].cells.c_codes[6]/10)>>8;
//	data_buffer_local[17] = (BMS_IC[0].cells.c_codes[6]/10) & 0xFF;
//	data_buffer_local[18] = (BMS_IC[0].cells.c_codes[7]/10)>>8;
//	data_buffer_local[19] = (BMS_IC[0].cells.c_codes[7]/10) & 0xFF;
//	data_buffer_local[20] = (BMS_IC[0].cells.c_codes[8]/10)>>8;
//	data_buffer_local[21] = (BMS_IC[0].cells.c_codes[8]/10) & 0xFF;
//	data_buffer_local[22] = (BMS_IC[0].cells.c_codes[9]/10)>>8;
//	data_buffer_local[23] = (BMS_IC[0].cells.c_codes[9]/10) & 0xFF;
//	data_buffer_local[24] = (BMS_IC[0].cells.c_codes[10]/10)>>8;
//	data_buffer_local[25] = (BMS_IC[0].cells.c_codes[10]/10) & 0xFF;
//	data_buffer_local[26] = (BMS_IC[0].cells.c_codes[12]/10)>>8;
//	data_buffer_local[27] = (BMS_IC[0].cells.c_codes[12]/10) & 0xFF;
//	data_buffer_local[28] = (BMS_IC[0].cells.c_codes[13]/10)>>8;
//	data_buffer_local[29] = (BMS_IC[0].cells.c_codes[13]/10) & 0xFF;
//	data_buffer_local[30] = (BMS_IC[0].cells.c_codes[14]/10)>>8;
//	data_buffer_local[31] = (BMS_IC[0].cells.c_codes[14]/10) & 0xFF;
//	data_buffer_local[32] = (BMS_IC[0].cells.c_codes[15]/10)>>8;
//	data_buffer_local[33] = (BMS_IC[0].cells.c_codes[15]/10) & 0xFF;
//	data_buffer_local[34] = (BMS_IC[0].cells.c_codes[16]/10)>>8;
//	data_buffer_local[35] = (BMS_IC[0].cells.c_codes[16]/10) & 0xFF;
//	//Temperatures measurement
//	/*
//	 * Unsigned integer (0.1K), actual value = (transmission value-2731)/10 (℃).
//	 * For example: 3032 means (3032-2731)/10 (℃)=30.1℃
//	 */
//	data_buffer_local[36] = TEMPS_PER_BMS;
//	data_buffer_local[37] = BMS_IC[0].heat.temp[0]>>8;
//	data_buffer_local[38] = BMS_IC[0].heat.temp[0]& 0xFF;
//	data_buffer_local[39] = BMS_IC[0].heat.temp[1]>>8;
//	data_buffer_local[40] = BMS_IC[0].heat.temp[1]& 0xFF;
//	data_buffer_local[41] = BMS_IC[0].heat.temp[2]>>8;
//	data_buffer_local[42] = BMS_IC[0].heat.temp[2]& 0xFF;
//	data_buffer_local[43] = BMS_IC[0].heat.temp[3]>>8;
//	data_buffer_local[44] = BMS_IC[0].heat.temp[3]& 0xFF;
//	data_buffer_local[45] = BMS_IC[0].heat.temp[4]>>8;
//	data_buffer_local[46] = BMS_IC[0].heat.temp[4]& 0xFF;
//	data_buffer_local[41] = BMS_IC[0].heat.temp[5]>>8;
//	data_buffer_local[42] = BMS_IC[0].heat.temp[5]& 0xFF;
//	//Current
////	data_buffer_local[42] = BMS_IC;
//}
//



//--------------------------------------------------------------//
