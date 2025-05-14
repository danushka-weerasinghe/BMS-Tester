/*
 * Display.h
 *
 *  Created on: Feb 26, 2025
 *      Author: Danushka Weerasinghe
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Initializes the display hardware and the u8g2 library.
 */
extern void Display_Init(void);

/**
 * @brief Displays the main title page.
 */
extern void Display_MainTitlePage(void);

/**
 * @brief Displays an alternate main title page.
 */
extern void Display_MainTitlePage_1(void);


/* New prototype to display a variable (e.g., an I2C address) */
extern void Display_Address(uint8_t address);

extern void Display_Reading(void);

void display_lcd(const char *message);

void UpdateChannel(uint8_t channel, const char *data);

void UpdateDisplayArea(uint8_t x_start, uint8_t y_start, uint8_t width, uint8_t height);

#endif /* INC_DISPLAY_H_ */
