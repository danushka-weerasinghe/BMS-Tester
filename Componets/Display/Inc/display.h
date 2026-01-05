/*
 * display.h
 *
 *  Created on: Feb 26, 2025
 *      Author: Danushka Weerasinghe
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "main.h"
#include "u8g2.h"
#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "INA229.h"   // or the header that defines INA229_Handle

extern u8g2_t u8g2;
extern uint8_t DC_chain ;
extern uint8_t csu_type ;



/*==============================================================================
                          DISPLAY MACROS AND CONSTANTS
==============================================================================*/
#define DISPLAY_UPDATE_DELAY_U32   (2U)
#define SPI_TIMEOUT_U32            (10000U)
#define CONTRAST_VALUE_U8          (120U)
#define PAGE_DELAY_U32             (1000U)
#define TEXT_DELAY_U32             (10U)
#define FINAL_PAGE_DELAY_U32       (2000U)
#define SCREENSAVER_TIMEOUT_MS     (60000U) // +60 seconds to screensaver
#define BACKLIGHT_OFF_TIMEOUT_MS   (30000)  // +30 seconds to turn off backlight
#define SCREEN_OFF_TIMEOUT_MS      (30000)  // +30 seconds to turn off screen
#define BLINK_INTERVAL_MS 			500U
// Menu definitions
#define MAX_MENU_ITEMS             (8)
#define MAX_STRING_LENGTH          (20)


#ifndef MAX_CELL_COUNT
#define MAX_CELL_COUNT 24
#endif

#ifndef MAX_TEMP_CARDS
#define MAX_TEMP_CARDS 6
#endif

#ifndef NUM_CSU_INA
#define NUM_CSU_INA 2
#endif


// Display dimensions (ST7565 128x64)
#define DISPLAY_WIDTH              (128)
#define DISPLAY_HEIGHT             (64)

/*==============================================================================
                          MENU SYSTEM ENUMERATIONS
==============================================================================*/
typedef enum {
    SCREEN_SAVER = 0,
    MAIN_MENU,
    LIVE_DATA_MENU,
    CELL_MONITOR_SCREEN,
	CELL_MONITOR_CSU,
	CARD_CELL_SCREEN,
	CELL_MONITOR_DCCSU,
	CELL_SET_DAISY_CHAIN,
    PACK_OVERVIEW_SCREEN,
    TEMPERATURE_SCREEN,
	CELL_TEMPERATURE_SCREEN,      // CSU
	CARD_TEMPERATURE_SCREEN,      // temp_card
	SYSTEM_TEMPERATURE_SCREEN,    // daisy chain
    CURRENT_VOLTAGE_SCREEN,
    CONTROL_MENU,
    CELL_BALANCE_MENU,
    VOLTAGE_SET_MENU,
    TEMP_SET_MENU,
    RELAY_CONTROL_MENU,
    TEST_MENU,
    AUTO_SEQUENCE_SCREEN,
    MANUAL_TEST_SCREEN,
    CALIBRATION_MENU,
    OPEN_WIRE_TEST_SCREEN,
    SETTINGS_MENU,
    SYSTEM_INFO_SCREEN,
    DISPLAY_SETTINGS_SCREEN,
    COMMUNICATION_SCREEN,
    DIAGNOSTIC_MENU,
    ERROR_LOG_SCREEN,
    STATUS_SCREEN,
    ABOUT_SCREEN
} menu_screen_t;

typedef enum {
    BTN_ENTER = 0,
    BTN_UP,
    BTN_DOWN,
    BTN_BACK
} button_t;

// Add to display.h
typedef enum {
    POWER_NORMAL = 0,      // Normal operation
    POWER_SCREENSAVER,     // Screensaver active, backlight on
    POWER_BACKLIGHT_OFF,   // Screensaver active, backlight off
    POWER_SCREEN_OFF       // Screen completely off
} power_state_t;

typedef enum {
    VOLTAGE_MODE_SELECT_CELL,    // Scrolling between cells
    VOLTAGE_MODE_ADJUST_VOLTAGE  // Adjusting voltage for selected cell
} voltage_mode_t;

typedef enum{
	TEMPERATURE_MODE_SELECT_CARD,
	TEMPERATURE_MODE_ADJUST_TEMPERATURE
}temperature_mode_t;

/*==============================================================================
                          DATA STRUCTURES
==============================================================================*/
typedef struct {

    float temp_card_C[MAX_TEMP_CARDS];

    float pack_voltage;
    float pack_current;
    uint8_t cell_balance_states[MAX_CELL_COUNT];
    uint8_t cell_led_states[MAX_CELL_COUNT];
    uint8_t error_flags;
    uint32_t last_update_time;
} bms_data_t;

typedef struct {
    menu_screen_t current_screen;
    menu_screen_t previous_screen;
    uint8_t selected_item;
    uint8_t selected_cell;
    voltage_mode_t voltage_mode;
    temperature_mode_t temperature_mode;
    uint8_t selected_temp_card;
    uint8_t scroll_offset;
    float target_voltage;
    float target_temperature;
    float target_resistance;
    uint8_t test_sequence_running;
    uint8_t current_test_step;
    uint32_t screensaver_timer;
    uint8_t display_contrast;
    uint8_t auto_refresh;
    uint8_t show_warnings;
    power_state_t power_state;
    uint32_t power_timer;
    uint8_t  blink_on;
    uint32_t last_blink_time;

} display_state_t;


//static INA229_Handle get_ina_handle(uint8_t index);

/*==============================================================================
                          FUNCTION PROTOTYPES
==============================================================================*/

/* Initialization functions */
void Display_Init(void);
void Display_MainTitlePage(void);

/* Core menu functions */
void Display_Update(void);
void Display_HandleButton(button_t button);
void Display_SetScreen(menu_screen_t screen);

/* Main menu screens */
void Display_MainMenu(void);
void Display_Screensaver(void);

/* Live data screens */
void Display_LiveDataMenu(void);
void Display_CellMonitor(void);

void Display_PackOverview(void);
void Display_TemperatureScreen(void);
void Display_CurrentVoltageScreen(void);
static void Update_PackMetrics(void);

/* Cell screens*/

//void Display_CellMonitorCSU(void);
void Display_CellMonitorCard(void);
void Display_CellMonitorDaisyCSU(void);
void Display_SetDaisyChain(void);


/* Temperature screens*/
void Display_CellTemperatureScreen(void);
void Display_CardTemperatureScreen(void);
void Display_SystemTemperatureScreen(void);


/* Control menu screens */
void Display_ControlMenu(void);
void Display_CellBalanceMenu(void);
void Display_VoltageSetMenu(void);
void Display_TempSetMenu(void);
void Display_RelayControlMenu(void);

void Display_VoltageSetAdjust(voltage_mode_t voltage_mode);

/* Test menu screens */
void Display_TestMenu(void);
void Display_AutoSequence(void);
void Display_ManualTest(void);
void Display_CalibrationMenu(void);
void Display_OpenWireTest(void);

/* Settings and diagnostic screens */
void Display_SettingsMenu(void);
void Display_SystemInfo(void);
void Display_DisplaySettings(void);
void Display_CommunicationScreen(void);
void Display_DiagnosticMenu(void);
void Display_ErrorLog(void);
void Display_StatusScreen(void);
void Display_AboutScreen(void);

/* Utility functions */
void Display_DrawHeader(const char* title);
void Display_DrawMenuItem(uint8_t index, const char* text, uint8_t selected);
void Display_DrawProgressBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, float percentage);
void Display_DrawValue(uint8_t x, uint8_t y, const char* label, float value, const char* unit);
void Display_DrawGraph(uint8_t x, uint8_t y, uint8_t width, uint8_t height, float* data, uint8_t count);
void Display_ShowPopup(const char* title, const char* message, uint16_t timeout);
void Display_ClearScreen(void);
void Display_FormatVoltage(char* buffer, float voltage);
void Display_FormatCurrent(char* buffer, float current);
void Display_FormatTemperature(char* buffer, float temperature);

/* Data access functions */
void Display_UpdateBMSData(void);
bms_data_t* Display_GetBMSData(void);
display_state_t* Display_GetState(void);

/* Navigation helpers */
void Display_NavigateUp(void);
void Display_NavigateDown(void);
void Display_NavigateEnter(void);
void Display_NavigateBack(void);

/* Animation and effects */
void Display_AnimateTransition(menu_screen_t from, menu_screen_t to);
void Display_ShowLoadingSpinner(const char* message);

void Display_UpdateBlink(void);
void Display_ResetBlink(void);

/* External interface */
extern uint8_t buttonUp;
extern uint8_t buttonDown;
extern uint8_t buttonEnter;
extern uint8_t buttonBack;
#endif /* INC_DISPLAY_H_ */
