/*
 * Display.c
 *
 *  Created on: Apr 8, 2025
 *      Author: pavank
 */

#include "display.h"

u8g2_t u8g2;  // Display handle

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
U8X8_UNUSED void *arg_ptr) {
	switch (msg) {
	case U8X8_MSG_GPIO_AND_DELAY_INIT:
		HAL_Delay(1);
		break;
	case U8X8_MSG_DELAY_MILLI:
		HAL_Delay(arg_int);
		break;
	case U8X8_MSG_GPIO_DC:
		HAL_GPIO_WritePin(DISPLAY_IO_2_GPIO_Port, DISPLAY_IO_2_Pin, arg_int);
		break;
	case U8X8_MSG_GPIO_RESET:
		HAL_GPIO_WritePin(DISPLAY_IO_1_GPIO_Port, DISPLAY_IO_1_Pin, arg_int);
		break;
	}
	return 1;
}

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr) {
	switch (msg) {
	case U8X8_MSG_BYTE_SEND:
//		HAL_SPI_Transmit(hspi, pData, Size, Timeout)
		HAL_SPI_Transmit(&hspi3, (uint8_t*) arg_ptr, arg_int, 10000);
		break;
	case U8X8_MSG_BYTE_INIT:
		break;
	case U8X8_MSG_BYTE_SET_DC:
		HAL_GPIO_WritePin(DISPLAY_IO_2_GPIO_Port, DISPLAY_IO_2_Pin, arg_int);
		break;
	case U8X8_MSG_BYTE_START_TRANSFER:
		HAL_GPIO_WritePin(DISPLAY_CS_GPIO_Port, DISPLAY_CS_Pin, u8x8->display_info->chip_enable_level);
		break;
	case U8X8_MSG_BYTE_END_TRANSFER:
		HAL_GPIO_WritePin(DISPLAY_CS_GPIO_Port, DISPLAY_CS_Pin, u8x8->display_info->chip_disable_level);
		break;
	default:
		return 0;
	}
	return 1;
}

void Display_Init(void)
{
    // Setup display with hardware SPI
    u8g2_Setup_st7565_64128n_f(&u8g2, U8G2_R2, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);

    // Initialize display
    u8g2_InitDisplay(&u8g2);

    // Wake up display
//    u8g2_SetPowerSave(&u8g2, 0);
    Display_PowerSave(0);

    // Clear display
    u8g2_ClearDisplay(&u8g2);

    // Set contrast
    u8g2_SetContrast(&u8g2, 100);

    // Initial display update
//    u8g2_FirstPage(&u8g2);
//    u8g2_ClearBuffer(&u8g2);
//    u8g2_SendBuffer(&u8g2);
}

void Display_PowerSave(uint8_t enable) {
    u8g2_SetPowerSave(&u8g2, enable);
    HAL_GPIO_WritePin(BACKLIGHT_1_GPIO_Port, BACKLIGHT_1_Pin, !enable);
    HAL_GPIO_WritePin(BACKLIGHT_2_GPIO_Port, BACKLIGHT_1_Pin, !enable);
}

void Display_Clear(void)
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_ClearDisplay(&u8g2);
}

void Display_Update(void)
{
    u8g2_SendBuffer(&u8g2);
}

void MainTitlePage() {
	do {
		// Load and display the image
		u8g2_FirstPage(&u8g2);
		do {
			// Draw your image using U8g2 functions
//			u8g2_DrawXBM(&u8g2, 0, 0, 128, 64, logo);
		} while (u8g2_NextPage(&u8g2));

		// Wait for a moment to display the image
		HAL_Delay(500); // Adjust the delay time as needed

		// Clear the screen for the title page
		u8g2_ClearBuffer(&u8g2);

		u8g2_SetFont(&u8g2, u8g2_font_fub11_tf);
		u8g2_DrawStr(&u8g2, 42, 16, "VEGA");
		u8g2_SetFont(&u8g2, u8g2_font_fub11_tf);
		u8g2_DrawStr(&u8g2, 6, 33, "INNOVATIONS");
		u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
		u8g2_DrawStr(&u8g2, 16, 50, "BMS-TESTBENCH");
		u8g2_SetFont(&u8g2, u8g2_font_tom_thumb_4x6_tr);
		u8g2_DrawStr(&u8g2, 48, 62, "Testing..");
	    u8g2_SendBuffer(&u8g2);
	    u8g2_ClearBuffer(&u8g2);

		HAL_Delay(1000);

	} while (u8g2_NextPage(&u8g2));
	HAL_Delay(100);
}

void display_lcd(const char *message)
{
	u8g2_FirstPage(&u8g2);
	    do {
	    	u8g2_ClearBuffer(&u8g2);
			u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
			u8g2_DrawStr(&u8g2, 0, 30, message);
			u8g2_SendBuffer(&u8g2);
	    } while (u8g2_NextPage(&u8g2));
}

void display_progress_bar(const char *message, float percentage)
{
    u8g2_ClearBuffer(&u8g2);

    // Display message at top
    u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
    u8g2_DrawStr(&u8g2, 10, 20, message);

    // Draw progress bar frame
    u8g2_DrawFrame(&u8g2, 10, 30, 108, 10);

    // Calculate and draw fill bar
    uint8_t fill_width = (uint8_t)((percentage / 100.0f) * 106);
    if (fill_width > 0) {
        u8g2_DrawBox(&u8g2, 11, 31, fill_width, 8);
    }

    // Display percentage text below
    char percent_str[10];
    sprintf(percent_str, "%.1f%%", percentage);
    u8g2_DrawStr(&u8g2, 40, 50, percent_str);

    u8g2_SendBuffer(&u8g2);
}
