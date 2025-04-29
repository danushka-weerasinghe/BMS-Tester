/*
 * LTC68xx_API.h
 *
 *  Created on: Apr 29, 2025
 *      Author: Danushka Weerasinghe
 */

#ifndef ISO_SPI_INC_LTC68XX_API_H_
#define ISO_SPI_INC_LTC68XX_API_H_


#include "main.h"
#include "flash_LTC.h"
#include "LTC6811.h"
#include "LTC681x.h"
#include <stdint.h>

// Configuration Constants

extern const uint8_t ADC_OPT ; //!< ADC Mode option bit
extern const uint8_t ADC_CONVERSION_MODE ; //!< ADC ModeMD_27KHZ_14KHZ;/
extern const uint8_t ADC_DCP ; //!< Discharge Permitted
extern const uint8_t CELL_CH_TO_CONVERT ; //!< Channel Selection for ADC conversion
extern const uint8_t AUX_CH_TO_CONVERT ; //!< Channel Selection for ADC conversion
extern const uint8_t STAT_CH_TO_CONVERT ; //!< Channel Selection for ADC conversion
extern const uint8_t SEL_ALL_REG ; //!< Register Selection
extern const uint8_t SEL_REG_A; //!< Register Selection
extern const uint8_t SEL_REG_B ; //!< Register Selection
extern const uint16_t OV_THRESHOLD ; //!< Over voltage threshold ADC Code. LSB = 0.0001 ---(4.1V)
extern const uint16_t UV_THRESHOLD ; //!< Under voltage threshold ADC Code. LSB = 0.0001 ---(3V)

// Global Variables

//temp data
extern int resistance[6];
//iso spi data
extern cell_asic BMS_IC[TOTAL_IC];
extern setup_data_sv setup_data;
extern uint8_t TxData[8];
extern uint32_t TxMailbox;

// Configuration Variables
extern unsigned char REF_ON ; //!< Reference Powered Up Bit
extern unsigned char ADCOPT ; //!< ADC Mode option bit
extern unsigned char GPIOBITS_A[5] ; //!< GPIO Pin Control // Gpio 1,2,3,4,5
extern uint16_t UV ; //!< Under-voltage Comparison Voltage
extern uint16_t OV ; //!< Over-voltage Comparison Voltage
extern unsigned char DCCBITS_A[12] ; //!< Discharge cell switch //Dcc 1,2,3,4,5,6,7,8,9,10,11,12
extern unsigned char DCTOBITS[4] ; //!< Discharge time value // Dcto 0,1,2,3 // Programed for 4 min


// Error Flags

/*
 * error data
 */

extern int8_t error_c;
extern int8_t error_cell;
extern int8_t error_aux;
extern int8_t error_stat;
extern int8_t adc_overlap_error;
extern int8_t error_aux_dr;
extern int8_t error_stat_dr;
extern int16_t crc_error_count_gl;


// Cell Balancing Variables
extern volatile uint8_t cell_balancing_enable;
extern volatile uint8_t cell_balancing_enable_int;
extern volatile uint8_t balancing_timer;


// Temperature and Voltage Tracking

/*
 * Highest & lowest data
 */
extern uint8_t highest_board_t;
extern uint8_t highest_board_t_id;
extern uint8_t lowest_board_t;
extern uint8_t lowest_board_t_id;
extern int8_t highest_t;
extern int8_t lowest_t;
extern uint8_t highest_t_id;
extern uint8_t lowest_t_id;
extern uint16_t lowest_cell_id;
extern uint16_t highest_cell_id;
extern uint16_t lowest_cell_voltage;
extern uint16_t highest_cell_voltage;
extern uint16_t total_pack_voltage;
extern uint16_t globle_loweset_v;
extern uint8_t env_board_t;
extern uint8_t pwr_board_t;

/* Temperature Data */
extern const signed char TemperatureDataTable[];

// Function Declarations
// SPI Communication Functions

void cs_low(void);
void cs_high(void);
void delay_time_us(uint32_t time_delay_val);
uint8_t spi_write_reg_analog(uint8_t data);
void spi_write_array(uint8_t len, uint8_t data[]);
void spi_write_read(uint8_t tx_Data[], uint8_t tx_len, uint8_t *rx_data, uint8_t rx_len);
uint8_t spi_read_byte(uint8_t tx_dat);


// LTC6811 Functions
void temperature_data_read(void);
signed char temp_data_table_read(uint16_t tempe_v_in);
int8_t cell_voltage_read(void);
uint8_t aux_data_read(void);
int8_t stat_data_read(void);
int8_t adc_test_data_read(void);
int8_t adc_overlap_data_read(void);
int8_t digital_redundancy_check(void);
uint16_t crc_error_read(void);
void open_wire_check(void);
void open_wire_gpio_check(void);
int8_t config_data_read(void);
void pwm_reg_read(void);
int8_t s_control_reg_read(void);
void cell_balance_LTC(void);
void analyze_cell_data(void);

////////////////////////////////////////////















//--------------------------------------------------------------//
signed char tempDataTableRead(uint16_t tempe_v_in);


void cs_low();  //CS pin asserted
void cs_high(); //// Disable Pack, /CS deserted
void delay_time_us(uint32_t time_delay_val);

uint8_t spi_write_reg_analog(uint8_t data  //data byte
		);

void spi_transfer_byte(uint8_t cs_pin,      //!< Chip select pin
		uint8_t tx,          //!< Byte to be transmitted
		uint8_t *rx          //!< Byte to be received
		);

void spi_transfer_word(uint8_t cs_pin,      //!< Chip select pin
		uint16_t tx,         //!< Byte to be transmitted
		uint16_t *rx         //!< Byte to be received
		);

void spi_transfer_block(uint8_t cs_pin,     //!< Chip select pin
		uint8_t *tx,        //!< Byte array to be transmitted
		uint8_t *rx,        //!< Byte array to be received
		uint8_t length      //!< Length of array
		);

uint8_t spi_read_byte(uint8_t tx_dat);

void spi_write_array(uint8_t len, // Option: Number of bytes to be written on the SPI port
		uint8_t data[] //Array of bytes to be written on the SPI port
		);

void spi_write_read(uint8_t tx_Data[], //array of data to be written on SPI port
		uint8_t tx_len, //length of the tx data array
		uint8_t *rx_data, //Input: array that will store the data read by the SPI port
		uint8_t rx_len //Option: number of bytes to be read from the SPI port
		);
//--------------------------------------------------------------//

#endif /* ISO_SPI_INC_LTC68XX_API_H_ */
