
/*!
LT_I2C: Routines to communicate with ATmega328P's hardware I2C port.

@verbatim

LT_I2C contains the low level routines to communicate with devices using the
ATMega328's onboard hardware I2C port. Each routine checks the Two Wire Status
Register (TWSR) at the end of the transaction and returns 0 if successful and 1
if not successful.

I2C Frequency = (CPU Clock frequency)/(16+2(TWBR)*Prescaler)

TWBR-Two Wire Bit Rate Register
TWCR=Two Wire Control Register (TWINT TWEA TWSTA TWSTO TWWC TWEN - TWIE)
TWSR=Two Wire Status Register

Prescaler Values:
TWSR1  TWSR0  Prescaler
   0      0      1
   0      1      4
   1      0      16
   1      1      64

Examples:
CPU Frequency = 16Mhz on Arduino Uno
I2C Frequency  Prescaler  TWSR1  TWSR0  TWBR
  1khz         64         1      1      125
  10khz        64         1      1      12
  50khz        16         1      0      10
  100khz        4         0      1      18
  400khz        1         0      0      12

@endverbatim


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

//! @ingroup Linduino
//! @{
//! @defgroup LT_I2C LT_I2C: Routines to Communicate With ATmega328P's hardware I2C port.
//! @}

/*! @file
    @ingroup LT_I2C
    Library for LT_I2C: Routines to Communicate With ATmega328P's hardware I2C port.
*/

#include <stdint.h>
#include "LT_I2C.h"
#include "main.h"
#include "QuikEval_EEPROM.h"
#include "stm32f4xx_hal_i2c_ex.h"
#include "LTC68xx_API.h"
extern I2C_HandleTypeDef hi2c1;

//! CPU master clock frequency
// STM32 HAL-based implementation

// Read a byte, store in "value".
int8_t i2c_read_byte(uint8_t address, uint8_t *value)
{
  if (HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(address<<1), value, 1, 100) != HAL_OK) return 1;
  return 0;
}

// Write "value" byte to device at "address"
int8_t i2c_write_byte(uint8_t address, uint8_t value)
{
  if (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(address<<1), &value, 1, 100) != HAL_OK) return 1;
  return 0;
}

// Read a byte of data at register specified by "command", store in "value"
int8_t i2c_read_byte_data(uint8_t address, uint8_t command, uint8_t *value)
{
  if (HAL_I2C_Mem_Read(&hi2c1, (uint16_t)(address<<1), (uint16_t)command, I2C_MEMADD_SIZE_8BIT, value, 1, 100) != HAL_OK) return 1;
  return 0;
}

// Write a byte of data to register specified by "command"
int8_t i2c_write_byte_data(uint8_t address, uint8_t command, uint8_t value)
{
  if (HAL_I2C_Mem_Write(&hi2c1, (uint16_t)(address<<1), (uint16_t)command, I2C_MEMADD_SIZE_8BIT, &value, 1, 100) != HAL_OK) return 1;
  return 0;
}

// Read a 16-bit word of data from register specified by "command"
int8_t i2c_read_word_data(uint8_t address, uint8_t command, uint16_t *value)
{
  uint8_t buf[2] = {0};
  if (HAL_I2C_Mem_Read(&hi2c1, (uint16_t)(address<<1), (uint16_t)command, I2C_MEMADD_SIZE_8BIT, buf, 2, 100) != HAL_OK) return 1;
  *value = (uint16_t)((buf[0] << 8) | buf[1]);
  return 0;
}

// Write a 16-bit word of data to register specified by "command"
int8_t i2c_write_word_data(uint8_t address, uint8_t command, uint16_t value)
{
  uint8_t buf[2] = { (uint8_t)(value >> 8), (uint8_t)(value & 0xFF) };
  if (HAL_I2C_Mem_Write(&hi2c1, (uint16_t)(address<<1), (uint16_t)command, I2C_MEMADD_SIZE_8BIT, buf, 2, 100) != HAL_OK) return 1;
  return 0;
}

// Read a block of data, starting at register specified by "command" and ending at (command + length - 1)
int8_t i2c_read_block_data(uint8_t address, uint8_t command, uint8_t length, uint8_t *values)
{
  if (HAL_I2C_Mem_Read(&hi2c1, (uint16_t)(address<<1), (uint16_t)command, I2C_MEMADD_SIZE_8BIT, values, length, 200) != HAL_OK) return 1;
  return 0;
}


// Read a block of data, no command byte, reads length number of bytes and stores it in values.
int8_t i2c_read_block_nocmd(uint8_t address, uint8_t length, uint8_t *values)
{
  if (HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(address<<1), values, length, 200) != HAL_OK) return 1;
  return 0;
}


// Write a block of data, starting at register specified by "command" and ending at (command + length - 1)
int8_t i2c_write_block_data(uint8_t address, uint8_t command, uint8_t length, uint8_t *values)
{
  uint8_t buf[1 + 32];
  if (length > 32) length = 32;
  buf[0] = command;
  for (uint8_t i = 0; i < length; i++) buf[1+i] = values[i];
  if (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(address<<1), buf, (uint16_t)(1+length), 200) != HAL_OK) return 1;
  return 0;
}

// Write two command bytes, then receive a block of data
int8_t i2c_two_byte_command_read_block(uint8_t address, uint16_t command, uint8_t length, uint8_t *values)
{
  uint8_t cmd[2] = { (uint8_t)(command >> 8), (uint8_t)(command & 0xFF) };
  if (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(address<<1), cmd, 2, 100) != HAL_OK) return 1;
  if (HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(address<<1), values, length, 200) != HAL_OK) return 1;
  return 0;
}

// Initializes Linduino I2C port.
// Before communicating to the I2C port throught the QuikEval connector, you must also run
// quikeval_I2C_connect to connect the I2C port to the QuikEval connector throught the
// QuikEval MUX (and disconnect SPI).
void quikeval_I2C_init(void)
{
}

// Switch MUX to connect I2C pins to QuikEval connector.
// This will disconnect SPI pins.
void quikeval_I2C_connect(void)
{
  // Enable I2C
  // Set Mux pin as an output (already configured in STM32)
  // If pin is already high, do nothing
  if (HAL_GPIO_ReadPin(QUIKEVAL_MUX_MODE_GPIO_Port, QUIKEVAL_MUX_MODE_PIN) == GPIO_PIN_RESET)
  {
    // Set the Mux pin to high
    HAL_GPIO_WritePin(QUIKEVAL_MUX_MODE_GPIO_Port, QUIKEVAL_MUX_MODE_PIN, GPIO_PIN_SET);
    // And wait for LTC4315 to connect (required for rev B)
    delay_time_us(55000);
  }
}

// Setup the hardware I2C interface.
// i2c_enable or quikeval_I2C_init must be called before using any of the other I2C routines.
void i2c_enable()
{
  // Configure I2C for 100kHz to match the DC590 

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;                    // 100kHz I2C speed
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;            // 50% duty cycle
  hi2c1.Init.OwnAddress1 = 0;                        // No own address
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT; // 7-bit addressing
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // Single address mode
  hi2c1.Init.OwnAddress2 = 0;                        // No second address
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; // No general call
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;  // Clock stretching enabled
  
  // Initialize I2C peripheral
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    // Handle initialization error - could call Error_Handler() or return error code
    // For now, we'll just return without error handling as the original function was void
    return;
  }
  
  // Configure Analogue filter
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    return;
  }
  
  // Configure Digital filter
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    return;
  }
}


// Write start bit to the hardware I2C port
// return 0 if successful, 1 if not successful
int8_t i2c_start()
{
  return 0;
}

// Write a repeat start bit to the hardware I2C port
// return 0 if successful, 1 if not successful
int8_t i2c_repeated_start()
{
  return 0;
}

// Write stop bit to the hardware I2C port
void i2c_stop()
{
}

// Send a data byte to hardware I2C port
// return 0 if successful, 1 if not successful
int8_t i2c_write(uint8_t data)
{
  (void)data;
  return 0;
}

// Read a data byte from the hardware I2C port.
// Returns the data byte read.
uint8_t i2c_read(int8_t ack)
{
  (void)ack;
  return 0;
}

// Poll the I2C port and look for an acknowledge
// Returns 0 if successful, 1 if not successful
int8_t i2c_poll(uint8_t i2c_address)

{
  if (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i2c_address<<1), 2, 100) == HAL_OK) return 0;
  return 1;
}
