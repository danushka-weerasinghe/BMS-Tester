//! @todo Review this file.
/*
UserInterface.cpp

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

#include "UserInterface.h"
#include "main.h"
#include "LTC681x.h"
#include "LTC6811.h"
#include "LTC68xx_API.h"
#include "flash_Itc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// External HAL handle declarations
extern UART_HandleTypeDef huart1;

// Global variables for BMS Tester (moved from main.c)
uint8_t modbus_rx_flag = 0;
uint8_t RxData_modbus_01[256];
uint8_t in_measurement_loop = 0; // Flag to track if we're in measurement loop

// External variables from LTC68xx_API.c
extern cell_asic BMS_IC[TOTAL_IC];
extern uint8_t DC_chain;

// TOTAL_IC is defined as a macro in flash_Itc.h

// External function declarations - these are now properly declared in the included headers

// Serial interface functions removed - using Modbus communication instead

// Global variables for hex conversion
char hex_digits[16] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

char hex_to_byte_buffer[5] = {
  '0', 'x', '0', '0', '\0'
};

char byte_to_hex_buffer[3] = {
  '\0','\0','\0'
};

// Constants ENABLED, DISABLED, DATALOG_ENABLED, DATALOG_DISABLED are now defined as macros in main.h
// Measurement loop constants (moved from main.c)
const uint16_t MEASUREMENT_LOOP_TIME = 500; //!< Loop Time in milliseconds(ms)

//Loop Measurement Setup. These Variables are ENABLED or DISABLED. Remember ALL CAPS
const uint8_t WRITE_CONFIG = DISABLED;  //!< This is to ENABLED or DISABLED writing into to configuration registers in a continuous loop
const uint8_t READ_CONFIG = DISABLED; //!< This is to ENABLED or DISABLED reading the configuration registers in a continuous loop
const uint8_t MEASURE_CELL = ENABLED; //!< This is to ENABLED or DISABLED measuring the cell voltages in a continuous loop
const uint8_t MEASURE_AUX = DISABLED; //!< This is to ENABLED or DISABLED reading the auxiliary registers in a continuous loop
const uint8_t MEASURE_STAT = DISABLED; //!< This is to ENABLED or DISABLED reading the status registers in a continuous loop
const uint8_t PRINT_PEC = DISABLED;  //!< This is to ENABLED or DISABLED printing the PEC Error Count in a continuous loop

// STM32 print function implementations
void stm32_print(const char* str)
{
  HAL_GPIO_WritePin(GPIOB, USART1_ENABLE_Pin, GPIO_PIN_SET); // Enable TX
  HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
  HAL_GPIO_WritePin(GPIOB, USART1_ENABLE_Pin, GPIO_PIN_RESET); // Enable RX
}

void stm32_println(const char* str)
{
  HAL_GPIO_WritePin(GPIOB, USART1_ENABLE_Pin, GPIO_PIN_SET); // Enable TX
  HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
  HAL_UART_Transmit(&huart1, (uint8_t*)"\r\n", 2, 1000);
  HAL_GPIO_WritePin(GPIOB, USART1_ENABLE_Pin, GPIO_PIN_RESET); // Enable RX
}

void stm32_print_int(int value)
{
  char buffer[32];
  sprintf(buffer, "%d", value);
  stm32_print(buffer);
}

void stm32_println_int(int value)
{
  char buffer[32];
  sprintf(buffer, "%d", value);
  stm32_println(buffer);
}

void stm32_print_float(float value, int precision)
{
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%d.%02d", (int)value, abs((int)(value*100)%100));
  stm32_print(buffer);
}

void stm32_print_hex(uint8_t value)
{
  char buffer[8];
  sprintf(buffer, "%02X", value);
  stm32_print(buffer);
}

void delay(uint32_t ms)
{
  HAL_Delay(ms);
}

// Command processing function
void run_command(uint32_t cmd)
{
  uint8_t streg=0;
  int8_t error = 0;
  uint32_t conv_time = 0;
  int8_t s_pin_read=0;

  switch (cmd)
  {
    case 1: // Write and Read Configuration Register
      wakeup_sleep(TOTAL_IC);
      LTC6811_wrcfg(TOTAL_IC,BMS_IC); // Write into Configuration Register
      print_wrconfig();
      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdcfg(TOTAL_IC,BMS_IC); // Read Configuration Register
      check_error(error);
      print_rxconfig();
      break;

    case 2: // Read Configuration Register
      wakeup_sleep(TOTAL_IC);
      error = LTC6811_rdcfg(TOTAL_IC,BMS_IC);
      check_error(error);
      print_rxconfig();
      break;

    case 3: // Start Cell ADC Measurement
      wakeup_sleep(TOTAL_IC);
      LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
      conv_time = LTC6811_pollAdc();
      print_conv_time(conv_time);
      break;

    case 4: // Read Cell Voltage Registers
      wakeup_sleep(TOTAL_IC);
      error = LTC6811_rdcv(SEL_ALL_REG, TOTAL_IC,BMS_IC); // Set to read back all cell voltage registers
      check_error(error);
      print_cells(DATALOG_DISABLED);
      break;

    case 5: // Start GPIO ADC Measurement
      wakeup_sleep(TOTAL_IC);
      LTC6811_adax(ADC_CONVERSION_MODE, AUX_CH_TO_CONVERT);
      conv_time = LTC6811_pollAdc();
      print_conv_time(conv_time);
      break;

    case 6: // Read AUX Voltage Registers
      wakeup_sleep(TOTAL_IC);
      error = LTC6811_rdaux(SEL_ALL_REG,TOTAL_IC,BMS_IC); // Set to read back all aux registers
      check_error(error);
      print_aux(DATALOG_DISABLED);
      break;

    case 7: // Start Status ADC Measurement
      wakeup_sleep(TOTAL_IC);
      LTC6811_adstat(ADC_CONVERSION_MODE, STAT_CH_TO_CONVERT);
      conv_time=LTC6811_pollAdc();
      print_conv_time(conv_time);
      break;

    case 8: // Read Status registers
      wakeup_sleep(TOTAL_IC);
      error = LTC6811_rdstat(SEL_ALL_REG,TOTAL_IC,BMS_IC); // Set to read back all stat registers
      check_error(error);
      print_stat();
      break;

    case 9:// Start Combined Cell Voltage and GPIO1, GPIO2 Conversion and Poll Status
      wakeup_sleep(TOTAL_IC);
      LTC6811_adcvax(ADC_CONVERSION_MODE,ADC_DCP);
      conv_time = LTC6811_pollAdc();
      print_conv_time(conv_time);
      wakeup_idle(TOTAL_IC);
      error =LTC6811_rdcv(SEL_ALL_REG, TOTAL_IC,BMS_IC); // Set to read back all cell voltage registers
      check_error(error);
      print_cells(DATALOG_DISABLED);
      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdaux(SEL_REG_A,TOTAL_IC,BMS_IC); // Set to read back aux registers A
      check_error(error);
      print_aux(DATALOG_DISABLED);
      break;

    case 10: //Start Combined Cell Voltage and Sum of cells
      wakeup_sleep(TOTAL_IC);
      LTC6811_adcvsc(ADC_CONVERSION_MODE,ADC_DCP);
      conv_time = LTC6811_pollAdc();
      print_conv_time(conv_time);
      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdcv(SEL_ALL_REG, TOTAL_IC,BMS_IC); // Set to read back all cell voltage registers
      check_error(error);
      print_cells(DATALOG_DISABLED);
      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdstat(SEL_REG_A,TOTAL_IC,BMS_IC); // Set to read stat registers A
      check_error(error);
      print_sumofcells();
      break;

    case 11: // Loop Measurements of configuration register or cell voltages or auxiliary register or status register without data-log output
      wakeup_sleep(TOTAL_IC);
      LTC6811_wrcfg(TOTAL_IC,BMS_IC);
      measurement_loop(DATALOG_DISABLED);
      print_menu();
      break;

    case 12: //Data-log print option Loop Measurements of configuration register or cell voltages or auxiliary register or status register
      wakeup_sleep(TOTAL_IC);
      LTC6811_wrcfg(TOTAL_IC,BMS_IC);
      measurement_loop(DATALOG_ENABLED);
      print_menu();
      break;

    case 13: // Clear all ADC measurement registers
      wakeup_sleep(TOTAL_IC);
      LTC6811_clrcell();
      LTC6811_clraux();
      LTC6811_clrstat();
      wakeup_idle(TOTAL_IC);
      LTC6811_rdcv(SEL_ALL_REG, TOTAL_IC,BMS_IC); // Read back all cell voltage registers
      print_cells(DATALOG_DISABLED);

      LTC6811_rdaux(SEL_ALL_REG,TOTAL_IC,BMS_IC); // Read back all aux registers
      print_aux(DATALOG_DISABLED);

      LTC6811_rdstat(SEL_ALL_REG,TOTAL_IC,BMS_IC); // Read back all stat
      print_stat();
      break;

    case 14: //Read CV,AUX and ADSTAT Voltages
      wakeup_sleep(TOTAL_IC);
      LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
      conv_time = LTC6811_pollAdc();
      print_conv_time(conv_time);
      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdcv(SEL_ALL_REG, TOTAL_IC,BMS_IC); // Set to read back all cell voltage registers
      check_error(error);
      print_cells(DATALOG_DISABLED);

      wakeup_sleep(TOTAL_IC);
      LTC6811_adax(ADC_CONVERSION_MODE , AUX_CH_TO_CONVERT);
      conv_time = LTC6811_pollAdc();
      print_conv_time(conv_time);
      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdaux(SEL_ALL_REG,TOTAL_IC,BMS_IC); // Set to read back all aux registers
      check_error(error);
      print_aux(DATALOG_DISABLED);

      wakeup_sleep(TOTAL_IC);
      LTC6811_adstat(ADC_CONVERSION_MODE, STAT_CH_TO_CONVERT);
      conv_time = LTC6811_pollAdc();
      print_conv_time(conv_time);
      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdstat(SEL_ALL_REG,TOTAL_IC,BMS_IC); // Set to read back all status registers
      check_error(error);
      print_stat();
      break;

    case 15: // Run the Mux Decoder Self Test
      wakeup_sleep(TOTAL_IC);
      LTC6811_diagn();
      conv_time = LTC6811_pollAdc();
      print_conv_time(conv_time);
      error = LTC6811_rdstat(SEL_REG_B,TOTAL_IC,BMS_IC); // Set to read back status register B
      check_error(error);
      check_mux_fail();
      break;

    case 16:  // Run the ADC/Memory Self Test
      error =0;
      wakeup_sleep(TOTAL_IC);
      error = LTC6811_run_cell_adc_st(CELL,TOTAL_IC,BMS_IC, ADC_CONVERSION_MODE, ADCOPT);
      print_selftest_errors(CELL, error);

      error =0;
      wakeup_sleep(TOTAL_IC);
      error = LTC6811_run_cell_adc_st(AUX,TOTAL_IC, BMS_IC, ADC_CONVERSION_MODE, ADCOPT);
      print_selftest_errors(AUX, error);

      error =0;
      wakeup_sleep(TOTAL_IC);
      error = LTC6811_run_cell_adc_st(STAT,TOTAL_IC, BMS_IC, ADC_CONVERSION_MODE, ADCOPT);
      print_selftest_errors(STAT, error);
      print_menu();
      break;

    case 17: // Run ADC Overlap self test
      error =0;
      wakeup_sleep(TOTAL_IC);
      error = (int8_t)LTC6811_run_adc_overlap(TOTAL_IC,BMS_IC);
      print_overlap_results(error);
      break;

    case 18: // Run ADC Digital Redundancy self test
      error =0;
      wakeup_sleep(TOTAL_IC);
      error = LTC6811_run_adc_redundancy_st(ADC_CONVERSION_MODE,AUX,TOTAL_IC, BMS_IC);
      print_digital_redundancy_errors(AUX, error);

      error =0;
      wakeup_sleep(TOTAL_IC);
      error = LTC6811_run_adc_redundancy_st(ADC_CONVERSION_MODE,STAT,TOTAL_IC, BMS_IC);
      print_digital_redundancy_errors(STAT, error);
      break;

    case 19: // Open Wire test for single cell detection
      wakeup_sleep(TOTAL_IC);
      LTC6811_run_openwire_single(TOTAL_IC, BMS_IC);
      print_open_wires();
      break;

    case 20: // Open Wire test for multiple cell and two consecutive cells detection
      wakeup_sleep(TOTAL_IC);
      LTC6811_run_openwire_multi(TOTAL_IC, BMS_IC);
      break;

    case 21:// PEC Errors Detected
      print_pec_error_count();
      break;

    case 22: // Reset PEC Counter
      LTC6811_reset_crc_count(TOTAL_IC,BMS_IC);
      print_pec_error_count();
      break;

    case 23: // Enable a discharge transistor
      s_pin_read = select_s_pin();
      wakeup_sleep(TOTAL_IC);
      LTC6811_set_discharge(s_pin_read,TOTAL_IC,BMS_IC);
      LTC6811_wrcfg(TOTAL_IC,BMS_IC);
      print_wrconfig();
      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdcfg(TOTAL_IC,BMS_IC);
      check_error(error);
      print_rxconfig();
      break;

    case 24: // Clear all discharge transistors
      wakeup_sleep(TOTAL_IC);
      LTC6811_clear_discharge(TOTAL_IC,BMS_IC);
      LTC6811_wrcfg(TOTAL_IC,BMS_IC);
      print_wrconfig();
      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdcfg(TOTAL_IC,BMS_IC);
      check_error(error);
      print_rxconfig();
      break;

    case 25:// Write read pwm configuration
      /*****************************************************
         PWM configuration data.
         1)Set the corresponding DCC bit to one for pwm operation.
         2)Set the DCTO bits to the required discharge time.
         3)Choose the value to be configured depending on the
          required duty cycle.
         Refer to the data sheet.
      *******************************************************/
      wakeup_sleep(TOTAL_IC);
      for (uint8_t current_ic = 0; current_ic<TOTAL_IC;current_ic++)
      {
        BMS_IC[current_ic].pwm.tx_data[0]= 0x88; // Duty cycle for S pin 2 and 1
        BMS_IC[current_ic].pwm.tx_data[1]= 0x88; // Duty cycle for S pin 4 and 3
        BMS_IC[current_ic].pwm.tx_data[2]= 0x88; // Duty cycle for S pin 6 and 5
        BMS_IC[current_ic].pwm.tx_data[3]= 0x88; // Duty cycle for S pin 8 and 7
        BMS_IC[current_ic].pwm.tx_data[4]= 0x88; // Duty cycle for S pin 10 and 9
        BMS_IC[current_ic].pwm.tx_data[5]= 0x88; // Duty cycle for S pin 12 and 11
      }
      LTC6811_wrpwm(TOTAL_IC,0,BMS_IC);
      print_wrpwm();

      wakeup_idle(TOTAL_IC);
      LTC6811_rdpwm(TOTAL_IC,0,BMS_IC);
      print_rxpwm();
      break;

    case 26: // Write and read S Control Register Group
      wakeup_sleep(TOTAL_IC);
      /**************************************************************************************
         S pin control.
         1)Ensure that the pwm is set according to the requirement using the previous case.
         2)Choose the value depending on the required number of pulses on S pin.
         Refer to the data sheet.
      ***************************************************************************************/
      for (uint8_t current_ic = 0; current_ic<TOTAL_IC;current_ic++)
      {
        BMS_IC[current_ic].sctrl.tx_data[0]=0xFF; // No. of high pulses on S pin 2 and 1
        BMS_IC[current_ic].sctrl.tx_data[1]=0xFF; // No. of high pulses on S pin 4 and 3
        BMS_IC[current_ic].sctrl.tx_data[2]=0xFF; // No. of high pulses on S pin 6 and 5
        BMS_IC[current_ic].sctrl.tx_data[3]=0xFF; // No. of high pulses on S pin 8 and 7
        BMS_IC[current_ic].sctrl.tx_data[4]=0xFF; // No. of high pulses on S pin 10 and 9
        BMS_IC[current_ic].sctrl.tx_data[5]=0xFF; // No. of high pulses on S pin 12 and 11
      }
      LTC6811_wrsctrl(TOTAL_IC,streg,BMS_IC);
      print_wrsctrl();

      // Start S Control pulsing
      wakeup_idle(TOTAL_IC);
      LTC6811_stsctrl();

      // Read S Control Register Group
      wakeup_idle(TOTAL_IC);
      error=LTC6811_rdsctrl(TOTAL_IC,streg,BMS_IC);
      check_error(error);
      print_rxsctrl();
      break;

    case 27: // Clear S Control Register Group
      wakeup_sleep(TOTAL_IC);
      LTC6811_clrsctrl();

      wakeup_idle(TOTAL_IC);
      error=LTC6811_rdsctrl(TOTAL_IC,streg,BMS_IC); // Read S Control Register Group
      check_error(error);
      print_rxsctrl();
      break;

    case 28://SPI Communication
      /*************************************************************
         Ensure to set the GPIO bits to 1 in the CFG register group.
      *************************************************************/
      for (uint8_t current_ic = 0; current_ic<TOTAL_IC;current_ic++)
      {
        //Communication control bits and communication data bytes. Refer to the data sheet.
        BMS_IC[current_ic].com.tx_data[0]= 0x81; // Icom CSBM Low(8) + data D0 (0x11)
        BMS_IC[current_ic].com.tx_data[1]= 0x10; // Fcom CSBM Low(0)
        BMS_IC[current_ic].com.tx_data[2]= 0xA2; // Icom CSBM Falling Edge (A) +  D1 (0x25)
        BMS_IC[current_ic].com.tx_data[3]= 0x50; // Fcom CSBM Low(0)
        BMS_IC[current_ic].com.tx_data[4]= 0xA1; // Icom CSBM Falling Edge (A) +  D2 (0x17)
        BMS_IC[current_ic].com.tx_data[5]= 0x79; // Fcom CSBM High(9)
      }
      wakeup_sleep(TOTAL_IC);
      LTC6811_wrcomm(TOTAL_IC,BMS_IC); // write to comm register
      print_wrcomm(); // print data in the comm register

      wakeup_idle(TOTAL_IC);
      LTC6811_stcomm(3); // data length=3 // initiates communication between master and the I2C slave

      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdcomm(TOTAL_IC,BMS_IC); // read from comm register
      check_error(error);
      print_rxcomm();  // print received data into the comm register
      break;

  case 29: // write byte I2C Communication on the GPIO Ports(using I2C eeprom 24LC025)
       /************************************************************
         Ensure to set the GPIO bits to 1 in the CFG register group.
      *************************************************************/
      for (uint8_t current_ic = 0; current_ic<TOTAL_IC;current_ic++)
      {
        //Communication control bits and communication data bytes. Refer to the data sheet.
        BMS_IC[current_ic].com.tx_data[0]= 0x6A; // Icom Start(6) + I2C_address D0 (0xA0)
        BMS_IC[current_ic].com.tx_data[1]= 0x08; // Fcom master NACK(8)
        BMS_IC[current_ic].com.tx_data[2]= 0x00; // Icom Blank (0) + eeprom address D1 (0x00)
        BMS_IC[current_ic].com.tx_data[3]= 0x08; // Fcom master NACK(8)
        BMS_IC[current_ic].com.tx_data[4]= 0x01; // Icom Blank (0) + data D2 (0x11)
        BMS_IC[current_ic].com.tx_data[5]= 0x19; // Fcom master NACK + Stop(9)
      }
      wakeup_sleep(TOTAL_IC);
      LTC6811_wrcomm(TOTAL_IC,BMS_IC); // write to comm register
      print_wrcomm(); // print transmitted data from the comm register

      wakeup_idle(TOTAL_IC);
      LTC6811_stcomm(3); // data length=3 // initiates communication between master and the I2C slave

      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdcomm(TOTAL_IC,BMS_IC); // read from comm register
      check_error(error);
      print_rxcomm(); // print received data into the comm register
      break;

    case 30: // Read byte data I2C Communication on the GPIO Ports(using I2C eeprom 24LC025)
      /************************************************************
         Ensure to set the GPIO bits to 1 in the CFG register group.
      *************************************************************/
      for (uint8_t current_ic = 0; current_ic<TOTAL_IC;current_ic++)
      {
        //Communication control bits and communication data bytes. Refer to the data sheet.
        BMS_IC[current_ic].com.tx_data[0]= 0x6A; // Icom Start (6) + I2C_address D0 (A0) (Write operation to set the word address)
        BMS_IC[current_ic].com.tx_data[1]= 0x08; // Fcom master NACK(8)
        BMS_IC[current_ic].com.tx_data[2]= 0x00; // Icom Blank (0) + eeprom address(word address) D1 (0x00)
        BMS_IC[current_ic].com.tx_data[3]= 0x08; // Fcom master NACK(8)
        BMS_IC[current_ic].com.tx_data[4]= 0x6A; // Icom Start (6) + I2C_address D2 (0xA1)(Read operation)
        BMS_IC[current_ic].com.tx_data[5]= 0x18; // Fcom master NACK(8)
      }
      wakeup_sleep(TOTAL_IC);
      LTC6811_wrcomm(TOTAL_IC,BMS_IC); // write to comm register

      wakeup_idle(TOTAL_IC);
      LTC6811_stcomm(3); // data length=3 // initiates communication between master and the I2C slave

      for (uint8_t current_ic = 0; current_ic<TOTAL_IC;current_ic++)
      {
        //Communication control bits and communication data bytes. Refer to the data sheet.
        BMS_IC[current_ic].com.tx_data[0]= 0x0F; // Icom Blank (0) + data D0 (FF)
        BMS_IC[current_ic].com.tx_data[1]= 0xF9; // Fcom master NACK + Stop(9)
        BMS_IC[current_ic].com.tx_data[2]= 0x7F; // Icom No Transmit (7) + data D1 (FF)
        BMS_IC[current_ic].com.tx_data[3]= 0xF9; // Fcom master NACK + Stop(9)
        BMS_IC[current_ic].com.tx_data[4]= 0x7F; // Icom No Transmit (7) + data D2 (FF)
        BMS_IC[current_ic].com.tx_data[5]= 0xF9; // Fcom master NACK + Stop(9)
      }

      wakeup_idle(TOTAL_IC);
      LTC6811_wrcomm(TOTAL_IC,BMS_IC); // write to comm register

      wakeup_idle(TOTAL_IC);
      LTC6811_stcomm(1); // data length=1 // initiates communication between master and the I2C slave

      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdcomm(TOTAL_IC,BMS_IC); // read from comm register
      check_error(error);
      print_rxcomm(); // print received data from the comm register
      break;

    case 31: // Set or reset the gpio pins(to drive output on gpio pins)
      /***********************************************************************
       Please ensure you have set the GPIO bits according to your requirement
       in the configuration register.( check the global variable GPIOBITS_A )
      ************************************************************************/
      wakeup_sleep(TOTAL_IC);
      for (uint8_t current_ic = 0; current_ic<TOTAL_IC;current_ic++)
      {
        LTC6811_set_cfgr(current_ic,BMS_IC,(bool)REF_ON,(bool)ADCOPT,GPIOBITS_A,DCCBITS_A, DCTOBITS, UV, OV);
      }
      wakeup_idle(TOTAL_IC);
      LTC6811_wrcfg(TOTAL_IC,BMS_IC);
      print_wrconfig();
      break;

    case 'm': //prints menu
      print_menu();
      break;

    default:
      char str_error[]="Incorrect Option \n";
      serial_print_text(str_error);
      break;
  }
}

// Measurement loop function
void measurement_loop(uint8_t datalog_en)
{
  int8_t error = 0;
  char input = 0;

  //in measurement loop
  in_measurement_loop = 1;
  
  stm32_println("Transmit 'm' to quit");

  while (input != 'm')
  {

     if (modbus_rx_flag == 1)
     {
       if (RxData_modbus_01[0] == 'm')
       {
         input = 'm';
         modbus_rx_flag = 0;
         break;
       }
       modbus_rx_flag = 0; // Reset flag for other commands
     }

    if (WRITE_CONFIG == ENABLED)
    {
      wakeup_sleep(TOTAL_IC);
      LTC6811_wrcfg(TOTAL_IC,BMS_IC);
      print_wrconfig();
    }

    if (READ_CONFIG == ENABLED)
    {
      wakeup_sleep(TOTAL_IC);
      error = LTC6811_rdcfg(TOTAL_IC,BMS_IC);
      check_error(error);
      print_rxconfig();
    }

    if (MEASURE_CELL == ENABLED)
    {
      wakeup_idle(TOTAL_IC);
      LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
      LTC6811_pollAdc();
      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdcv(SEL_ALL_REG, TOTAL_IC,BMS_IC);
      check_error(error);
      print_cells(datalog_en);
    }

    if (MEASURE_AUX == ENABLED)
    {
      wakeup_idle(TOTAL_IC);
      LTC6811_adax(ADC_CONVERSION_MODE , AUX_CH_ALL);
      LTC6811_pollAdc();
      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdaux(SEL_ALL_REG,TOTAL_IC,BMS_IC); // Set to read back all aux registers
      check_error(error);
      print_aux(datalog_en);
    }

    if (MEASURE_STAT == ENABLED)
    {
      wakeup_idle(TOTAL_IC);
      LTC6811_adstat(ADC_CONVERSION_MODE, STAT_CH_ALL);
      LTC6811_pollAdc();
      wakeup_idle(TOTAL_IC);
      error = LTC6811_rdstat(SEL_ALL_REG,TOTAL_IC,BMS_IC); // Set to read back all aux registers
      check_error(error);
      print_stat();
    }

    if (PRINT_PEC == ENABLED)
    {
      print_pec_error_count();
    }

    HAL_Delay(MEASUREMENT_LOOP_TIME);
  }
  
  // Clear flag when exiting measurement loop
  in_measurement_loop = 0;
}

// Print menu function
void print_menu(void)
{
	stm32_println("List of 6811 Commands: ");
	stm32_println("Write and Read Configuration: 1                            |Loop measurements with data-log output: 12     |Set Discharge: 23");
	stm32_println("Read Configuration: 2                                      |Clear Registers: 13                            |Clear Discharge: 24");
	stm32_println("Start Cell Voltage Conversion: 3                           |Read CV,AUX and ADSTAT Voltages: 14            |Write and Read of PWM: 25");
	stm32_println("Read Cell Voltages: 4                                      |Run Mux Self Test: 15                          |Write and Read of S control: 26");
	stm32_println("Start Aux Voltage Conversion: 5                            |Run ADC Self Test: 16                          |Clear S control register: 27");
	stm32_println("Read Aux Voltages: 6                                       |ADC overlap Test : 17                          |SPI Communication: 28");
	stm32_println("Start Stat Voltage Conversion: 7                           |Run Digital Redundancy Test: 18                |I2C Communication Write to Slave: 29");
	stm32_println("Read Stat Voltages: 8                                      |Open Wire Test for single cell detection: 19   |I2C Communication Read from Slave:30");
	stm32_println("Start Combined Cell Voltage and GPIO1, GPIO2 Conversion: 9 |Open Wire Test for multiple cell detection: 20 |Set or Reset the GPIO pins: 31 ");
	stm32_println("Start  Cell Voltage and Sum of cells : 10                  |Print PEC Counter: 21                          |");
	stm32_println("Loop Measurements: 11                                      |Reset PEC Counter: 22                          | \n ");

	stm32_println("Print 'm' for menu");
	stm32_println("Please enter command: \n");
}

// Print configuration functions
void print_wrconfig(void)
{
  int cfg_pec;

  stm32_println("Written Configuration: ");
  for (int current_ic = 0; current_ic<TOTAL_IC; current_ic++)
  {
    stm32_print("CFGA IC ");
    stm32_print_int(current_ic+1);
    for(int i = 0;i<6;i++)
    {
      stm32_print(", 0x");
      stm32_print_hex(BMS_IC[current_ic].config.tx_data[i]);
    }
    stm32_print(", Calculated PEC: 0x");
    cfg_pec = pec15_calc(6,&BMS_IC[current_ic].config.tx_data[0]);
    stm32_print_hex((uint8_t)(cfg_pec>>8));
    stm32_print(", 0x");
    stm32_print_hex((uint8_t)(cfg_pec));
    stm32_println("\n");
  }
}

void print_rxconfig(void)
{
  stm32_println("Received Configuration ");
  for (int current_ic=0; current_ic<TOTAL_IC; current_ic++)
  {
    stm32_print("CFGA IC ");
    stm32_print_int(current_ic+1);
    for(int i = 0; i < 6; i++)
    {
      stm32_print(", 0x");
      stm32_print_hex(BMS_IC[current_ic].config.rx_data[i]);
    }
    stm32_print(", Received PEC: 0x");
    stm32_print_hex(BMS_IC[current_ic].config.rx_data[6]);
    stm32_print(", 0x");
    stm32_print_hex(BMS_IC[current_ic].config.rx_data[7]);
    stm32_println("\n");
  }
}

// Print cell voltage function
void print_cells(uint8_t datalog_en)
{
  for (int current_ic = 0 ; current_ic < TOTAL_IC; current_ic++)
  {
    if (datalog_en == 0)
    {
      stm32_print(" IC ");
      stm32_print_int(current_ic+1);
      stm32_print(": ");
      for (int i=0; i< BMS_IC[0].ic_reg.cell_channels; i++)
      {
        stm32_print(" C");
        stm32_print_int(i+1);
        stm32_print(":");
        stm32_print_float(BMS_IC[current_ic].cells.c_codes[i]*0.0001, 4);
        stm32_print(",");
      }
      stm32_println("");
    }
    else
    {
      stm32_print(" Cells :");
      for (int i=0; i<BMS_IC[0].ic_reg.cell_channels; i++)
      {
        stm32_print_float(BMS_IC[current_ic].cells.c_codes[i]*0.0001, 4);
        stm32_print(",");
      }
    }
  }
  stm32_println("\n");
}

// Print auxiliary voltage function
void print_aux(uint8_t datalog_en)
{

  for (int current_ic =0 ; current_ic < TOTAL_IC; current_ic++)
  {
    if (datalog_en == 0)
    {
      stm32_print(" IC ");
      stm32_print_int(current_ic+1);
      stm32_print(":");

      for (int i=0; i < 5; i++)
      {
        stm32_print((" GPIO-"));
        stm32_print_int(i+1);
        stm32_print(":");
        stm32_print_float(BMS_IC[current_ic].aux.a_codes[i]*0.0001, 4);
        stm32_print(",");
      }
      stm32_print((" Vref2"));
      stm32_print(":");
      stm32_print_float(BMS_IC[current_ic].aux.a_codes[5]*0.0001, 4);
      stm32_println("");
    }
    else
    {
      stm32_print("AUX ");
      stm32_print(" IC ");
      stm32_print_int(current_ic+1);
      stm32_print(": ");

      for (int i=0; i < 6; i++)
      {
        stm32_print_float(BMS_IC[current_ic].aux.a_codes[i]*0.0001, 4);
        stm32_print(",");
      }
    }
  }
  stm32_println("\n");
}

// Print status function
void print_stat(void)
{
   double itmp;
  for (uint8_t current_ic =0 ; current_ic < TOTAL_IC; current_ic++)
  {
    stm32_print((" IC "));
    stm32_print_int(current_ic+1);
    stm32_print((": "));
    stm32_print((" SOC:"));
    stm32_print_float(BMS_IC[current_ic].stat.stat_codes[0]*0.0001*20, 4);
    stm32_print((","));
    stm32_print((" Itemp:"));
    itmp = (double)((BMS_IC[current_ic].stat.stat_codes[1] * (0.0001 / 0.0075)) - 273);   //Internal Die Temperature(°C) = itmp • (100 µV / 7.5mV)°C - 273°C
    stm32_print_float(itmp, 4);
    stm32_print((","));
    stm32_print((" VregA:"));
    stm32_print_float(BMS_IC[current_ic].stat.stat_codes[2]*0.0001, 4);
    stm32_print((","));
    stm32_print((" VregD:"));
    stm32_print_float(BMS_IC[current_ic].stat.stat_codes[3]*0.0001, 4);
    stm32_println("");
    stm32_print((" Flags:"));
    stm32_print((" 0x"));
    serial_print_hex(BMS_IC[current_ic].stat.flags[0]);
    stm32_print((", 0x"));
    serial_print_hex(BMS_IC[current_ic].stat.flags[1]);
    stm32_print((", 0x"));
    serial_print_hex(BMS_IC[current_ic].stat.flags[2]);
    stm32_print(("   Mux fail flag:"));
    stm32_print((" 0x"));
    serial_print_hex(BMS_IC[current_ic].stat.mux_fail[0]);
    stm32_print(("   THSD:"));
    stm32_print((" 0x"));
    serial_print_hex(BMS_IC[current_ic].stat.thsd[0]);
    stm32_println("\n");
  }
}

// Print sum of cells function
void print_sumofcells(void)
{
 for (int current_ic =0 ; current_ic < TOTAL_IC; current_ic++)
  {
    stm32_print((" IC "));
    stm32_print_int(current_ic+1);
    stm32_print((": "));
    stm32_print((" SOC:"));
    stm32_print_float(BMS_IC[current_ic].stat.stat_codes[0]*0.0001*20, 4);
    stm32_print((","));
  }
  stm32_println("\n");
}

// Check mux fail function
void check_mux_fail(void)
{
  int8_t error = 0;
  for (int ic = 0; ic<TOTAL_IC; ic++)
    {
      stm32_print(" IC ");
      stm32_println_int(ic+1);
      if (BMS_IC[ic].stat.mux_fail[0] != 0) error++;

      if (error==0) stm32_println(("Mux Test: PASS \n"));
      else stm32_println(("Mux Test: FAIL \n"));
    }
}

// Print selftest errors function
void print_selftest_errors(uint8_t adc_reg ,int8_t error)
{
  if(adc_reg==1)
  {
    stm32_println("Cell ");
    }
  else if(adc_reg==2)
  {
    stm32_println("Aux ");
    }
  else if(adc_reg==3)
  {
    stm32_println("Stat ");
    }
  stm32_print_int(error);
  stm32_println((" : errors detected in Digital Filter and Memory \n"));
}

// Print overlap results function
void print_overlap_results(int8_t error)
{
  if (error==0) stm32_println(("Overlap Test: PASS \n"));
  else stm32_println(("Overlap Test: FAIL \n"));
}

// Print digital redundancy errors function
void print_digital_redundancy_errors(uint8_t adc_reg ,int8_t error)
{
  if(adc_reg==2)
  {
    stm32_println("Aux ");
    }
  else if(adc_reg==3)
  {
    stm32_println("Stat ");
    }

  stm32_print_int(error);
  stm32_println((" : errors detected in Measurement \n"));
}

// Print open wires function
void print_open_wires(void)
{
  for (int current_ic =0 ; current_ic < TOTAL_IC; current_ic++)
  {
    if (BMS_IC[current_ic].system_open_wire == 65535)
    {
      stm32_print("No Opens Detected on IC ");
      stm32_println_int(current_ic+1);
    }
    else
    {
      stm32_print(("There is an open wire on IC "));
      stm32_print_int(current_ic + 1);
      stm32_print((" Channel: "));
      stm32_println_int(BMS_IC[current_ic].system_open_wire);
    }
    stm32_println("\n");
  }
}

// Print PEC error count function
void print_pec_error_count(void)
{
  for (int current_ic=0; current_ic<TOTAL_IC; current_ic++)
  {
    stm32_println("");
    stm32_print_int(BMS_IC[current_ic].crc_count.pec_count);
    stm32_print((" : PEC Errors Detected on IC"));
    stm32_println_int(current_ic+1);
  }
  stm32_println("\n");
}

// Select S pin function
int8_t select_s_pin(void)
{
  int8_t read_s_pin=0;

  stm32_print("Please enter the Spin number: ");
  
  // Wait for UART data to be received
  while (modbus_rx_flag == 0) {
	  delay_time_us(10000);
  }
  
  // Parse the received integer data directly
  if (RxData_modbus_01[0] >= '0' && RxData_modbus_01[0] <= '9') {
    // Single digit (0-9)
    read_s_pin = RxData_modbus_01[0] - '0';
  } else if (RxData_modbus_01[0] >= '1' && RxData_modbus_01[0] <= '9' && 
             RxData_modbus_01[1] >= '0' && RxData_modbus_01[1] <= '9') {
    // Two digits (10-99)
    read_s_pin = (RxData_modbus_01[0] - '0') * 10 + (RxData_modbus_01[1] - '0');
  } else {
    read_s_pin = 0; // Default value if no valid input
  }
  
  modbus_rx_flag = 0; // Reset flag
  stm32_println_int(read_s_pin);
  return(read_s_pin);
}

// PWM print functions
void print_wrpwm(void)
{
  int pwm_pec;

  stm32_println(("Written PWM Configuration: "));
  for (uint8_t current_ic = 0; current_ic<TOTAL_IC; current_ic++)
  {
    stm32_print(("IC "));
    stm32_print_int(current_ic+1);
    for(int i = 0; i < 6; i++)
    {
      stm32_print((", 0x"));
     serial_print_hex(BMS_IC[current_ic].pwm.tx_data[i]);
    }
    stm32_print((", Calculated PEC: 0x"));
    pwm_pec = pec15_calc(6,&BMS_IC[current_ic].pwm.tx_data[0]);
    serial_print_hex((uint8_t)(pwm_pec>>8));
    stm32_print((", 0x"));
    serial_print_hex((uint8_t)(pwm_pec));
    stm32_println("\n");
  }
}

void print_rxpwm(void)
{
  stm32_println(("Received pwm Configuration:"));
  for (uint8_t current_ic=0; current_ic<TOTAL_IC; current_ic++)
  {
    stm32_print(("IC "));
    stm32_print_int(current_ic+1);
    for(int i = 0; i < 6; i++)
    {
      stm32_print((", 0x"));
     serial_print_hex(BMS_IC[current_ic].pwm.rx_data[i]);
    }
    stm32_print((", Received PEC: 0x"));
    serial_print_hex(BMS_IC[current_ic].pwm.rx_data[6]);
    stm32_print((", 0x"));
    serial_print_hex(BMS_IC[current_ic].pwm.rx_data[7]);
    stm32_println("\n");
  }
}

// S control print functions
void print_wrsctrl(void)
{
   int sctrl_pec;

  stm32_println(("Written Data in Sctrl register: "));
  for (int current_ic = 0; current_ic<TOTAL_IC; current_ic++)
  {
    stm32_print((" IC: "));
    stm32_print_int(current_ic+1);
    stm32_print((" Sctrl register group:"));
    for(int i = 0; i < 6; i++)
    {
      stm32_print((", 0x"));
      serial_print_hex(BMS_IC[current_ic].sctrl.tx_data[i]);
    }

    stm32_print((", Calculated PEC: 0x"));
    sctrl_pec = pec15_calc(6,&BMS_IC[current_ic].sctrl.tx_data[0]);
    serial_print_hex((uint8_t)(sctrl_pec>>8));
    stm32_print((", 0x"));
    serial_print_hex((uint8_t)(sctrl_pec));
    stm32_println("\n");
  }
}

void print_rxsctrl(void)
{
  stm32_println(("Received Data:"));
  for (int current_ic=0; current_ic<TOTAL_IC; current_ic++)
  {
    stm32_print((" IC "));
    stm32_print_int(current_ic+1);

    for(int i = 0; i < 6; i++)
    {
    stm32_print((", 0x"));
    serial_print_hex(BMS_IC[current_ic].sctrl.rx_data[i]);
    }

    stm32_print((", Received PEC: 0x"));
    serial_print_hex(BMS_IC[current_ic].sctrl.rx_data[6]);
    stm32_print((", 0x"));
    serial_print_hex(BMS_IC[current_ic].sctrl.rx_data[7]);
    stm32_println("\n");
  }
}

// Communication print functions
void print_wrcomm(void)
{
  int comm_pec;

  stm32_println(("Written Data in COMM Register: "));
  for (int current_ic = 0; current_ic<TOTAL_IC; current_ic++)
  {
    stm32_print((" IC- "));
    stm32_print_int(current_ic+1);

    for(int i = 0; i < 6; i++)
    {
      stm32_print((", 0x"));
      serial_print_hex(BMS_IC[current_ic].com.tx_data[i]);
    }
    stm32_print((", Calculated PEC: 0x"));
    comm_pec = pec15_calc(6,&BMS_IC[current_ic].com.tx_data[0]);
    serial_print_hex((uint8_t)(comm_pec>>8));
    stm32_print((", 0x"));
    serial_print_hex((uint8_t)(comm_pec));
    stm32_println("\n");
  }
}

void print_rxcomm(void)
{
  stm32_println(("Received Data in COMM register:"));
  for (int current_ic=0; current_ic<TOTAL_IC; current_ic++)
  {
    stm32_print((" IC- "));
    stm32_print_int(current_ic+1);

    for(int i = 0; i < 6; i++)
    {
      stm32_print((", 0x"));
      serial_print_hex(BMS_IC[current_ic].com.rx_data[i]);
    }
    stm32_print((", Received PEC: 0x"));
    serial_print_hex(BMS_IC[current_ic].com.rx_data[6]);
    stm32_print((", 0x"));
    serial_print_hex(BMS_IC[current_ic].com.rx_data[7]);
    stm32_println("\n");
  }
}

// Utility print functions
void print_conv_time(uint32_t conv_time)
{
  uint16_t m_factor=1000;  // to print in ms

  stm32_print(("Conversion completed in:"));
  stm32_print_float(((float)conv_time/m_factor), 1);
  stm32_println(("ms \n"));
}

void check_error(int error)
{
  if (error == -1)
  {
    stm32_println(("A PEC error was detected in the received data"));
  }
}

// Serial communication functions
void serial_print_text(char data[])
{
  stm32_println(data);
}

void serial_print_hex(uint8_t data)
{
  if (data< 16)
  {
    stm32_print("0");
    stm32_print_hex((uint8_t)data);
  }
  else
    stm32_print_hex((uint8_t)data);
}

// Hex conversion functions
char read_hex(void)
{
  uint8_t data;
  hex_to_byte_buffer[2]=get_char();
  hex_to_byte_buffer[3]=get_char();
  get_char();
  get_char();
  data = strtol(hex_to_byte_buffer, NULL, 0);
  return(data);
}

char get_char(void)
{
  uint8_t rx_data;
  while (!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE));
  HAL_UART_Receive(&huart1, &rx_data, 1, 100);
  return(rx_data);
}

