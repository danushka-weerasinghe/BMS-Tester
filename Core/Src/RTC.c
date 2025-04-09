/*
 * RTC.c
 *
 *  Created on: Feb 28, 2025
 *      Author: Pavan Kavinda
 */

#include "RTC.h"
//#include "EEPROM.h"

//static uint8_t last_timelapse_minute = 0;

static I2C_HandleTypeDef *hi2c_rtc;
DateTime time;

// Convert decimal to BCD
static uint8_t DEC2BCD(int val) {
    return (uint8_t)((val / 10 * 16) + (val % 10));
}

// Convert BCD to decimal
static int BCD2DEC(uint8_t val) {
    return (int)((val / 16 * 10) + (val % 16));
}

void RTC_Init(I2C_HandleTypeDef *hi2c)
{
    hi2c_rtc = hi2c;
    uint8_t ctrl;

    // Step 1: Clear oscillator start bit first
    HAL_I2C_Mem_Read(hi2c_rtc, MCP7940N_I2C_ADDR, MCP7940N_RTCSEC, 1, &ctrl, 1, HAL_MAX_DELAY);
    ctrl &= ~MCP7940N_ST;
    HAL_I2C_Mem_Write(hi2c_rtc, MCP7940N_I2C_ADDR, MCP7940N_RTCSEC, 1, &ctrl, 1, HAL_MAX_DELAY);
    HAL_Delay(10);  // Wait for oscillator to stop

    // Step 2: Enable battery backup
    HAL_I2C_Mem_Read(hi2c_rtc, MCP7940N_I2C_ADDR, MCP7940N_RTCWKDAY, 1, &ctrl, 1, HAL_MAX_DELAY);
    ctrl |= MCP7940N_VBATEN;
    HAL_I2C_Mem_Write(hi2c_rtc, MCP7940N_I2C_ADDR, MCP7940N_RTCWKDAY, 1, &ctrl, 1, HAL_MAX_DELAY);

    // Step 3: Start oscillator
    HAL_I2C_Mem_Read(hi2c_rtc, MCP7940N_I2C_ADDR, MCP7940N_RTCSEC, 1, &ctrl, 1, HAL_MAX_DELAY);
    ctrl |= MCP7940N_ST;
    HAL_I2C_Mem_Write(hi2c_rtc, MCP7940N_I2C_ADDR, MCP7940N_RTCSEC, 1, &ctrl, 1, HAL_MAX_DELAY);

    // Step 4: Wait for oscillator to start running
    do {
        HAL_I2C_Mem_Read(hi2c_rtc, MCP7940N_I2C_ADDR, MCP7940N_RTCWKDAY, 1, &ctrl, 1, HAL_MAX_DELAY);
    } while(!(ctrl & MCP7940N_OSCON));

    // Step 5: Clear power fail flag if set
    HAL_I2C_Mem_Read(hi2c_rtc, MCP7940N_I2C_ADDR, MCP7940N_RTCWKDAY, 1, &ctrl, 1, HAL_MAX_DELAY);
    if(ctrl & MCP7940N_PWRFAIL) {
        ctrl &= ~MCP7940N_PWRFAIL;
        HAL_I2C_Mem_Write(hi2c_rtc, MCP7940N_I2C_ADDR, MCP7940N_RTCWKDAY, 1, &ctrl, 1, HAL_MAX_DELAY);
    }
}

void RTC_ReadTime(void)
{
    uint8_t data[7];

    HAL_I2C_Mem_Read(hi2c_rtc, MCP7940N_I2C_ADDR, MCP7940N_RTCSEC, 1, data, 7, HAL_MAX_DELAY);

    time.second = BCD2DEC(data[0] & 0x7F);
    time.minute = BCD2DEC(data[1] & 0x7F);
    time.hour = BCD2DEC(data[2] & 0x3F);
    time.dayofweek = BCD2DEC(data[3] & 0x07);
    time.day = BCD2DEC(data[4] & 0x3F);
    time.month = BCD2DEC(data[5] & 0x1F);
    time.year = BCD2DEC(data[6]);
}

void RTC_SetTime(uint8_t sec, uint8_t min, uint8_t hour,
                 uint8_t dow, uint8_t day, uint8_t month, uint8_t year)
{
    uint8_t data[7];

    data[0] = DEC2BCD(sec) | MCP7940N_ST;
    data[1] = DEC2BCD(min);
    data[2] = DEC2BCD(hour);
    data[3] = DEC2BCD(dow);
    data[4] = DEC2BCD(day);
    data[5] = DEC2BCD(month);
    data[6] = DEC2BCD(year);

    HAL_I2C_Mem_Write(hi2c_rtc, MCP7940N_I2C_ADDR, MCP7940N_RTCSEC, 1, data, 7, HAL_MAX_DELAY);
}

void RTC_SetTrim(int8_t trim_value) {
    uint8_t trim_reg = 0;

    if (trim_value > 0) {
        trim_reg = (uint8_t)(trim_value & 0x7F) | 0x80; // Set SIGN bit for slow oscillator
    } else {
        trim_reg = (uint8_t)((-trim_value) & 0x7F); // Clear SIGN bit for fast oscillator
    }

    HAL_I2C_Mem_Write(hi2c_rtc, MCP7940N_I2C_ADDR, MCP7940N_OSCTRIM, 1, &trim_reg, 1, HAL_MAX_DELAY);
}

void RTC_TrimByFrequency(float f_meas, float f_ideal) {
    int error_clocks = (int)(((f_ideal - f_meas) / f_ideal) * 32768 * 60);
    int8_t trim_value = error_clocks / 2; // Divide by 2 as each step adjusts by 2 cycles
    RTC_SetTrim(trim_value);
}

void RTC_TrimByDeviation(int sec_deviation, int expected_seconds) {
    float ppm_error = ((float)sec_deviation / expected_seconds) * 1e6;
    int8_t trim_value = (int8_t)((ppm_error * 32768 * 60) / (1e6 * 2));
    RTC_SetTrim(trim_value);
}

/*
void RTC_TimeLapseInit(void)
{
    static uint8_t timestamp[TIMELAPSE_SIZE];

    // Read current time
    RTC_ReadTime();

    // Check if 5 minutes have passed since last timelapse
    if ((time.minute % TIMELAPSE_INTERVAL == 0) && (time.minute != last_timelapse_minute)) {
        // Pack time data for EEPROM
        timestamp[0] = DEC2BCD(time.second);
        timestamp[1] = DEC2BCD(time.minute);
        timestamp[2] = DEC2BCD(time.hour);
        timestamp[3] = DEC2BCD(time.dayofweek);
        timestamp[4] = DEC2BCD(time.day);
        timestamp[5] = DEC2BCD(time.month);
        timestamp[6] = DEC2BCD(time.year);

        // Write to EEPROM with wear leveling
        EEPROM_WriteData(timestamp);

        // Update last timelapse time
        last_timelapse_minute = time.minute;
    }

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
	  }

}
*/
