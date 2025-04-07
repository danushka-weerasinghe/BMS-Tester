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

/* Example use */

/* USER CODE BEGIN 2 */
/*
RTC_Init();
RTC_SetTime(00,29,12,5,03,03,25);
RTC_TimeLapseInit();
int BCD2DEC(uint8_t val) {
    return (int)((val / 16 * 10) + (val % 16));
}
*/
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
/*
while (1)
{

RTC_ReadTime();

do {

  char timeStr[16];
  char dateStr[16];
  sprintf(timeStr, "%02d:%02d:%02d", time.hour, time.minute, time.second);
  sprintf(dateStr, "%02d/%02d/%02d", time.day, time.month, time.year);

  u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
  u8g2_DrawStr(&u8g2, 85, 62, timeStr);
  u8g2_DrawStr(&u8g2, 0, 62, dateStr);

} while (u8g2_NextPage(&u8g2));
HAL_Delay(100);

RTC_TimeLapseCheck();

uint8_t read_buffer[7];
if (EEPROM_ReadData(read_buffer) == HAL_OK) {

  char timeStr[32];
  sprintf(timeStr, "%02d:%02d:%02d %02d/%02d/%02d",
		  BCD2DEC(read_buffer[2]),  // Hour
		  BCD2DEC(read_buffer[1]),  // Minute
		  BCD2DEC(read_buffer[0]),  // Second
		  BCD2DEC(read_buffer[4]),  // Day
		  BCD2DEC(read_buffer[5]),  // Month
		  BCD2DEC(read_buffer[6])); // Year

  u8g2_ClearBuffer(&u8g2);
  u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
  u8g2_DrawStr(&u8g2, 20, 30, timeStr);
  u8g2_SendBuffer(&u8g2);
}

*/
/* USER CODE END WHILE */
