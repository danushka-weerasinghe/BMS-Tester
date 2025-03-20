/*
 * Display.c
 *
 *  Created on: Feb 26, 2025
 *      Author: Danushka Weerasinghe
 */


#include "display.h"
#include "u8g2.h"
#include "Logo.h"
#include "main.h"  /* Provides HAL_Delay, HAL_SPI_Transmit, and HAL_GPIO_WritePin */

/*==============================================================================
                          LOCAL MACROS AND TYPES
==============================================================================*/
#define DISPLAY_UPDATE_DELAY_U32   (2U)
#define SPI_TIMEOUT_U32            (10000U)
#define CONTRAST_VALUE_U8          (120U)
#define PAGE_DELAY_U32             (1000U)
#define TEXT_DELAY_U32             (10U)
#define FINAL_PAGE_DELAY_U32       (2000U)

/*==============================================================================
                          LOCAL FUNCTION PROTOTYPES
==============================================================================*/
static uint8_t u8x8_stm32_gpio_and_delay(u8x8_t * const u8x8,
                                           uint8_t msg,
                                           uint8_t arg_int,
                                           void * const arg_ptr);

static uint8_t u8x8_byte_4wire_hw_spi(u8x8_t * const u8x8,
                                      uint8_t msg,
                                      uint8_t arg_int,
                                      void * const arg_ptr);

/*==============================================================================
                          LOCAL VARIABLES
==============================================================================*/
/* Static instance of the u8g2 display object. */
static u8g2_t u8g2;
extern SPI_HandleTypeDef hspi3;


/*==============================================================================
                          LOCAL FUNCTIONS
==============================================================================*/

/**
 * @brief  u8g2 GPIO and delay callback.
 *
 * This callback handles GPIO control and delays for the u8g2 library.
 *
 * @param  u8x8   Pointer to the u8x8 structure.
 * @param  msg    Message identifier.
 * @param  arg_int Message parameter as an integer.
 * @param  arg_ptr Message parameter as a pointer.
 * @return 1 on success.
 */
static uint8_t u8x8_stm32_gpio_and_delay(u8x8_t * const u8x8,
                                           uint8_t msg,
                                           uint8_t arg_int,
                                           void * const arg_ptr)
{
    /* Suppress unused parameter warnings */
    (void)u8x8;
    (void)arg_ptr;

    switch (msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            HAL_Delay(DISPLAY_UPDATE_DELAY_U32);
            break;
        case U8X8_MSG_DELAY_MILLI:
            HAL_Delay(arg_int);
            break;
        case U8X8_MSG_GPIO_DC:
            /* Cast arg_int to GPIO_PinState explicitly */
            HAL_GPIO_WritePin(DISPLAY_IO_2_GPIO_Port, DISPLAY_IO_2_Pin, (GPIO_PinState)arg_int);
            break;
        case U8X8_MSG_GPIO_RESET:
            HAL_GPIO_WritePin(DISPLAY_IO_1_GPIO_Port, DISPLAY_IO_1_Pin, (GPIO_PinState)arg_int);
            break;
        default:
            /* No action */
            break;
    }
    return 1U;
}

/**
 * @brief  u8g2 SPI transmission callback.
 *
 * This callback handles SPI data transmission for the u8g2 library.
 *
 * @param  u8x8   Pointer to the u8x8 structure.
 * @param  msg    Message identifier.
 * @param  arg_int Message parameter as an integer.
 * @param  arg_ptr Pointer to the data to be transmitted.
 * @return 1 on success, 0 otherwise.
 */
static uint8_t u8x8_byte_4wire_hw_spi(u8x8_t * const u8x8,
                                      uint8_t msg,
                                      uint8_t arg_int,
                                      void * const arg_ptr)
{
    uint8_t retVal = 1U;

    switch (msg)
    {
        case U8X8_MSG_BYTE_SEND:
            (void)HAL_SPI_Transmit(&hspi3, (uint8_t *)arg_ptr, arg_int, SPI_TIMEOUT_U32);
            break;
        case U8X8_MSG_BYTE_INIT:
            break;
        case U8X8_MSG_BYTE_SET_DC:
            HAL_GPIO_WritePin(DISPLAY_IO_2_GPIO_Port, DISPLAY_IO_2_Pin, (GPIO_PinState)arg_int);
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            HAL_GPIO_WritePin(DISPLAY_CS_GPIO_Port, DISPLAY_CS_Pin, u8x8->display_info->chip_enable_level);
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            HAL_GPIO_WritePin(DISPLAY_CS_GPIO_Port, DISPLAY_CS_Pin, u8x8->display_info->chip_disable_level);
            break;
        default:
            retVal = 0U;
            break;
    }
    return retVal;
}



/*==============================================================================
                          GLOBAL FUNCTIONS
==============================================================================*/

/**
 * @brief Initializes the display hardware and u8g2 library.
 */
void Display_Init(void)
{
    /* Turn on the backlight */
    HAL_GPIO_WritePin(BACKLIGHT_1_GPIO_Port, BACKLIGHT_1_Pin, GPIO_PIN_SET);

    /* Setup u8g2 for the ST7565 display.
       Adjust the setup function if your display type differs. */
    u8g2_Setup_st7565_64128n_f(&u8g2, U8G2_R2, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_ClearDisplay(&u8g2);
    u8g2_SetContrast(&u8g2, CONTRAST_VALUE_U8);
}

/**
 * @brief Displays the main title page.
 *
 * This function draws the main title page with a logo and text.
 */
void Display_MainTitlePage(void)
{
    do
    {
        /* Begin a new page */
        u8g2_FirstPage(&u8g2);
        do
        {
            /* Draw the logo image */
            u8g2_DrawXBM(&u8g2, 0, 0, 128, 64, logo);
        }
        while (u8g2_NextPage(&u8g2) != 0U);

        HAL_Delay(PAGE_DELAY_U32);

        /* Clear the buffer and draw title text */
        u8g2_ClearBuffer(&u8g2);
        u8g2_SetFont(&u8g2, u8g2_font_fub11_tf);
        u8g2_DrawStr(&u8g2, 42, 16, "VEGA");
        u8g2_SetFont(&u8g2, u8g2_font_fub11_tf);
        u8g2_DrawStr(&u8g2, 6, 33, "INNOVATIONS");
        HAL_Delay(TEXT_DELAY_U32);
        u8g2_ClearBuffer(&u8g2);
    }
    while (u8g2_NextPage(&u8g2) != 0U);

    HAL_Delay(FINAL_PAGE_DELAY_U32);

    u8g2_ClearBuffer(&u8g2);

    HAL_GPIO_WritePin(BACKLIGHT_1_GPIO_Port, BACKLIGHT_1_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Displays an alternate main title page.
 *
 * This function draws an alternate title page with different text.
 */
void Display_MainTitlePage_1(void)
{
    do
    {
        u8g2_FirstPage(&u8g2);
        do
        {
            u8g2_DrawXBM(&u8g2, 0, 0, 128, 64, logo);
        }
        while (u8g2_NextPage(&u8g2) != 0U);

        HAL_Delay(PAGE_DELAY_U32);

        u8g2_ClearBuffer(&u8g2);
        u8g2_SetFont(&u8g2, u8g2_font_fub11_tf);
        u8g2_DrawStr(&u8g2, 42, 16, "Wada");
        u8g2_SetFont(&u8g2, u8g2_font_fub11_tf);
        u8g2_DrawStr(&u8g2, 6, 33, "  Hu####o");
        HAL_Delay(TEXT_DELAY_U32);
    }
    while (u8g2_NextPage(&u8g2) != 0U);

    HAL_Delay(FINAL_PAGE_DELAY_U32);
}

/**
 * @brief  Displays an I2C address on the display.
 * @param  address: The 7-bit I2C device address to display.
 */
void Display_Address(uint8_t address)
{
    char addressBuffer[16];

    /* Format the address into a string.
       The format "0x%02X" displays the address in hexadecimal with two digits. */
    (void)sprintf(addressBuffer, "I2C Addr: 0x%02X", address);

    /* Clear the display buffer */
    u8g2_ClearBuffer(&u8g2);

    /* Set an appropriate font */
    u8g2_SetFont(&u8g2, u8g2_font_fub11_tf);

    /* Draw the formatted string at desired coordinates (adjust as needed) */
    u8g2_DrawStr(&u8g2, 0, 20, addressBuffer);

    /* Send the buffer to the display */
    u8g2_SendBuffer(&u8g2);
}

void Display_Reading(void)
{
    // Clear any previous content in the buffer
    u8g2_ClearBuffer(&u8g2);

    // Set a large font for the main numeric reading
    u8g2_SetFont(&u8g2, u8g2_font_fub30_tr);

    // Draw the numeric reading at coordinates (x=10, y=40)
    // Adjust as needed for your display
    u8g2_DrawStr(&u8g2, 10, 40, "7.43");

    // Switch to a smaller font for additional text
    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);

    // Draw smaller text lines. Adjust x/y positions as needed
    // “BM VALUE” near the bottom-left
    u8g2_DrawStr(&u8g2, 10, 60, "BM VALUE");

    // “HOLD” near the bottom-right
    // If your display is 128 wide, for example, you might place it around x=90
    u8g2_DrawStr(&u8g2, 90, 60, "HOLD");

    // Send the buffer to the display
    u8g2_SendBuffer(&u8g2);
}


