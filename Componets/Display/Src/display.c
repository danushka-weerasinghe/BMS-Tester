/*
 * Display.c
 *
 *  Created on: Feb 26, 2025
 *      Author: Danushka Weerasinghe
 */

#include "display.h"
#include "u8g2.h"
#include "Logo.h"

#include "main.h"
#include "main_data.h"

#include "BMS_test_protocol.h"
#include "INA229.h"
#include "LTC68xx_API.h"
#include "string.h"
#include "mcu.h"
#include "config.h"
#include "math.h"
#include <float.h>   // for FLT_MAX
#include <stdio.h>
#include "gpio_expander.h"
#include "Temp_controller.h"


/*==============================================================================
                          LOCAL FUNCTION PROTOTYPES
==============================================================================*/
static uint8_t u8x8_stm32_gpio_and_delay(u8x8_t * const u8x8, uint8_t msg, uint8_t arg_int, void * const arg_ptr);
static uint8_t u8x8_byte_4wire_hw_spi(u8x8_t * const u8x8, uint8_t msg, uint8_t arg_int, void * const arg_ptr);

/*==============================================================================
                          LOCAL VARIABLES
==============================================================================*/
u8g2_t u8g2;
static bms_data_t bms_data;
extern const Cell_Config cell_configs[];

static display_state_t display_state = {
    .current_screen = SCREEN_SAVER,
    .previous_screen = MAIN_MENU,
	.power_state = POWER_NORMAL,
	.power_timer = 0,
    .selected_item = 0,
    .selected_cell = 0,
    .selected_temp_card = 0,
    .scroll_offset = 0,
    .target_voltage = 3.3f,
    .target_resistance = 10,
    .test_sequence_running = 0,
    .current_test_step = 0,
    .screensaver_timer = 0,
    .display_contrast = CONTRAST_VALUE_U8,
    .auto_refresh = 1,
    .show_warnings = 1
};

// ---- Temp Card value source (UI-agnostic) ----
static float TempCard_GetDisplayC(uint8_t id)
{
    if (id >= MAX_TEMP_CARDS) return 0.0f;

    // If  Temp_controller exposes a readback (recommended):
    //   extern float TempController_GetTempC(uint8_t id);
    //   return TempController_GetTempC(id);
    //
    // Until then, fall back to whatever the system already maintains:
    return bms_data.temp_card_C[id];
}

extern SPI_HandleTypeDef hspi3;

/* Menu item arrays */
typedef struct {
    const uint8_t *font;
    uint16_t icon;
    const char *text;
} MenuItem;

typedef struct {
	const uint8_t *font;
	uint16_t icon;
}IconItem;

static const IconItem icon_items[] = {
		{u8g2_font_open_iconic_arrow_1x_t,78},
		{u8g2_font_open_iconic_all_1x_t,67},
		{u8g2_font_open_iconic_embedded_1x_t,71}

};

// Replace string array with MenuItem array:
static const MenuItem main_menu_items[] = {
    {u8g2_font_open_iconic_embedded_2x_t, 68, "Status"},      // Heart/pulse icon
    {u8g2_font_open_iconic_thing_2x_t, 80, "Control"},     // Power button
    {u8g2_font_open_iconic_embedded_2x_t, 72, "Test & Cal"},  // Task/checklist
    {u8g2_font_open_iconic_embedded_2x_t, 66, "Settings"},    // Wrench icon
    {u8g2_font_open_iconic_thing_2x_t,77, "Diagnostics"}, // Flash/alert
    {u8g2_font_open_iconic_all_2x_t, 188, "About"}             // Info icon
};

uint8_t menu_count = sizeof(main_menu_items) / sizeof(main_menu_items[0]);

static const char* status_items[] = {
    "Cell Monitor",
    "Pack Overview",
    "Temperature",
    "Current/Voltage",
};

static const char* control_items[] = {
    "Cell Balance",
    "Set Voltage",
    "Set Temperature",
};

static const char* test_items[] = {
    "Auto Sequence",
    "Manual Test",
    "Calibration",
    "Open Wire Test",
    "Back"
};

static const char* settings_items[] = {
    "System Info",
    "Display",
    "Communication",
};

//MenuItem main_menu_items[] = {
//
//};

/*==============================================================================
                          LOCAL FUNCTIONS
==============================================================================*/
static uint8_t u8x8_stm32_gpio_and_delay(u8x8_t * const u8x8, uint8_t msg, uint8_t arg_int, void * const arg_ptr)
{
    (void)u8x8;
    (void)arg_ptr;

    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            HAL_Delay(DISPLAY_UPDATE_DELAY_U32);
            break;
        case U8X8_MSG_DELAY_MILLI:
            HAL_Delay(arg_int);
            break;
        case U8X8_MSG_GPIO_DC:
            HAL_GPIO_WritePin(DISPLAY_IO_2_GPIO_Port, DISPLAY_IO_2_Pin, (GPIO_PinState)arg_int);
            break;
        case U8X8_MSG_GPIO_RESET:
            HAL_GPIO_WritePin(DISPLAY_IO_1_GPIO_Port, DISPLAY_IO_1_Pin, (GPIO_PinState)arg_int);
            break;
        default:
            break;
    }
    return 1U;
}

static uint8_t u8x8_byte_4wire_hw_spi(u8x8_t * const u8x8, uint8_t msg, uint8_t arg_int, void * const arg_ptr)
{
    uint8_t retVal = 1U;

    switch (msg) {
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
void Display_Init(void)
{
    HAL_GPIO_WritePin(BACKLIGHT_1_GPIO_Port, BACKLIGHT_1_Pin, GPIO_PIN_SET);

    u8g2_Setup_st7565_64128n_f(&u8g2, U8G2_R2, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_ClearDisplay(&u8g2);
    u8g2_SetContrast(&u8g2, display_state.display_contrast);

    uint32_t current_time = HAL_GetTick();

    display_state.screensaver_timer = current_time;
    bms_data.last_update_time = current_time;

    display_state.auto_refresh = 1;


}

void Display_MainTitlePage(void)
{
    // Show logo
    u8g2_FirstPage(&u8g2);
    do {
        u8g2_DrawXBM(&u8g2, 0, 0, 128, 64, logo);
    } while (u8g2_NextPage(&u8g2));

    HAL_Delay(PAGE_DELAY_U32);

    // Show text
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_fub11_tf);
    u8g2_DrawStr(&u8g2, 42, 16, "VEGA");
    u8g2_SetFont(&u8g2, u8g2_font_fub11_tf);
    u8g2_DrawStr(&u8g2, 6, 33, "INNOVATIONS");
    u8g2_SendBuffer(&u8g2);

    HAL_Delay(TEXT_DELAY_U32);

    // Set to main menu - this should work now
    Display_SetScreen(MAIN_MENU);
}



void Display_Update(void)
{
    uint32_t current_time = HAL_GetTick();
    uint32_t idle_time = current_time - display_state.screensaver_timer;

    Display_UpdateBlink();

    // Check for screensaver timeout
    // Power management state machine
        switch (display_state.power_state) {
            case POWER_NORMAL:
                if (idle_time > SCREENSAVER_TIMEOUT_MS) {
                    display_state.power_state = POWER_SCREENSAVER;
                    display_state.power_timer = current_time;
                    Display_SetScreen(SCREEN_SAVER);
                }
                break;

            case POWER_SCREENSAVER:
                if (idle_time > ( BACKLIGHT_OFF_TIMEOUT_MS)) {
                    display_state.power_state = POWER_BACKLIGHT_OFF;
                    // Turn off backlight
                    HAL_GPIO_WritePin(BACKLIGHT_1_GPIO_Port, BACKLIGHT_1_Pin, GPIO_PIN_RESET);
                }
                break;

            case POWER_BACKLIGHT_OFF:
                if (idle_time > (BACKLIGHT_OFF_TIMEOUT_MS + SCREEN_OFF_TIMEOUT_MS)) {
                    display_state.power_state = POWER_SCREEN_OFF;
                    // Turn off screen completely
                    u8g2_SetPowerSave(&u8g2, 1);  // Enable power save mode
                    u8g2_ClearDisplay(&u8g2);     // Clear display
                }
                break;

            case POWER_SCREEN_OFF:
                // Screen is off, do nothing until button press
                return;  // Don't render anything
        }

        // Only update BMS data and render if screen is active
        if (display_state.power_state != POWER_SCREEN_OFF) {
            // Update BMS data if auto refresh is enabled
            if (display_state.auto_refresh &&
				(current_time - bms_data.last_update_time)> 1000) {
                Display_UpdateBMSData();
            }
        }



    // Render current screen
    switch (display_state.current_screen) {
        case SCREEN_SAVER:
            Display_Screensaver();
            break;
        case MAIN_MENU:
            Display_MainMenu();
            break;
        case LIVE_DATA_MENU:       //status menu
            Display_LiveDataMenu();
            break;
        case CELL_MONITOR_SCREEN:
            Display_CellMonitor();
            break;
        case CELL_MONITOR_CSU:
        	Display_CellMonitorCSU();
        	break;
        case CARD_CELL_SCREEN:
        	Display_CellMonitorCard();
        	break;
        case CELL_MONITOR_DCCSU:
        	Display_CellMonitorDaisyCSU();
        	break;
        case PACK_OVERVIEW_SCREEN:
            Display_PackOverview();
            break;
        case TEMPERATURE_SCREEN:
            Display_TemperatureScreen();
            break;
        case CELL_TEMPERATURE_SCREEN:
            Display_CellTemperatureScreen();
            break;
       case CARD_TEMPERATURE_SCREEN:
            Display_CardTemperatureScreen();
            break;
       case SYSTEM_TEMPERATURE_SCREEN:
            Display_SystemTemperatureScreen();
            break;
        case CURRENT_VOLTAGE_SCREEN:
            Display_CurrentVoltageScreen();
            break;
        case CONTROL_MENU:
            Display_ControlMenu();
            break;
        case CELL_BALANCE_MENU:
            Display_CellBalanceMenu();
            break;
        case VOLTAGE_SET_MENU:
             Display_VoltageSetMenu();
             break;
        case TEMP_SET_MENU:
            Display_TempSetMenu();
            break;
        case TEST_MENU:
            Display_TestMenu();
            break;
        case AUTO_SEQUENCE_SCREEN:
            Display_AutoSequence();
            break;
        case SETTINGS_MENU:
            Display_SettingsMenu();
            break;
        case SYSTEM_INFO_SCREEN:
            Display_SystemInfo();
            break;
        case ABOUT_SCREEN:
            Display_AboutScreen();
            break;
        default:
            Display_MainMenu();
            break;
    }
}

void Display_MainMenu(void)
{
    uint8_t menu_count = sizeof(main_menu_items) / sizeof(main_menu_items[0]);
    uint8_t visible_items = 3;  // Show only 3 icons at a time

    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("MAIN MENU");

    // CIRCULAR SCROLL: Always center the selected item with wraparound
    int8_t start_index = display_state.selected_item - 1;


    // Calculate spacing for 3 icons
    uint8_t iconSpacing = u8g2_GetDisplayWidth(&u8g2) / visible_items;
    uint8_t yPos = 49;

    // Draw the 3 visible icons with circular wraparound
    for (uint8_t i = 0; i < visible_items; i++) {
        // CIRCULAR INDEX CALCULATION: Handle wraparound using modulo
        int8_t raw_index = start_index + i;
        uint8_t item_index;

        // Handle negative wraparound (e.g., -1 becomes 5 for 6-item menu)
        if (raw_index < 0) {
            item_index = menu_count + raw_index;
        }
        // Handle positive wraparound (e.g., 6 becomes 0 for 6-item menu)
        else if (raw_index >= menu_count) {
            item_index = raw_index - menu_count;
        }
        // Normal case
        else {
            item_index = raw_index;
        }

        // Calculate position
        uint8_t xPos = (iconSpacing * i) + (iconSpacing / 2);
        uint8_t iconYOffset = 0;

        // Determine if this is the selected item and if it's in the middle position
        bool is_selected = (item_index == display_state.selected_item);
        bool is_middle_position = (i == 1); // Middle position in our 3-item display

        // Choose the appropriate font size
        const uint8_t *icon_font;
        if (is_middle_position) {
            // For middle icon, use 4x_t version (larger)
            if (main_menu_items[item_index].font == u8g2_font_open_iconic_embedded_2x_t) {
                icon_font = u8g2_font_open_iconic_embedded_4x_t;
                yPos=yPos+5;
            } else if (main_menu_items[item_index].font == u8g2_font_open_iconic_all_2x_t) {
                icon_font = u8g2_font_open_iconic_all_4x_t;
                yPos=yPos+5;
            } else if (main_menu_items[item_index].font == u8g2_font_open_iconic_thing_2x_t){
            	icon_font = u8g2_font_open_iconic_thing_4x_t;
            	yPos=yPos+5;
            } else {

                icon_font = main_menu_items[item_index].font;
                yPos=yPos+5;// Fallback
            }
            iconYOffset = 0; // Move up slightly to center better
        } else {
            // For side icons, use original 2x_t version (smaller)
            icon_font = main_menu_items[item_index].font;
            yPos=yPos-5;
        }

        // Set font and calculate icon width for centering
        u8g2_SetFont(&u8g2, icon_font);
        uint8_t icon_width = u8g2_GetGlyphWidth(&u8g2, main_menu_items[item_index].icon);
        xPos -= icon_width / 2; // Center the icon

        // Set draw color (inverse for selected item)
        if (is_selected) {
            u8g2_SetDrawColor(&u8g2, 0);  // Inverted
            // Draw background box for selected item
            uint8_t box_height = is_middle_position ? 32 : 16; // Larger box for 4x icons
            u8g2_DrawBox(&u8g2, xPos - 2, yPos + iconYOffset - box_height + 4,
                         icon_width + 4, box_height);
            u8g2_SetDrawColor(&u8g2, 1);
        } else {
            u8g2_SetDrawColor(&u8g2, 1);  // Normal
        }

        // Draw the icon
        u8g2_DrawGlyph(&u8g2, xPos, yPos + iconYOffset, main_menu_items[item_index].icon);

        u8g2_SetDrawColor(&u8g2, 1);  // Reset color
    }

    // Draw the text for the selected menu item
    const char *selectedText = main_menu_items[display_state.selected_item].text;
    uint8_t textWidth = u8g2_GetStrWidth(&u8g2, selectedText);
    uint8_t textX = (u8g2_GetDisplayWidth(&u8g2) / 2) - (textWidth / 2);
    uint8_t textY = yPos+5;

    // Draw a line below the menu text
    uint8_t lineY = textY + 2;
    u8g2_DrawHLine(&u8g2, 0, lineY, u8g2_GetDisplayWidth(&u8g2));

    // Draw navigation indicators
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mr);
    const char *words[] = {"Enter","<",main_menu_items[display_state.selected_item].text,">", "Back"};
    uint8_t wordSpacing = u8g2_GetDisplayWidth(&u8g2) / 5;
    uint8_t wordY = lineY + 9;

    for (uint8_t i = 0; i < 5; i++) {
        uint8_t wordX = (wordSpacing * i) + (wordSpacing / 2) - (u8g2_GetStrWidth(&u8g2, words[i]) / 2);
        u8g2_DrawStr(&u8g2, wordX, wordY, words[i]);
    }

    // Draw scroll indicators - ALWAYS show since it's circular
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mr);
    u8g2_DrawStr(&u8g2, 2, yPos-5, "<");  // Always show left arrow
    u8g2_DrawStr(&u8g2, u8g2_GetDisplayWidth(&u8g2) - 7, yPos-5, ">");  // Always show right arrow

    // Draw item counter (e.g., "2/6")
    char counter[8];
    sprintf(counter, "%d/%d", display_state.selected_item + 1, menu_count);
    uint8_t counterWidth = u8g2_GetStrWidth(&u8g2, counter);
    u8g2_DrawStr(&u8g2, u8g2_GetDisplayWidth(&u8g2) - counterWidth - 2,10, counter);

    // Send the buffer to the display
    u8g2_SendBuffer(&u8g2);
}

void Display_Screensaver(void)
{
    static uint32_t last_animation = 0;
    static uint8_t animation_step = 0;
    uint32_t current_time = HAL_GetTick();


    char status_text[20];
        	        switch (animation_step % 3) {
        	            case 0: strcpy(status_text, "Monitoring..."); break;
        	            case 1: strcpy(status_text, "Ready");         break;
        	            case 2:strcpy(status_text, "Standby");        break;
        	        }
    // Animate BMS logo or status



    // Only animate if backlight is on
    if (display_state.power_state == POWER_BACKLIGHT_OFF) {
        // Show static screensaver when backlight is off
    	u8g2_ClearBuffer(&u8g2);

    	u8g2_SetFont(&u8g2, u8g2_font_fub11_tf);
    	u8g2_DrawStr(&u8g2, 15, 25, "BMS TESTER");

    	u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
    	u8g2_DrawStr(&u8g2, 40, 45, status_text);



    }

    if ((current_time - last_animation) > 1000) {
        u8g2_ClearBuffer(&u8g2);

        u8g2_SetFont(&u8g2, u8g2_font_fub11_tf);
        u8g2_DrawStr(&u8g2, 15, 25, "BMS TESTER");

        u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
        u8g2_DrawStr(&u8g2, 40, 45, status_text);


        // Show time if available
        char time_str[10];
        sprintf(time_str, "%02lu:%02lu", (HAL_GetTick() / 60000) % 24, (HAL_GetTick() / 1000) % 60);
        u8g2_DrawStr(&u8g2, 85, 60, time_str);

        // Show power state indicator
        switch (display_state.power_state) {
            case POWER_SCREENSAVER:
                u8g2_DrawStr(&u8g2, 2, 60, "SCR");
                break;
            case POWER_BACKLIGHT_OFF:
                u8g2_DrawStr(&u8g2, 2, 60, "DIM");
                break;
        }

        u8g2_SendBuffer(&u8g2);
        last_animation = current_time;
        animation_step++;
    }
}

void Display_LiveDataMenu(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("STATUS");

    uint8_t menu_count = sizeof(status_items) / sizeof(status_items[0]);

    for (uint8_t i = 0; i < menu_count; i++) {
        Display_DrawMenuItem(i, status_items[i], i == display_state.selected_item);
    }

    u8g2_SendBuffer(&u8g2);
}

void Display_CellMonitor(void)
{


	{
	    u8g2_ClearBuffer(&u8g2);
	    Display_DrawHeader("CELL MONITOR");

	    // cell monitoring methods
	    static const char* cell_methods[] = {
	        "CSU",
	        "Cell Card",
	        "DC-CSU",
	    };

	    uint8_t menu_count = sizeof(cell_methods) / sizeof(cell_methods[0]);

	    for (uint8_t i = 0; i < menu_count; i++) {
	        Display_DrawMenuItem(i, cell_methods[i], i == display_state.selected_item);
	    }

	    u8g2_SendBuffer(&u8g2);
	}
}

void Display_CellMonitorCSU(void)
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawLine(&u8g2, 0, 12, 127, 12);

    display_state_t* state = Display_GetState();

    // Decide which CSU set we're on
    uint8_t csu_type  = (state->selected_cell < 12) ? 1 : 2;   // 1=12-CSU, 2=11-CSU
    uint8_t max_cells = (csu_type == 1) ? 12 : 11;

    // Select the correct LTC chain and take a fresh reading
    DC_chain = (csu_type == 1) ? 1 : 2;
    int8_t err = cell_voltage_read();
    if (err != 0) { HAL_Delay(5);} //(void)cell_voltage_read(); }

    // Header
    char header[24];
    const char* csu_name = (csu_type == 1) ? "12-CSU" : "11-CSU";
    snprintf(header, sizeof(header), "%s - CELL MON", csu_name);
    Display_DrawHeader(header);

    // Paging calculations (4 rows per screen)
    const uint8_t cells_per_screen = 4;
    uint8_t active_cell = (csu_type == 1) ? state->selected_cell : (uint8_t)(state->selected_cell - 12);
    if (active_cell >= max_cells) active_cell = (uint8_t)(max_cells - 1);
    uint8_t start_cell = (uint8_t)((active_cell / cells_per_screen) * cells_per_screen);

    char cell_text[16];
    char voltage_text[16];

    // IMPORTANT: the LTC driver fills BMS_IC[0] for the selected DC_chain
    const uint8_t BMS_IC_NUM = 0;

    for (uint8_t i = 0; i < cells_per_screen && (start_cell + i) < max_cells; ++i) {
        uint8_t local_idx  = (uint8_t)(start_cell + i);     // 0..11 (12-CSU) or 0..10 (11-CSU)
        uint8_t y_pos      = (uint8_t)(24 + (i * 12));
        bool     is_selected = (local_idx == active_cell);

        // Labeling (keeps your cell_configs mapping)
        uint8_t global_cfg_idx = (csu_type == 1) ? local_idx : (uint8_t)(12 + local_idx);
        const Cell_Config* cfg = &cell_configs[global_cfg_idx];
        uint8_t display_cell_number = (uint8_t)(cfg->cell_id + 1);  // for "Cxx:" text

        // Read voltage from the *local* channel of the active chain
        // local_idx is guaranteed in-range: 0..11 or 0..10
        uint16_t raw = BMS_IC[BMS_IC_NUM].cells.c_codes[local_idx];
        float voltage = (float)raw * 0.0001f;

        if (raw == 0 || voltage <= 0.0001f || voltage > 5.0f) {
            strcpy(voltage_text, "N/C");
        } else {
            Display_FormatVoltage(voltage_text, voltage);
        }

        // Layout tweaks for selected row
        uint8_t cell_text_x    = is_selected ?  8 : 10;
        uint8_t voltage_text_x = is_selected ? 32 : 38;
        uint8_t bar_frame_x    = 87;
        uint8_t bar_fill_x     = 87;

        // Draw labels
        snprintf(cell_text, sizeof(cell_text), "C%02u:", (unsigned)display_cell_number);
        u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
        u8g2_DrawStr(&u8g2, cell_text_x,    y_pos, cell_text);
        u8g2_DrawStr(&u8g2, voltage_text_x, y_pos, voltage_text);

        // Warning icon & bar (only if we have a plausible voltage)
        const float MIN_VOLTAGE = 2.0f;
        const float MAX_VOLTAGE = 4.2f;
        const uint8_t BAR_MAX_WIDTH = 30;

        if (raw != 0 && voltage > 0.0001f && voltage <= 5.0f) {
            if ((voltage < 2.1f) || (voltage > 4.1f)) {
                u8g2_SetFont(&u8g2, icon_items[2].font);
                u8g2_DrawGlyph(&u8g2, 75, y_pos, icon_items[2].icon);
            }
            float pct = (voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE);
            if (pct < 0.0f) pct = 0.0f;
            if (pct > 1.0f) pct = 1.0f;
            uint8_t bar_width = (uint8_t)(pct * BAR_MAX_WIDTH);

            u8g2_DrawFrame(&u8g2, bar_frame_x, y_pos - 6, 30, 6);
            if (bar_width > 0) {
                u8g2_DrawBox(&u8g2, bar_fill_x, y_pos - 5, bar_width, 4);
            }
        } else {
            // draw empty bar frame for N/C
            u8g2_DrawFrame(&u8g2, bar_frame_x, y_pos - 6, 30, 6);
        }

        // Selection arrow
        if (is_selected) {
            u8g2_SetFont(&u8g2, icon_items[0].font);
            u8g2_DrawGlyph(&u8g2, 0, y_pos, icon_items[0].icon);
        }
    }

    // Footer: show page-relative position
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mr);
    char nav_text[20];
    snprintf(nav_text, sizeof(nav_text), "Cell %u/%u", (unsigned)(active_cell + 1), (unsigned)max_cells);
    uint8_t nav_width = u8g2_GetStrWidth(&u8g2, nav_text);
    u8g2_DrawStr(&u8g2, (uint8_t)(128 - nav_width - 2), 10, nav_text);

    u8g2_SendBuffer(&u8g2);
}




void Display_CellMonitorCard(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("CELL CARDS");


    char cell_text[16];
    char voltage_text[10];

    display_state_t* state = Display_GetState();

    // Use cells 0-23 for card monitoring (INA229 based)
    uint8_t max_cells = MAX_CELL_COUNT; // 24 cells
    uint8_t active_cell = state->selected_cell;
    if (active_cell >= max_cells) active_cell = max_cells - 1;

    // Show 4 cells per screen
    uint8_t cells_per_screen = 4;
    uint8_t start_cell = (active_cell / cells_per_screen) * cells_per_screen;

    for (uint8_t i = 0; i < cells_per_screen && (start_cell + i) < max_cells; i++) {
        uint8_t array_index = start_cell + i;  // Array index for cell_configs
        const Cell_Config* config = &cell_configs[array_index];
        uint8_t cell_id = config->cell_id;  // Actual cell ID from struct member

        bool is_selected = (array_index == active_cell);  // Selection based on array index

        uint8_t cell_text_x = is_selected ? 8 : 10;
	    uint8_t voltage_text_x = is_selected ? 32 : 38;
//            uint8_t current_x = is_selected ? 65 : 70;
	    uint8_t bar_frame_x = 	87;			//is_selected ? 87 : 97;
	    uint8_t bar_fill_x =    87;

        uint8_t y_pos = 24 + (i * 12);
        uint8_t ina_index = config->ina_index;


        //char current_text[16];

        sprintf(cell_text, "C%02u:",(cell_id + 1));  // Use actual cell_id from struct

        // Check if reading is valid
        float voltage =0.0f;
        if (ina_index< NUM_INA229 && INA229_Readings[ina_index].reading_valid) {
            voltage = INA229_Readings[ina_index].voltage_V;

            Display_FormatVoltage(voltage_text, voltage);
        } else {
            sprintf(voltage_text, "N/C");
//            sprintf(current_text, "N/C");
        }

        // Draw cell info
        u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
        u8g2_DrawStr(&u8g2, cell_text_x, y_pos, cell_text);
        u8g2_DrawStr(&u8g2, voltage_text_x, y_pos, voltage_text);
     // u8g2_DrawStr(&u8g2, 85, y_pos, current_text);


        const float MIN_VOLTAGE = 2.0f;
		const float MAX_VOLTAGE = 4.2f;
		const uint8_t BAR_MAX_WIDTH = 30;

		float voltage_percentage = (voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE);
		if (voltage_percentage < 0.0f) voltage_percentage = 0.0f;
		if (voltage_percentage > 1.0f) voltage_percentage = 1.0f;

		if( (voltage < 2.1f) || (voltage > 4.1f) ){
			u8g2_SetFont(&u8g2, icon_items[2].font);
			u8g2_DrawGlyph(&u8g2, 75, y_pos, icon_items[2].icon);
		}
		uint8_t bar_width = (uint8_t)(voltage_percentage * BAR_MAX_WIDTH);

		// Draw voltage bar frame
		u8g2_DrawFrame(&u8g2, bar_frame_x, y_pos - 6, 30, 6);

		// Draw filled portion if there's any voltage
		if (bar_width > 0) {
			u8g2_DrawBox(&u8g2, bar_fill_x, y_pos - 5, bar_width, 4);
		}

		// Draw selection indicator arrow for selected cell
		if (is_selected) {
			u8g2_SetFont(&u8g2, icon_items[0].font);
			u8g2_DrawGlyph(&u8g2, 0, y_pos, icon_items[0].icon);
		}



    }
    // Footer info
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mr);

    // Navigation counter - use actual cell_id from active config
    const Cell_Config* active_config = &cell_configs[active_cell];
    char nav_text[16];
    sprintf(nav_text, "Cell %u/%u", (unsigned)(active_config->cell_id + 1), (unsigned)max_cells);
    uint8_t nav_width = u8g2_GetStrWidth(&u8g2, nav_text);
    u8g2_DrawStr(&u8g2, 128 - nav_width - 2, 10, nav_text);

    u8g2_SendBuffer(&u8g2);
}



void Display_CellMonitorDaisyCSU(void)
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawLine(&u8g2, 0, 12, 127, 12);

    display_state_t* state = Display_GetState();

    /* DC-CSU / Daisy mode uses up to 23 cells (1..23 in protocol) */
    const uint8_t max_cells = 23;

    /* Ensure the daisy/DC chain is enabled for this view (keeps parity with getter case 0x05) */
    DC_chain = 1;
    cell_voltage_read(); /* refresh voltages for DC-CSU view */

    /* Active cell (0-based). Clamp if out of range. */
    uint8_t active_cell = state->selected_cell;
    if (active_cell >= max_cells) active_cell = max_cells - 1;

    /* Pagination - 4 cells per page (same style as other monitors) */
    uint8_t cells_per_screen = 4;
    uint8_t start_cell = (active_cell / cells_per_screen) * cells_per_screen;

    char voltage_text[16];
    char cell_text[16];

    for (uint8_t i = 0; i < cells_per_screen && (start_cell + i) < max_cells; i++) {
    	 uint8_t array_index = start_cell + i;
			const Cell_Config* config = &cell_configs[array_index];
			uint8_t cell_id = config->cell_id;  // Actual cell ID from struct member
			uint8_t y_pos = 24 + (i * 12);

			bool is_selected = (array_index == active_cell);  // Selection based on array index

			uint8_t cell_text_x = is_selected ? 8 : 10;
			uint8_t voltage_text_x = is_selected ? 32 : 38;
	//            uint8_t current_x = is_selected ? 65 : 70;
			uint8_t bar_frame_x = 	87;			//is_selected ? 87 : 97;
			uint8_t bar_fill_x =    87;


        uint8_t CELL_ID_2 = 0;
        uint8_t BMS_IC_NUM  = 0;
        if (cell_id > 11) {
            CELL_ID_2 = cell_id - 12;
            BMS_IC_NUM = 1;
        } else {
            CELL_ID_2 = cell_id;
            BMS_IC_NUM = 0;
        }




        uint16_t raw = BMS_IC[BMS_IC_NUM].cells.c_codes[CELL_ID_2];
        float voltage = (float)raw * 0.0001f;

	   if (raw == 0 || voltage <= 0.0001f || voltage > 5.0f) {
		   strcpy(voltage_text, "N/C");
	   } else {
		   Display_FormatVoltage(voltage_text, voltage);
	   }

//        if (BMS_IC_NUM >= TOTAL_IC || CELL_ID_2 >= BMS_IC[0].ic_reg.cell_channels) {
//            /* Show N/C if invalid */
//            sprintf(voltage_text, "N/C");
//        } else {
//            uint16_t raw_voltage = BMS_IC[BMS_IC_NUM].cells.c_codes[CELL_ID_2];
//            float voltage = (float)raw_voltage * 0.0001f;
//
//            if (voltage > 4.5f || voltage < 0.0f) {
//                sprintf(voltage_text, "N/C");
//            } else {
//                sprintf(voltage_text, "%.3fV", voltage);
//            }
//        }


        /* Label (use 1-based numbering for user) */
        sprintf(cell_text, "C%02u:", (unsigned)(cell_id + 1));

        /* Draw label and voltage */
        u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
			  u8g2_DrawStr(&u8g2, cell_text_x, y_pos, cell_text);
			  u8g2_DrawStr(&u8g2, voltage_text_x, y_pos, voltage_text);
		   // u8g2_DrawStr(&u8g2, 85, y_pos, current_text);


       			const float MIN_VOLTAGE = 2.0f;
				const float MAX_VOLTAGE = 4.2f;
				const uint8_t BAR_MAX_WIDTH = 30;

				float voltage_percentage = (voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE);
				if (voltage_percentage < 0.0f) voltage_percentage = 0.0f;
				if (voltage_percentage > 1.0f) voltage_percentage = 1.0f;

				if( (voltage < 2.1f) || (voltage > 4.1f) ){
					u8g2_SetFont(&u8g2, icon_items[2].font);
					u8g2_DrawGlyph(&u8g2, 75, y_pos, icon_items[2].icon);
				}
				uint8_t bar_width = (uint8_t)(voltage_percentage * BAR_MAX_WIDTH);

				// Draw voltage bar frame
				u8g2_DrawFrame(&u8g2, bar_frame_x, y_pos - 6, 30, 6);

				// Draw filled portion if there's any voltage
				if (bar_width > 0) {
					u8g2_DrawBox(&u8g2, bar_fill_x, y_pos - 5, bar_width, 4);
				}


        /* Selection indicator (same icon usage pattern as other monitors) */
        if (is_selected) {
            u8g2_SetFont(&u8g2, icon_items[0].font);
            u8g2_DrawGlyph(&u8g2, 0, y_pos, icon_items[0].icon);
        }
    }

    /* Footer: show DC-CSU label and chain state */
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mr);
    char footer_text[32];
    sprintf(footer_text, "DC-CSU DC_ch:%u", (unsigned)DC_chain);
    u8g2_DrawStr(&u8g2, 2, 10, footer_text);

    /* Navigation counter "Cell X/Y" on right */
    char nav_text[16];
    sprintf(nav_text, "Cell %u/%u", (unsigned)(active_cell + 1), (unsigned)max_cells);
    uint8_t nav_width = u8g2_GetStrWidth(&u8g2, nav_text);
    u8g2_DrawStr(&u8g2, 128 - nav_width - 2, 10, nav_text);

    u8g2_SendBuffer(&u8g2);
}


void Display_SetDaisyChain(void)
{

}




void Display_PackOverview(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("PACK OVERVIEW");

    char buffer[32];

    // Pack voltage (pre-computed by Update_PackMetrics)
    Display_FormatVoltage(buffer, bms_data.pack_voltage);
    Display_DrawValue(0, 20, "Pack V:", bms_data.pack_voltage, "V");

    // Pack current (pre-computed or 0 if not available)
    Display_FormatCurrent(buffer, bms_data.pack_current);
    Display_DrawValue(0, 30, "Pack I:", bms_data.pack_current, "A");

    // Find Min/Max valid cell voltages by scanning cell_configs -> INA229_Readings
    float min_voltage = FLT_MAX;
    float max_voltage = -FLT_MAX;
    int min_idx = -1;
    int max_idx = -1;

    for (uint8_t i = 0; i < MAX_CELL_COUNT; i++) {
        const Cell_Config* config = &cell_configs[i];
        if (config == NULL) continue;
        if (config->ina_index >= NUM_INA229) continue;

        INA229_Reading* r = &INA229_Readings[config->ina_index];
        if (!r->reading_valid) continue;

        float v = r->voltage_V;

        if (v < min_voltage) {
            min_voltage = v;
            min_idx = i;
        }
        if (v > max_voltage) {
            max_voltage = v;
            max_idx = i;
        }
    }

    // Draw Min cell
    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
    u8g2_DrawStr(&u8g2, 0, 45, "Min:");
    if (min_idx >= 0) {
        // Use cell_id if you want the logical cell number, otherwise show array index+1
        uint8_t disp_cell_num = cell_configs[min_idx].cell_id + 1; // +1 for human-readable
        snprintf(buffer, sizeof(buffer), "C%02d:%.2fV", disp_cell_num, min_voltage);
        u8g2_DrawStr(&u8g2, 30, 45, buffer);
    } else {
        u8g2_DrawStr(&u8g2, 30, 45, "N/C");
    }

    // Draw Max cell
    u8g2_DrawStr(&u8g2, 0, 55, "Max:");
    if (max_idx >= 0) {
        uint8_t disp_cell_num = cell_configs[max_idx].cell_id + 1;
        snprintf(buffer, sizeof(buffer), "C%02d:%.2fV", disp_cell_num, max_voltage);
        u8g2_DrawStr(&u8g2, 30, 55, buffer);
    } else {
        u8g2_DrawStr(&u8g2, 30, 55, "N/C");
    }

    // Draw pack status indicator (unchanged)
    u8g2_DrawCircle(&u8g2, 110, 50, 8, U8G2_DRAW_ALL);
    if (bms_data.error_flags == 0) {
        u8g2_DrawDisc(&u8g2, 110, 50, 6, U8G2_DRAW_ALL); // Filled circle for OK
    }

    u8g2_SendBuffer(&u8g2);
}





// Replace the existing Display_TemperatureScreen() function with this menu:
void Display_TemperatureScreen(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("TEMPERATURE");

    // Temperature monitoring methods
    static const char* temp_methods[] = {
        "CSU",
        "Temp Card",
        "DC-CSU",
    };

    uint8_t menu_count = sizeof(temp_methods) / sizeof(temp_methods[0]);

    for (uint8_t i = 0; i < menu_count; i++) {
        Display_DrawMenuItem(i, temp_methods[i], i == display_state.selected_item);
    }

    u8g2_SendBuffer(&u8g2);
}

/* ---------- CELL TEMPERATURE SCREEN (CSU ) ---------- */
void Display_CellTemperatureScreen(void)
{
    u8g2_ClearBuffer(&u8g2);

    display_state_t* state = Display_GetState();

    // Map selection to CSU group: 0..2 => 12-CSU, 3..5 => 11-CSU
    const uint8_t csu_type = (state->selected_temp_card < 3) ? 1 : 2; // 1=12-CSU, 2=11-CSU
    const uint8_t max_temp_sensors = 3;                               // 3 sensors per CSU

    // Pull fresh data for the selected chain (matches your voltage pattern)
    DC_chain = csu_type;
    temparature_data_read();

    // Active (0..2) inside the selected CSU
    uint8_t active_temp_sensor = (csu_type == 1)
                               ? state->selected_temp_card
                               : (uint8_t)(state->selected_temp_card - 3);
    if (active_temp_sensor >= max_temp_sensors) active_temp_sensor = (uint8_t)(max_temp_sensors - 1);

    // Header
    const char* csu_name = (csu_type == 1) ? "12-CSU" : "11-CSU";
    char title[24];
    snprintf(title, sizeof(title), "%s TEMP", csu_name);
    Display_DrawHeader(title);

    // We show all 3 on a single page
    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);

    for (uint8_t row = 0; row < max_temp_sensors; ++row) {
        const bool is_selected = (row == active_temp_sensor);
        const uint8_t y = (uint8_t)(24 + row * 12);

        // Your temperature lives in BMS_IC[0] for the currently selected DC_chain
        const uint8_t BMS_IC_NUM = 0;
        const uint8_t TEMP_ID    = row;

        if (BMS_IC_NUM >= TOTAL_IC || TEMP_ID >= TEMP_PER_IC) continue;

        const int8_t tC = BMS_IC[BMS_IC_NUM].heat.temp[TEMP_ID];

        // Validity & text
        char label[16], val[12];
        snprintf(label, sizeof(label), "T%02u:", (unsigned)(row + 1));

        const bool valid = !(tC < -25 || tC > 100);
        if (valid) {
            snprintf(val, sizeof(val), "%d\xB0""C", (int)tC);
        } else {
            strcpy(val, "N/C");
        }

        // Row highlight when selected
        if (is_selected) {
            u8g2_SetDrawColor(&u8g2, 0);
            u8g2_DrawBox(&u8g2, 6, y - 8, 118, 10);
            u8g2_SetDrawColor(&u8g2, 1);
        }

        // Column positions (kept consistent with your other pages)
        const uint8_t label_x = is_selected ? 8  : 10;
        const uint8_t val_x   = is_selected ? 45 : 47;
        const uint8_t bar_x   = is_selected ? 87 : 89;

        u8g2_DrawStr(&u8g2, label_x, y, label);
        u8g2_DrawStr(&u8g2, val_x,   y, val);

        // Bar (-20..100 °C → 0..30 px) only if valid
        const int8_t  T_MIN = -20, T_MAX = 100;
        const uint8_t BW = 30;
        u8g2_DrawFrame(&u8g2, bar_x, (uint8_t)(y - 6), BW, 6);

        if (valid) {
            float pct = (float)(tC - T_MIN) / (float)(T_MAX - T_MIN);
            if (pct < 0) pct = 0; if (pct > 1) pct = 1;
            const uint8_t w = (uint8_t)(pct * (BW - 2)); // keep inside frame
            if (w > 0) u8g2_DrawBox(&u8g2, (uint8_t)(bar_x + 1), (uint8_t)(y - 5), w, 4);

            // Optional warning icon (like voltage page)
            if (tC < 0 || tC > 60) {
                u8g2_SetFont(&u8g2, icon_items[2].font);
                u8g2_DrawGlyph(&u8g2, 75, y, icon_items[2].icon);
                u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
            }
        }

        // Selection arrow
        if (is_selected) {
            u8g2_SetFont(&u8g2, icon_items[0].font);
            u8g2_DrawGlyph(&u8g2, 0, y, icon_items[0].icon);
            u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
        }
    }

    // Footer
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mr);
    char nav[16];
    snprintf(nav, sizeof(nav), "Temp %u/%u", (unsigned)(active_temp_sensor + 1), (unsigned)max_temp_sensors);
    const uint8_t nav_w = u8g2_GetStrWidth(&u8g2, nav);
    u8g2_DrawStr(&u8g2, (uint8_t)(128 - nav_w - 2), 10, nav);

    char dc_txt[24];
    snprintf(dc_txt, sizeof(dc_txt), "%s DC_ch:%u", csu_name, (unsigned)DC_chain);
    u8g2_DrawStr(&u8g2, 2, 10, dc_txt);

    u8g2_SendBuffer(&u8g2);
}



/* ---------- CARD TEMPERATURE SCREEN (temperature-card / external NTCs) ---------- */
void Display_CardTemperatureScreen(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("TEMP CARDS");

    static const char* card_names[MAX_TEMP_CARDS] = {
        "C12-T1", "C12-T2", "C12-T3",
        "C11-T1", "C11-T2", "C11-T3"
    };

    const uint8_t per_screen = 3;
    const uint8_t start = (uint8_t)((display_state.selected_temp_card / per_screen) * per_screen);

    for (uint8_t i = 0; i < per_screen && (start + i) < MAX_TEMP_CARDS; ++i) {
        const uint8_t card_id = (uint8_t)(start + i);
        const uint8_t y       = (uint8_t)(24 + i * 12);
        const bool is_sel     = (card_id == display_state.selected_temp_card);

        // Fetch value through helper (ready for Temp_controller readback)
        float tC = TempCard_GetDisplayC(card_id);

        // Clamp to plausible range for visualization only
        if (tC < -40.0f) tC = -40.0f;
        if (tC > 125.0f) tC = 125.0f;

        // Row style
        const uint8_t name_x = is_sel ? 8 : 10;
        const uint8_t val_x  = is_sel ? 50 : 55;
        const uint8_t bar_x  = is_sel ? 85 : 90;

        char name[16], val[16];
        snprintf(name, sizeof(name), "%s:", card_names[card_id]);
        snprintf(val,  sizeof(val),  "%.1f\xB0""C", tC);

        u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
        u8g2_DrawStr(&u8g2, name_x, y, name);
        u8g2_DrawStr(&u8g2, val_x,  y, val);

        // Bar: map -20..80 °C → 0..30 px (matches your comment, and keeps 30px frame)
        const float T_MIN = -20.0f, T_MAX =  80.0f;
        float pct = (tC - T_MIN) / (T_MAX - T_MIN);
        if (pct < 0) pct = 0; if (pct > 1) pct = 1;
        const uint8_t BW = 30, w = (uint8_t)(pct * (BW - 2));

        u8g2_DrawFrame(&u8g2, bar_x, (uint8_t)(y - 6), BW, 6);
        if (w > 0) u8g2_DrawBox(&u8g2, (uint8_t)(bar_x + 1), (uint8_t)(y - 5), w, 4);

        // Selection arrow
        if (is_sel) {
            u8g2_SetFont(&u8g2, icon_items[0].font);
            u8g2_DrawGlyph(&u8g2, 0, y, icon_items[0].icon);
        }
    }

    // Footer (like voltage)
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mr);
    char counter[20];
    snprintf(counter, sizeof(counter), "Card %u/%u", (unsigned)(display_state.selected_temp_card + 1), (unsigned)MAX_TEMP_CARDS);
    const uint8_t w = u8g2_GetStrWidth(&u8g2, counter);
    u8g2_DrawStr(&u8g2, (uint8_t)(u8g2_GetDisplayWidth(&u8g2) - w - 2), 10, counter);

    u8g2_SendBuffer(&u8g2);
}

/* ---------- SYSTEM TEMPERATURE SCREEN (daisy-chain / board-level) ---------- */

void Display_SystemTemperatureScreen(void)
{
    u8g2_ClearBuffer(&u8g2);

    // Like your DC-CSU voltage page: one chain with two ICs (12-CSU & 11-CSU)
    DC_chain = 1;
    temparature_data_read();   // should populate BMS_IC[0] and BMS_IC[1]

    display_state_t* state = Display_GetState();

    const uint8_t max_sensors = 6;   // 3 per CSU * 2
    uint8_t active = state->selected_temp_card;
    if (active >= max_sensors) active = (uint8_t)(max_sensors - 1);

    Display_DrawHeader("DC-CSU TEMP");

    const uint8_t per_screen = 3;
    const uint8_t start = (uint8_t)((active / per_screen) * per_screen);

    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);

    for (uint8_t i = 0; i < per_screen && (start + i) < max_sensors; ++i) {
        const uint8_t idx   = (uint8_t)(start + i); // 0..5
        const uint8_t y     = (uint8_t)(24 + i * 12);
        const bool is_sel   = (idx == active);

        // idx 0..2 -> BMS_IC[0] (12-CSU), idx 3..5 -> BMS_IC[1] (11-CSU)
        const uint8_t BMS_IC_NUM = (idx < 3) ? 0 : 1;
        const uint8_t TEMP_ID    = (idx < 3) ? idx : (uint8_t)(idx - 3);

        if (BMS_IC_NUM >= TOTAL_IC || TEMP_ID >= TEMP_PER_IC) continue;

        const int8_t tC = BMS_IC[BMS_IC_NUM].heat.temp[TEMP_ID];

        // Labels (CSU name + T#)
        const char* csu_name = (BMS_IC_NUM == 0) ? "12" : "11";
        char label[16], val[12];
        snprintf(label, sizeof(label), "%s T%u:", csu_name, (unsigned)(TEMP_ID + 1));

        const bool valid = !(tC < -25 || tC > 100);
        if (valid) snprintf(val, sizeof(val), "%d\xB0""C", (int)tC);
        else       strcpy(val, "N/C");

        // Row highlight
        if (is_sel) {
            u8g2_SetDrawColor(&u8g2, 0);
            u8g2_DrawBox(&u8g2, 6, y - 8, 118, 10);
            u8g2_SetDrawColor(&u8g2, 1);
        }

        const uint8_t label_x = is_sel ? 8  : 10;
        const uint8_t val_x   = is_sel ? 45 : 47;
        const uint8_t bar_x   = is_sel ? 87 : 89;

        u8g2_DrawStr(&u8g2, label_x, y, label);
        u8g2_DrawStr(&u8g2, val_x,   y, val);

        // Bar if valid
        const int8_t  T_MIN = -20, T_MAX = 100;
        const uint8_t BW = 30;
        u8g2_DrawFrame(&u8g2, bar_x, (uint8_t)(y - 6), BW, 6);
        if (valid) {
            float pct = (float)(tC - T_MIN) / (float)(T_MAX - T_MIN);
            if (pct < 0) pct = 0; if (pct > 1) pct = 1;
            const uint8_t w = (uint8_t)(pct * (BW - 2));
            if (w > 0) u8g2_DrawBox(&u8g2, (uint8_t)(bar_x + 1), (uint8_t)(y - 5), w, 4);

            if (tC < 0 || tC > 60) {
                u8g2_SetFont(&u8g2, icon_items[2].font);
                u8g2_DrawGlyph(&u8g2, 75, y, icon_items[2].icon);
                u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
            }
        }

        if (is_sel) {
            u8g2_SetFont(&u8g2, icon_items[0].font);
            u8g2_DrawGlyph(&u8g2, 0, y, icon_items[0].icon);
            u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
        }
    }

    // Footer
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mr);
    char nav[16];
    snprintf(nav, sizeof(nav), "Temp %u/%u", (unsigned)(active + 1), (unsigned)max_sensors);
    const uint8_t nav_w = u8g2_GetStrWidth(&u8g2, nav);
    u8g2_DrawStr(&u8g2, (uint8_t)(128 - nav_w - 2), 10, nav);

    char dc_txt[24];
    snprintf(dc_txt, sizeof(dc_txt), "DC_ch:%u", (unsigned)DC_chain);
    u8g2_DrawStr(&u8g2, 2, 10, dc_txt);

    u8g2_SendBuffer(&u8g2);
}


//void Display_TemperatureScreen(void)
//{
//    u8g2_ClearBuffer(&u8g2);
//    Display_DrawHeader("TEMPERATURE");
//
//
//
//
//    char temp_text[16];
//    char value_text[10];
//
//    for (uint8_t i = 0; i < MAX_TEMP_CARDS; i++) {
//        uint8_t y_pos = 18 + (i * 8);
//
//        sprintf(temp_text, "T%d:", i + 1);
//        u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
//        u8g2_DrawStr(&u8g2, 0, y_pos, temp_text);
//
//        Display_FormatTemperature(value_text, bms_data.cell_temperatures[i]);
//        u8g2_DrawStr(&u8g2, 25, y_pos, value_text);
//
//        // Draw temperature bar
//        uint8_t temp_bar = (uint8_t)((bms_data.cell_temperatures[i] + 20) / 120.0f * 40); // -20°C to 100°C range
//        u8g2_DrawFrame(&u8g2, 70, y_pos - 6, 42, 6);
//        u8g2_DrawBox(&u8g2, 71, y_pos - 5, temp_bar, 4);
//
//        // Highlight selected temp card
//        if (i == display_state.selected_temp_card) {
//            u8g2_DrawFrame(&u8g2, 0, y_pos - 8, 128, 10);
//        }
//    }
//
//    u8g2_SendBuffer(&u8g2);
//}

void Display_ControlMenu(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("CONTROL");

    uint8_t menu_count = sizeof(control_items) / sizeof(control_items[0]);

    for (uint8_t i = 0; i < menu_count; i++) {
        Display_DrawMenuItem(i, control_items[i], i == display_state.selected_item);
    }

    u8g2_SendBuffer(&u8g2);
}

void Display_TestMenu(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("TEST & CAL");

    uint8_t menu_count = sizeof(test_items) / sizeof(test_items[0]);

    for (uint8_t i = 0; i < menu_count; i++) {
        Display_DrawMenuItem(i, test_items[i], i == display_state.selected_item);
    }

    u8g2_SendBuffer(&u8g2);
}

void Display_SettingsMenu(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("SETTINGS");

    uint8_t menu_count = sizeof(settings_items) / sizeof(settings_items[0]);

    for (uint8_t i = 0; i < menu_count; i++) {
        Display_DrawMenuItem(i, settings_items[i], i == display_state.selected_item);
    }

    u8g2_SendBuffer(&u8g2);
}

/* Additional screen implementations continue... */
void Display_AboutScreen(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("ABOUT");

    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
    u8g2_DrawStr(&u8g2, 10, 25, "BMS Test System");
    u8g2_DrawStr(&u8g2, 10, 35, "Version: 1.0.1");
    u8g2_DrawStr(&u8g2, 10, 45, "VEGA Innovations");
    u8g2_DrawStr(&u8g2, 10, 55, "2025");

    u8g2_SendBuffer(&u8g2);
}

/*==============================================================================
                          UTILITY FUNCTIONS
==============================================================================*/
void Display_CurrentVoltageScreen(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("Power ");

    char buffer[20];

    // Display pack voltage and current
    Display_FormatVoltage(buffer, bms_data.pack_voltage);
    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
    u8g2_DrawStr(&u8g2, 0, 25, "Pack Voltage:");
    u8g2_DrawStr(&u8g2, 80, 25, buffer);

    Display_FormatCurrent(buffer, bms_data.pack_current);
    u8g2_DrawStr(&u8g2, 0, 35, "Pack Current:");
    u8g2_DrawStr(&u8g2, 80, 35, buffer);

    // Display power calculation
    sprintf(buffer, "%.1fW", bms_data.pack_voltage * bms_data.pack_current);
    u8g2_DrawStr(&u8g2, 0, 45, "Power:");
    u8g2_DrawStr(&u8g2, 80, 45, buffer);

    u8g2_SendBuffer(&u8g2);
}

void Display_CellBalanceMenu(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("CELL BALANCE");

    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
    u8g2_DrawStr(&u8g2, 10, 30, "Cell Balance Menu");
    u8g2_DrawStr(&u8g2, 10, 40, "Use buttons to");
    u8g2_DrawStr(&u8g2, 10, 50, "toggle cell balance");

    u8g2_SendBuffer(&u8g2);
}

void Display_VoltageSetMenu(void)
{
    display_state_t* state = Display_GetState();

    // If we're in adjust mode, render the persistent adjust UI (so blink is visible)
    if (state->voltage_mode == VOLTAGE_MODE_ADJUST_VOLTAGE) {
        // Draw Adjust screen using the shared adjust function (no direct screen switch)
        Display_VoltageSetAdjust(VOLTAGE_MODE_ADJUST_VOLTAGE);
        return;
    }

    u8g2_ClearBuffer(&u8g2);

    char title[20];
    snprintf(title, sizeof(title), "SET VOLTAGE C%d", state->selected_cell + 1);
    Display_DrawHeader(title);

    // We'll draw the two rows manually so we can invert/blink the Cell row.
    char cell_text[32];
    char target_text[32];
    snprintf(cell_text, sizeof(cell_text), "Cell:   %d", state->selected_cell + 1);
    snprintf(target_text, sizeof(target_text), "Target: %.2f V", state->target_voltage);

    // Coordinates / metrics (match the style used in Display_VoltageSetAdjust)
    const int text_x = 5;

    // Cell row: text_y = 25, inverted when blink_on
    const int cell_text_y = 25;
    const int cell_row_top = cell_text_y - 8;   // ~row top to cover the row (12px height)
    const int row_h = 12;

    if (state->blink_on) {
        // inverted row + caret for Cell
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_DrawBox(&u8g2, 0, cell_row_top, u8g2_GetDisplayWidth(&u8g2), row_h);
        u8g2_SetDrawColor(&u8g2, 0);
        u8g2_DrawStr(&u8g2, text_x, cell_text_y, cell_text);
        int str_w = u8g2_GetStrWidth(&u8g2, cell_text);
        // small caret/box after text
        u8g2_DrawBox(&u8g2, text_x + str_w + 2, cell_text_y - 2, 6, 2);
    } else {
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_DrawStr(&u8g2, text_x, cell_text_y, cell_text);
    }

    // Target row (not blinking in this menu)
    const int target_text_y = 38;
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawStr(&u8g2, text_x, target_text_y, target_text);

    // Footer help
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mr);
    u8g2_DrawStr(&u8g2, 5, 50, "Enter: Set Voltage");
    u8g2_DrawStr(&u8g2, 5, 58, "Up/Down: Change Cell");

    // ensure draw color restored
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SendBuffer(&u8g2);
}


// Temporary robust blinking UI for testing — replace your existing Display_VoltageSetAdjust()
void Display_VoltageSetAdjust(voltage_mode_t voltage_mode)
{
    display_state_t* state = Display_GetState();

    // Display_UpdateBlink() is already called at the top of Display_Update()
    // so blink state is kept in state->blink_on

    u8g2_ClearBuffer(&u8g2);

    // Header
    char title[20];
    snprintf(title, sizeof(title), "SET VOLTAGE C%d", state->selected_cell + 1);
    Display_DrawHeader(title);

    // Reuse same visuals as before: Cell row static, Target row blinks (when in adjust mode)
    char cell_text[32];

    snprintf(cell_text, sizeof(cell_text), "Cell:   %d", state->selected_cell + 1);


    char value_text[32];
    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
    snprintf(value_text, sizeof(value_text), "Target: %.2f V", state->target_voltage);

    // Coordinates for the target row (same as earlier)
    const int row_x = 0;
    const int row_y_top = 29;
    const int row_h = 12;
    const int text_x = 5;
    const int text_y = 38;

    // Use centralized blink flag (state->blink_on)
    if (state->blink_on) {
        // inverted row + caret
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_DrawBox(&u8g2, row_x, row_y_top, u8g2_GetDisplayWidth(&u8g2), row_h);
        u8g2_SetDrawColor(&u8g2, 0);
        u8g2_DrawStr(&u8g2, text_x, text_y, value_text);
        int str_w = u8g2_GetStrWidth(&u8g2, value_text);
        u8g2_DrawBox(&u8g2, text_x + str_w + 2, text_y - 2, 6, 2);
    } else {
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_DrawStr(&u8g2, text_x, text_y, value_text);
    }

    const int cell_text_y = 25;
	u8g2_SetDrawColor(&u8g2, 1);
	u8g2_DrawStr(&u8g2, text_x, cell_text_y, cell_text);

    // Footer help
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mr);
    u8g2_DrawStr(&u8g2, 5, 50, "Back: Set Cell");
	u8g2_DrawStr(&u8g2, 5, 58, "Up/Down: Change Voltage");
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SendBuffer(&u8g2);
}


void Display_TempSetMenu(void) {
    display_state_t *state = Display_GetState();

    // If we're in adjust mode, render the persistent adjust UI so blink is visible
    if (state->temperature_mode == TEMPERATURE_MODE_ADJUST_TEMPERATURE) {
        // Draw Adjust screen using the shared adjust function
        Display_TempSetAdjust(TEMPERATURE_MODE_ADJUST_TEMPERATURE);
        return;
    }

    u8g2_ClearBuffer(&u8g2);

    char title[20];
    snprintf(title, sizeof(title), "SET TEMP Card %d", state->selected_temp_card + 1);
    Display_DrawHeader(title);

    // Draw the two rows manually so we can invert/blink the Card row
    char cardtext[32];
    char targettext[32];

    snprintf(cardtext, sizeof(cardtext), "Card %d", state->selected_temp_card + 1);
    snprintf(targettext, sizeof(targettext), "Target %.1f°C", state->target_temperature);

    // Coordinates - metrics match the style used in DisplayVoltageSetAdjust
    const int textx = 5;
    // Card row (texty = 25), inverted when blinkon
    const int cardtexty = 25;
    const int cardrowtop = cardtexty - 8; // row top to cover the row (~12px height)
    const int rowh = 12;

    if (state->blink_on) {
        // inverted row caret for Card
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_DrawBox(&u8g2, 0, cardrowtop, u8g2_GetDisplayWidth(&u8g2), rowh);
        u8g2_SetDrawColor(&u8g2, 0);
        u8g2_DrawStr(&u8g2, textx, cardtexty, cardtext);

        // small caret/box after text
        int strw = u8g2_GetStrWidth(&u8g2, cardtext);
        u8g2_DrawBox(&u8g2, textx + strw + 2, cardtexty - 2, 6, 2);
    } else {
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_DrawStr(&u8g2, textx, cardtexty, cardtext);
    }

    // Target row (not blinking in this menu)
    const int targettexty = 38;
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawStr(&u8g2, textx, targettexty, targettext);

    // Footer help
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mr);
    u8g2_DrawStr(&u8g2, 5, 50, "Enter: Set Temp");
    u8g2_DrawStr(&u8g2, 5, 58, "Up/Down: Change Card");

    // ensure draw color restored
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SendBuffer(&u8g2);
}

void Display_TempSetAdjust(temperature_mode_t temperature_mode) {
    display_state_t *state = Display_GetState();

    // DisplayUpdateBlink is already called at the top of DisplayUpdate so blink state is kept in state->blinkon
    u8g2_ClearBuffer(&u8g2);

    // Header
    char title[20];
    snprintf(title, sizeof(title), "SET TEMP Card %d", state->selected_temp_card + 1);
    Display_DrawHeader(title);

    // Reuse same visuals as before
    // Card row (static), Target row (blinks when in adjust mode)
    char cardtext[32];
    snprintf(cardtext, sizeof(cardtext), "Card %d", state->selected_temp_card + 1);
    char valuetext[32];
    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
    snprintf(valuetext, sizeof(valuetext), "Target %.1f°C", state->target_temperature);

    // Coordinates for the target row (same as earlier)
    const int rowx = 0;
    const int rowytop = 29;
    const int rowh = 12;
    const int textx = 5;
    const int texty = 38;

    // Use centralized blink flag state->blinkon
    if (state->blink_on) {
        // inverted row caret
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_DrawBox(&u8g2, rowx, rowytop, u8g2_GetDisplayWidth(&u8g2), rowh);
        u8g2_SetDrawColor(&u8g2, 0);
        u8g2_DrawStr(&u8g2, textx, texty, valuetext);

        // small caret after text
        int strw = u8g2_GetStrWidth(&u8g2, valuetext);
        u8g2_DrawBox(&u8g2, textx + strw + 2, texty - 2, 6, 2);
    } else {
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_DrawStr(&u8g2, textx, texty, valuetext);
    }

    // Card row (static)
    const int cardtexty = 25;
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawStr(&u8g2, textx, cardtexty, cardtext);

    // Footer help
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mr);
    u8g2_DrawStr(&u8g2, 5, 50, "Back: Set Card");
    u8g2_DrawStr(&u8g2, 5, 58, "Up/Down: Change Temp");

    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SendBuffer(&u8g2);
}


void Display_AutoSequence(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("AUTO SEQUENCE");

    char buffer[2];
    sprintf(buffer, "Step: %d", display_state.current_test_step);

    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
    u8g2_DrawStr(&u8g2, 10, 25, "Auto Test Running");
    u8g2_DrawStr(&u8g2, 10, 35, buffer);

    if (display_state.test_sequence_running) {
        u8g2_DrawStr(&u8g2, 10, 45, "Status: Running");
    } else {
        u8g2_DrawStr(&u8g2, 10, 45, "Status: Stopped");
    }

    u8g2_SendBuffer(&u8g2);
}

void Display_SystemInfo(void)
{
    u8g2_ClearBuffer(&u8g2);
    Display_DrawHeader("SYSTEM INFO");

    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
    u8g2_DrawStr(&u8g2, 10, 25, "STM32F429 MCU");
    u8g2_DrawStr(&u8g2, 10, 35, "24 INA229 Sensors");
    u8g2_DrawStr(&u8g2, 10, 45, "LTC6811 BMS IC");
    u8g2_DrawStr(&u8g2, 10, 55, "Press button to exit");

    u8g2_SendBuffer(&u8g2);
}



void Display_DrawHeader(const char* title)
{
    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
    u8g2_DrawStr(&u8g2, 0, 10, title);
    u8g2_DrawLine(&u8g2, 0, 12, 127, 12);
}

void Display_DrawMenuItem(uint8_t index, const char* text, uint8_t selected)
{
    uint8_t y_pos = 26 + (index * 10);

    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);

    if (selected) {
        u8g2_DrawBox(&u8g2, 0, y_pos - 8, 128, 10);
        u8g2_SetDrawColor(&u8g2, 0);
        u8g2_DrawStr(&u8g2, 5, y_pos, text);
        u8g2_SetDrawColor(&u8g2, 1);
    } else {
        u8g2_DrawStr(&u8g2, 5, y_pos, text);
    }
}

void Display_DrawValue(uint8_t x, uint8_t y, const char* label, float value, const char* unit)
{
    char buffer[20];
    u8g2_SetFont(&u8g2, u8g2_font_6x10_mr);
    u8g2_DrawStr(&u8g2, x, y, label);
    sprintf(buffer, "%.2f%s", value, unit);
    u8g2_DrawStr(&u8g2, x + 50, y, buffer);
}
void Display_FormatVoltage(char* buffer, float voltage) {
    sprintf(buffer, "%.3fV", voltage);

}

void Display_FormatCurrent(char* buffer, float current)
{
    sprintf(buffer, "%.2fA", current);
}

void Display_FormatTemperature(char* buffer, float temperature)
{
    sprintf(buffer, "%.1f°C", temperature);
}

void Display_SetScreen(menu_screen_t screen)
{
    display_state.previous_screen = display_state.current_screen;
    display_state.current_screen = screen;
    display_state.selected_item = 0;
    display_state.scroll_offset = 0;
    display_state.screensaver_timer = HAL_GetTick();

    // Reset power state if changing screens
    if (screen != SCREEN_SAVER) {
        display_state.power_state = POWER_NORMAL;

    }

    if (screen == CELL_MONITOR_DCCSU) {
		Set_Daisy_Chain(1);
		Display_ShowConfirmation("Daisy Chain On");

		HAL_Delay(5); /* short settle */
//		cell_voltage_read();
	}


	/* Leaving DC-CSU screen */
	if (display_state.previous_screen == CELL_MONITOR_DCCSU && screen != CELL_MONITOR_DCCSU) {
		Set_Daisy_Chain(0);
		Display_ShowConfirmation("Daisy Chain Off");

		HAL_Delay(5);
		//cell_voltage_read();
	}
}

void Display_WakeUp(void)
{
    // Reset power state to normal
    display_state.power_state = POWER_NORMAL;
    display_state.screensaver_timer = HAL_GetTick();

    // Turn on backlight
    HAL_GPIO_WritePin(BACKLIGHT_1_GPIO_Port, BACKLIGHT_1_Pin, GPIO_PIN_SET);

    // Turn on screen
    u8g2_SetPowerSave(&u8g2, 0);  // Disable power save mode
    u8g2_SetContrast(&u8g2, display_state.display_contrast);

    // Return to main menu
    Display_SetScreen(display_state.previous_screen);  //
}


void Display_UpdateBMSData(void)
{



    uint32_t current_time = HAL_GetTick();

    cell_voltage_read();



    // Update all INA229 readings for individual cells (0-23)
    for (uint8_t cell = 0; cell < MAX_CELL_COUNT; cell++) {
    	//if (cell >= MAX_CELL_COUNT) break;

    	HAL_GPIO_WritePin(CELL12_TEMP_01_LED_GPIO_Port, CELL12_TEMP_01_LED_Pin, GPIO_PIN_RESET);
    	            HAL_GPIO_WritePin(GPIOC, CELL12_TEMP_02_LED_Pin, GPIO_PIN_RESET);
    	            HAL_GPIO_WritePin(GPIOF, CELL12_TEMP_03_LED_Pin, GPIO_PIN_RESET);

    		        for (int cell = CELL_1; cell <= CELL_24+1; cell++) {
    		        	Set_LED_status(cell, ON);
    		        }

    		        HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_01_LED_Pin, GPIO_PIN_RESET);
    		        HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_02_LED_Pin, GPIO_PIN_RESET);
    		        HAL_GPIO_WritePin(GPIOH, CELL11_TEMP_03_LED_Pin, GPIO_PIN_RESET);

        const Cell_Config* config = &cell_configs[cell];

        if (config->ina_index >= NUM_INA229) {
                    // mark invalid and continue
                    INA229_Readings[config->ina_index].reading_valid = 0; // only if index valid
                    continue;
            }




        // Use existing Get_INA functions that handle chip select properly
        INA229_Readings[config->ina_index].voltage_V = Get_INA_Voltage(config);



        INA229_Readings[config->ina_index].current_mA = Get_INA_Current(config) * 1000.0f; // Convert A to mA
     //   INA229_Readings[config->ina_index].temperature_C = Get_INA_Temp(config);
        INA229_Readings[config->ina_index].cell_id = config->cell_id;  // Use struct member, not loop variable
        INA229_Readings[config->ina_index].reading_valid = 1;
        INA229_Readings[config->ina_index].last_update_time = current_time;


    }

    bms_data.last_update_time = current_time;
    Update_PackMetrics();

}


/* Updated Display_UpdateBMSData */

#define DEBUG_INA_PRINT 0

#if DEBUG_INA_PRINT
extern UART_HandleTypeDef huart1; // adapt if your debug UART variable is different
static void dbg_printf(const char *fmt, ...)
{
    char tmp[128];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);
    HAL_UART_Transmit(&huart1, (uint8_t*)tmp, strlen(tmp), 100);
}
#else
#define dbg_printf(...)
#endif
//
//void Display_UpdateBMSData(void)
//{
//    uint32_t now = HAL_GetTick();
//
//    for (uint8_t cell = 0; cell < MAX_CELL_COUNT; cell++) {
//        const Cell_Config* cfg = &cell_configs[cell];
//        if (cfg == NULL) continue;
//
//        // sanity: ensure mapping inside bounds
//        if (cfg->ina_index >= NUM_INA229) {
//            dbg_printf("Cell %u has invalid ina_index %u\r\n", (unsigned)cell, (unsigned)cfg->ina_index);
//            continue;
//        }
//
//        // read raw register directly to validate transaction
//        INA229_Handle ina = get_ina_handle(cfg->ina_index);
//
//        // If you don't have direct access to INA229_readReg, call a wrapper that returns raw register
//        uint32_t raw = INA229_readReg(ina, INA229_vbus_register); // returns 32-bit raw reg
//        dbg_printf("cell %u ina %u raw 0x%08lX\r\n", (unsigned)cell, (unsigned)cfg->ina_index, (unsigned long)raw);
//
//        // Check for obviously-bad raw values (bus failure or not connected)
//        if (raw == 0xFFFFFFFFu || raw == 0x00000000u) {
//            INA229_Readings[cfg->ina_index].reading_valid = 0;
//            INA229_Readings[cfg->ina_index].voltage_V = 0.0f;
//            INA229_Readings[cfg->ina_index].last_update_time = now;
//            continue;
//        }
//
//        // convert raw to real voltage using existing converter
//        float v = INA229_getVBUS_V(ina);
//
//        // Validate converted voltage; adjust bounds to your system
//        if (!isnan(v) && v >= 0.3f && v <= 6.0f) {
//            INA229_Readings[cfg->ina_index].voltage_V = v;
//            INA229_Readings[cfg->ina_index].reading_valid = 1;
//        } else {
//            // If converted value unreasonable, mark invalid to avoid showing garbage
//            dbg_printf("INA%u v=%.3f invalid -> mark invalid\r\n", (unsigned)cfg->ina_index, v);
//            INA229_Readings[cfg->ina_index].voltage_V = v; // keep it for diagnostics
//            INA229_Readings[cfg->ina_index].reading_valid = 0;
//        }
//
//        INA229_Readings[cfg->ina_index].cell_id = cfg->cell_id;
//        INA229_Readings[cfg->ina_index].last_update_time = now;
//
//        // Optional: small delay between CS toggles if your hardware needs it
//        // HAL_Delay(1);
//    }
//
//    // Recalculate derived metrics (pack voltage)
//    Update_PackMetrics(); // keep this helper, sums valid readings
//    bms_data.last_update_time = now;
//}
///* Compute pack-level metrics from INA229_Readings and cell_configs */
void Update_PackMetrics(void)
{
    float pack_v = 0.0f;
    float pack_i = 0.0f; // if you aggregate current (optional)
    uint8_t valid_cells = 0;

    for (uint8_t cell = 0; cell < MAX_CELL_COUNT; cell++) {
        const Cell_Config* config = &cell_configs[cell];
        if (config == NULL) continue;
        if (config->ina_index >= NUM_INA229) continue;

        INA229_Reading* r = &INA229_Readings[config->ina_index];
        if (r->reading_valid) {
            // Sum cell voltages to get pack voltage (series stack)
            pack_v += r->voltage_V;
            valid_cells++;
            // pack_i aggregation: you might want an average or the pack current from a master INA
            // pack_i += r->current_mA; // if per-ina current present (units must be consistent)
        }
    }

    // If you want pack_current as average or from a dedicated current sensor, implement here
    bms_data.pack_voltage = pack_v;
    bms_data.pack_current = pack_i; // leave 0 if not computed
}

bms_data_t* Display_GetBMSData(void)
{
    return &bms_data;
}

display_state_t* Display_GetState(void)
{
    return &display_state;
}

void Display_DrawProgressBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, float percentage)
{
    // Clamp percentage to 0-100%
    if (percentage < 0.0f) percentage = 0.0f;
    if (percentage > 100.0f) percentage = 100.0f;

    // Draw outer frame
    u8g2_DrawFrame(&u8g2, x, y, width, height);

    // Calculate fill width
    uint8_t fill_width = (uint8_t)(percentage * (width - 2) / 100.0f);

    // Draw filled portion
    if (fill_width > 0) {
        u8g2_DrawBox(&u8g2, x + 1, y + 1, fill_width, height - 2);
    }
}

void Display_UpdateBlink(void)
{
    display_state_t *state = Display_GetState();
    uint32_t now = HAL_GetTick();

    // On first use, initialize
    if (state->last_blink_time == 0) {
        state->last_blink_time = now;
        state->blink_on = 1;
        return;
    }

    // Toggle every BLINK_INTERVAL_MS
    if ((now - state->last_blink_time) >= BLINK_INTERVAL_MS) {
        state->last_blink_time = now;
        state->blink_on = !state->blink_on;
    }
}

void Display_ResetBlink(void)
{
    display_state_t *state = Display_GetState();
    state->last_blink_time = HAL_GetTick();
    state->blink_on = 1;
}


