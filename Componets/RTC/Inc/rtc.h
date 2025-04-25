/*
 * rtc.h
 *
 *  Created on: Feb 28, 2025
 *      Author: Pavan Kavinda
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

extern I2C_HandleTypeDef hi2c1;

/* I2C device addresses for RTC */
#define MCP7940N_I2C_ADDR    0xDF	/* RTCSEC register */
#define MCP7940N_RTCSEC      0x00	/* RTCSEC register */
#define MCP7940N_RTCMIN      0x01	/* RTCSEC register */
#define MCP7940N_RTCHOUR     0x02	/* RTCSEC register */
#define MCP7940N_RTCWKDAY    0x03	/* RTCSEC register */
#define MCP7940N_RTCDATE     0x04	/* RTCSEC register */
#define MCP7940N_RTCMTH      0x05	/* RTCSEC register */
#define MCP7940N_RTCYEAR     0x06	/* RTCSEC register */
#define MCP7940N_CONTROL     0x07	/* RTCSEC register */
#define MCP7940N_OSCTRIM	 0x08	/* Oscillator Trim register */

/* RTC control bits */
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
void RTC_Init(void);
void RTC_ReadTime(void);
void RTC_SetTime(uint8_t sec, uint8_t min, uint8_t hour,
                 uint8_t dow, uint8_t day, uint8_t month, uint8_t year);
bool RTC_GetBatteryEnabled(void);
void RTC_SetBatteryEnabled(bool enabled);
bool RTC_IsOscillatorRunning(void);
//void RTC_TimeLapseInit(void);
void RTC_SetTrim(int8_t trim_value);
void RTC_TrimByFrequency(float f_meas, float f_ideal);
void RTC_TrimByDeviation(int sec_deviation, int expected_seconds);

#endif /* INC_RTC_H_ */
