/*
 * getDisplay.h
 *
 *  Created on: Aug 14, 2025
 *      Author: Saminda Attanayake
 */

#ifndef INC_GETDISPLAY_H_
#define INC_GETDISPLAY_H_

#include "main.h"
#include "display.h"
#include <stdint.h>
#include "u8g2.h"


/*==============================================================================
                          BUTTON HANDLING MACROS
==============================================================================*/
#define BUTTON_DEBOUNCE_MS         (50)
#define BUTTON_REPEAT_INITIAL_MS   (500)
#define BUTTON_REPEAT_RATE_MS      (100)
#define BUTTON_LONG_PRESS_MS       (1000)

/*==============================================================================
                          BUTTON STATE ENUMERATIONS
==============================================================================*/
typedef enum {
    BUTTON_STATE_IDLE = 0,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_HELD,
    BUTTON_STATE_RELEASED
} button_state_t;

typedef struct {
    button_state_t state;
    uint32_t press_time;
    uint32_t last_action_time;
    uint8_t is_repeating;
    uint8_t long_press_triggered;
} button_info_t;

/*==============================================================================
                          NAVIGATION CONSTANTS
==============================================================================*/
#define MENU_ITEMS_MAIN            (6)
#define MENU_ITEMS_LIVE_DATA       (4)
#define MENU_ITEMS_CONTROL         (4)
#define MENU_ITEMS_TEST            (4)
#define MENU_ITEMS_SETTINGS        (3)
#define MENU_ITEMS_TEMPERATURE 	   (3)
#define MENU_ITEMS_CELL_MONITOR	   (3)


/*==============================================================================
                          FUNCTION PROTOTYPES
==============================================================================*/

/* Main button handling functions */
void Display_HandleButtons(void);
void Display_HandleButtonPress(uint16_t GPIO_Pin);
void Display_ProcessButtonState(button_t button, button_info_t* button_info);

/* Navigation functions */
void Display_NavigateUp(void);
void Display_NavigateDown(void);
void Display_NavigateEnter(void);
void Display_NavigateBack(void);
void Display_WakeUp(void);


/* Screen-specific navigation handlers */
void Display_HandleMainMenuNavigation(button_t button);
void Display_HandleLiveDataNavigation(button_t button);
void Display_HandleControlMenuNavigation(button_t button);
void Display_HandleTestMenuNavigation(button_t button);
void Display_HandleSettingsNavigation(button_t button);

void Display_HandleCellNavigation(button_t button);
void Display_HandleTemperatureNavigation(button_t button);


/* Value adjustment functions */
void Display_AdjustVoltage(int8_t direction);
void Display_ToggleCell(int8_t direction);
void Display_AdjustTemperature(int8_t direction);
void Display_AdjustCell(int8_t direction);
void Display_AdjustTempCard(int8_t direction);

void Display_TempSetAdjust(temperature_mode_t temperaturemode);


/* Action execution functions */
void Display_ExecuteAction(void);
void Display_ExecuteCellBalance(void);
void Display_ExecuteTemperatureSet(void);
void Display_ExecuteTestSequence(void);

void Display_ExecuteVoltageSet(void) ;


/* Utility functions */
uint8_t Display_GetMenuItemCount(menu_screen_t screen);
void Display_ResetScreenSaver(void);
void Display_ShowConfirmation(const char* message);
void Display_ShowError(const char* error_message);

/* Button state access */
button_info_t* Display_GetButtonInfo(button_t button);
void Display_ResetButton(button_t button);

/* External button variables */
extern uint8_t buttonUp;
extern uint8_t buttonDown;
extern uint8_t buttonEnter;
extern uint8_t buttonBack;

/* Timing variables */
extern uint32_t screensaverTimer;

#endif /* INC_GETDISPLAY_H_ */
