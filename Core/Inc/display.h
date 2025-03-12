/*
 * display.h
 *
 *  Created on: Aug 8, 2024
 *      Author: DELL
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

//#include <spi.h>
#include "main.h"
#include "Logo.h"
#include "RTC.h"
//#include "U8g2 Display/u8g2.h"
extern SPI_HandleTypeDef hspi3;

static u8g2_t u8g2;

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
uint8_t errorline;

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
		u8g2_DrawStr(&u8g2, 0, 62, "Last_Time_Lapse");

	    u8g2_SetFont(&u8g2, u8g2_font_tom_thumb_4x6_tr);
	    u8g2_DrawStr(&u8g2, 70, 62, "nnnnnnnn");
	    u8g2_SendBuffer(&u8g2);
	    u8g2_ClearBuffer(&u8g2);

		HAL_Delay(1000);

	} while (u8g2_NextPage(&u8g2));
	HAL_Delay(100);
}

#endif /* INC_DISPLAY_H_ */
