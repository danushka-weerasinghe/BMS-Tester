//! @todo Review this file.
/*
UserInterface.h

UserInterface routines are used to read and write user data through the Arduino's
serial interface.

Copyright 2018(c) Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in
   the documentation and/or other materials provided with the
   distribution.
 - Neither the name of Analog Devices, Inc. nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.
 - The use of this software may or may not infringe the patent rights
   of one or more patent holders.  This license does not release you
   from the requirement that you obtain separate licenses from these
   patent holders to use this software.
 - Use of the software either in source or binary form, must be run
   on or directly connected to an Analog Devices Inc. component.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <stdint.h>
#include "LTC681x.h"
#include "flash_Itc.h"

// Serial interface defines and functions removed - using Modbus communication instead

// Command processing functions
void run_command(uint32_t cmd);
void measurement_loop(uint8_t datalog_en);
void print_menu(void);

// Print functions for configuration, cells, aux, status
void print_wrconfig(void);
void print_rxconfig(void);
void print_cells(uint8_t datalog_en);
void print_aux(uint8_t datalog_en);
void print_stat(void);
void print_sumofcells(void);
void check_mux_fail(void);
void print_selftest_errors(uint8_t adc_reg, int8_t error);
void print_overlap_results(int8_t error);
void print_digital_redundancy_errors(uint8_t adc_reg, int8_t error);
void print_open_wires(void);
void print_pec_error_count(void);

// PWM and control functions
void print_wrpwm(void);
void print_rxpwm(void);
void print_wrsctrl(void);
void print_rxsctrl(void);
void print_wrcomm(void);
void print_rxcomm(void);
void print_conv_time(uint32_t conv_time);
void check_error(int error);

// Serial communication functions
void serial_print_text(char data[]);
void serial_print_hex(uint8_t data);
char read_hex(void);
char get_char(void);
int8_t select_s_pin(void);

// STM32 print utility functions
void stm32_print(const char* str);
void stm32_println(const char* str);
void stm32_print_int(int value);
void stm32_println_int(int value);
void stm32_print_float(float value, int precision);
void stm32_print_hex(uint8_t value);

// Utility functions
void delay(uint32_t ms);

// External variables and constants
extern char hex_digits[16];
extern char hex_to_byte_buffer[5];
extern char byte_to_hex_buffer[3];

// Global variables for BMS Tester (defined in UserInterface.c)
extern uint8_t modbus_rx_flag;
extern uint8_t RxData_modbus_01[256];
extern uint8_t in_measurement_loop;

// External variables from LTC68xx_API.c
extern cell_asic BMS_IC[TOTAL_IC];
extern uint8_t DC_chain;

// Measurement loop constants (defined in UserInterface.c)
extern const uint16_t MEASUREMENT_LOOP_TIME;
extern const uint8_t WRITE_CONFIG;
extern const uint8_t READ_CONFIG;
extern const uint8_t MEASURE_CELL;
extern const uint8_t MEASURE_AUX;
extern const uint8_t MEASURE_STAT;
extern const uint8_t PRINT_PEC;

#endif  // USERINTERFACE_H
