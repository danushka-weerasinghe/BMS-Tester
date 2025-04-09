/*
 * Display.c
 *
 *  Created on: Apr 8, 2025
 *      Author: pavank
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "main.h"
#include "stdio.h"
//#include "Logo.h"
//#include "spi.h"
//#include "RTC.h"
#include "U8g2 Display/u8g2.h"
#include "gpio.h"

extern u8g2_t u8g2;
extern SPI_HandleTypeDef hspi3;

void Display_Init(void);
void Display_Clear(void);
void Display_Update(void);
void Display_PowerSave(uint8_t enable);

void MainTitlePage(void);
void display_lcd(const char *message);
void display_progress_bar(const char *message, float percentage);
void display_variable(int variable, uint8_t origin_x, uint8_t origin_y);

//uint8_t errorline;

#endif /* INC_DISPLAY_H_ */
