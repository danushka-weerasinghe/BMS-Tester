/*
 * RTC.h
 *
 *  Created on: Feb 28, 2025
 *      Author: Pavan Kavinda
 */

#ifndef RTC_MCP7940N_H
#define RTC_MCP7940N_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>

// Device address and registers
#define MCP7940N_I2C_ADDR    0xDF
#define MCP7940N_RTCSEC      0x00
#define MCP7940N_RTCMIN      0x01
#define MCP7940N_RTCHOUR     0x02
#define MCP7940N_RTCWKDAY    0x03
#define MCP7940N_RTCDATE     0x04
#define MCP7940N_RTCMTH      0x05
#define MCP7940N_RTCYEAR     0x06
#define MCP7940N_CONTROL     0x07

// Control bits
#define MCP7940N_ST          0x80    // Start oscillator
#define MCP7940N_VBATEN      0x08    // Battery enable
#define MCP7940N_OSCON       0x20    // Oscillator running
#define MCP7940N_PWRFAIL     0x10    // Power fail bit

#define TIMELAPSE_INTERVAL  5  // Minutes between timelapse captures
#define TIMELAPSE_SIZE     7   // Size of timestamp in bytes

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t dayofweek;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} DateTime;

extern DateTime time;

// Function prototypes
void RTC_Init(I2C_HandleTypeDef *hi2c);
void RTC_ReadTime(void);
void RTC_SetTime(uint8_t sec, uint8_t min, uint8_t hour,
                 uint8_t dow, uint8_t day, uint8_t month, uint8_t year);
bool RTC_GetBatteryEnabled(void);
void RTC_SetBatteryEnabled(bool enabled);
bool RTC_IsOscillatorRunning(void);
void RTC_TimeLapseInit(void);

#endif /* INC_RTC_H_ */
