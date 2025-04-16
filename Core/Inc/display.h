/*
 * Display.c
 *
 *  Created on: Apr 8, 2025
 *      Author: pavank
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

//#include "main.h"
#include "stdio.h"
//#include "Logo.h"
//#include "spi.h"
//#include "RTC.h"
#include "U8g2 Display/u8g2.h"
#include "gpio.h"

extern u8g2_t u8g2;
extern SPI_HandleTypeDef hspi3;

typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} TimeDate_t;

void Display_Init(void);
void Display_Clear(void);
void Display_Update(void);
void Display_PowerSave(uint8_t enable);
void DisplayLogoScreen(void);
void DisplayHomeScreen(TimeDate_t time, uint8_t mode, uint8_t id);
void MainTitlePage(void);
void display_lcd(const char *message);
void display_progress_bar(const char *message, float percentage);
void display_variable(int variable, uint8_t origin_x, uint8_t origin_y);

//uint8_t errorline;

#endif /* INC_DISPLAY_H_ */
