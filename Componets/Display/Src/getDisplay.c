/*
 * getDisplay.c
 *
 *  Created on: Aug 14, 2025
 *      Author: Saminda Attanayake
 */

#include "getDisplay.h"
#include "display.h"
#include "BMS_test_protocol.h"
#include "Temp_controller.h"
#include "u8g2.h"
#include <math.h>        //  needed for fabs/fabsf

/*==============================================================================
                          LOCAL VARIABLES
==============================================================================*/
static button_info_t button_states[4] = {0}; // BTN_ENTER, BTN_UP, BTN_DOWN, BTN_BACK

// External button flags
extern uint8_t buttonUp;
extern uint8_t buttonDown;
extern uint8_t buttonEnter;
extern uint8_t buttonBack;
extern uint32_t screensaverTimer;

/*==============================================================================
                          MAIN BUTTON HANDLING
==============================================================================*/
//void Display_HandleButtons(void)
//{
//    uint32_t current_time = HAL_GetTick();
//
//    // Process each button state
//    if (buttonEnter) {
//        Display_ProcessButtonState(BTN_ENTER, &button_states[BTN_ENTER]);
//        buttonEnter = 0;
//    }
//
//    if (buttonUp) {
//        Display_ProcessButtonState(BTN_UP, &button_states[BTN_UP]);
//        buttonUp = 0;
//    }
//
//    if (buttonDown) {
//        Display_ProcessButtonState(BTN_DOWN, &button_states[BTN_DOWN]);
//        buttonDown = 0;
//    }
//
//    if (buttonBack) {
//        Display_ProcessButtonState(BTN_BACK, &button_states[BTN_BACK]);
//        buttonBack = 0;
//    }
//
//    // Handle button repeat for held buttons
//    for (uint8_t i = 0; i < 4; i++) {
//        if (button_states[i].state == BUTTON_STATE_HELD && button_states[i].is_repeating) {
//            if ((current_time - button_states[i].last_action_time) > BUTTON_REPEAT_RATE_MS) {
//                switch (i) {
//                    case BTN_UP:
//                        Display_NavigateUp();
//                        break;
//                    case BTN_DOWN:
//                        Display_NavigateDown();
//                        break;
//                    default:
//                        break;
//                }
//                button_states[i].last_action_time = current_time;
//            }
//        }
//    }
//}
//
//void Display_ProcessButtonState(button_t button, button_info_t* button_info)
//{
//    uint32_t current_time = HAL_GetTick();
//
//    switch (button_info->state) {
//        case BUTTON_STATE_IDLE:
//            button_info->state = BUTTON_STATE_PRESSED;
//            button_info->press_time = current_time;
//            button_info->last_action_time = current_time;
//            button_info->is_repeating = 0;
//            button_info->long_press_triggered = 0;
//
//            // Reset screensaver
//            Display_ResetScreenSaver();
//
//            // Handle immediate button press
//            switch (button) {
//                case BTN_ENTER:
//                    Display_NavigateEnter();
//                    break;
//                case BTN_UP:
//                    Display_NavigateUp();
//                    break;
//                case BTN_DOWN:
//                    Display_NavigateDown();
//                    break;
//                case BTN_BACK:
//                    Display_NavigateBack();
//                    break;
//            }
//            break;
//
//        case BUTTON_STATE_PRESSED:
//            // Check for long press
//            if ((current_time - button_info->press_time) > BUTTON_LONG_PRESS_MS &&
//                !button_info->long_press_triggered) {
//
//                button_info->long_press_triggered = 1;
//                // Handle long press actions here if needed
//            }
//
//            // Check for repeat
//            if ((current_time - button_info->press_time) > BUTTON_REPEAT_INITIAL_MS) {
//                button_info->state = BUTTON_STATE_HELD;
//                button_info->is_repeating = 1;
//            }
//            break;
//
//        case BUTTON_STATE_HELD:
//            // Button is being held - repeat handled in main loop
//            break;
//
//        case BUTTON_STATE_RELEASED:
//            button_info->state = BUTTON_STATE_IDLE;
//            button_info->is_repeating = 0;
//            break;
//    }
//}

//void Display_HandleButtonPress(uint16_t GPIO_Pin)
//{
//    switch(GPIO_Pin) {
//        case PUSH_BUTTON_01_Pin: // Enter
//            buttonEnter = 1;
//            break;
//        case PUSH_BUTTON_02_Pin: // Up
//            buttonUp = 1;
//            break;
//        case PUSH_BUTTON_03_Pin: // Down
//            buttonDown = 1;
//            break;
//        case PUSH_BUTTON_04_Pin: // Back
//            buttonBack = 1;
//            break;
//        default:
//            break;
//    }
//}

//void Display_ProcessButtonState(button_t button, button_info_t* button_info)
//{
//    uint32_t current_time = HAL_GetTick();
//
//    // Check if we need to wake up from power saving
//
//
//    switch (button_info->state) {
//        case BUTTON_STATE_IDLE:
//            button_info->state = BUTTON_STATE_PRESSED;
//            button_info->press_time = current_time;
//            button_info->last_action_time = current_time;
//            button_info->is_repeating = 0;
//            button_info->long_press_triggered = 0;
//
//            Display_HandleButtons();
//
//    }
//}

void Display_HandleButtons(void)
{
	if (Display_GetState()->power_state != POWER_NORMAL) {

		if (buttonEnter || buttonUp||buttonDown ||buttonBack  ){
	        Display_WakeUp();
		    buttonEnter = buttonUp  = buttonDown = buttonBack = 0 ;
	        return;
	    }
	}

    // Simple single-action button handling
    if (buttonEnter) {
        Display_NavigateEnter(); buttonEnter = 0; Display_ResetScreenSaver();
    }

    if (buttonUp) {
        Display_NavigateUp();	 buttonUp = 0;	  Display_ResetScreenSaver();
    }

    if (buttonDown) {
        Display_NavigateDown();  buttonDown = 0;  Display_ResetScreenSaver();
    }

    if (buttonBack) {
        Display_NavigateBack();	 buttonBack = 0;  Display_ResetScreenSaver();
    }
}

/*==============================================================================
                          NAVIGATION FUNCTIONS
==============================================================================*/
void Display_NavigateUp(void)
{
    display_state_t* state = Display_GetState();

    switch (state->current_screen) {
        case MAIN_MENU:
        case LIVE_DATA_MENU:
        case CONTROL_MENU:
        case TEST_MENU:
        case SETTINGS_MENU:
        case TEMPERATURE_SCREEN:
        case CELL_MONITOR_SCREEN:
            if (state->selected_item > 0) {
                state->selected_item--;
            } else {
                state->selected_item = Display_GetMenuItemCount(state->current_screen) - 1;
            }
            break;

        case CELL_MONITOR_CSU:
        case CARD_CELL_SCREEN:
        case CELL_MONITOR_DCCSU:
            Display_AdjustCell(-1);
            break;

        case CELL_TEMPERATURE_SCREEN:
        case CARD_TEMPERATURE_SCREEN:
        case SYSTEM_TEMPERATURE_SCREEN:
            Display_AdjustTempCard(-1);
            break;

//        case TEMPERATURE_SCREEN:
//            Display_AdjustTempCard(-1);
//            break;

        case VOLTAGE_SET_MENU:
            if (state->voltage_mode == VOLTAGE_MODE_SELECT_CELL) {
           	 Display_AdjustCell(1);
            } else if (state->voltage_mode == VOLTAGE_MODE_ADJUST_VOLTAGE) {
           	 Display_AdjustVoltage(1);  // Increase voltage
            }
            break;

        case TEMP_SET_MENU:
                    if (state->temperature_mode == TEMPERATURE_MODE_SELECT_CARD) {
                        Display_AdjustTempCard(1);
                    } else if (state->temperature_mode == TEMPERATURE_MODE_ADJUST_TEMPERATURE) {
                        Display_AdjustTemperature(1); // Increase temperature
                    }
                    break;




        default:
            break;
    }
}

void Display_NavigateDown(void)
{
    display_state_t* state = Display_GetState();

    switch (state->current_screen) {
        case MAIN_MENU:
        case LIVE_DATA_MENU:
        case CONTROL_MENU:
        case TEST_MENU:
        case SETTINGS_MENU:
        case TEMPERATURE_SCREEN:
        case CELL_MONITOR_SCREEN:
            state->selected_item++;
            if (state->selected_item >= Display_GetMenuItemCount(state->current_screen)) {
                state->selected_item = 0;
            }
            break;

        case CELL_MONITOR_CSU:
        case CARD_CELL_SCREEN:
        case CELL_MONITOR_DCCSU:
            Display_AdjustCell(1);
            break;

        case CELL_TEMPERATURE_SCREEN:
        case CARD_TEMPERATURE_SCREEN:
        case SYSTEM_TEMPERATURE_SCREEN:
        	Display_AdjustTempCard(1);
            break;

//        case TEMPERATURE_SCREEN:
//            Display_AdjustTempCard(1);
//            break;

        case VOLTAGE_SET_MENU:
			if (state->voltage_mode == VOLTAGE_MODE_SELECT_CELL) {
				Display_AdjustCell(-1);
			} else if (state->voltage_mode == VOLTAGE_MODE_ADJUST_VOLTAGE) {
				// Adjust voltage down
				Display_AdjustVoltage(-1);
			}
			break;



        case TEMP_SET_MENU:
                    if (state->temperature_mode == TEMPERATURE_MODE_SELECT_CARD) {
                        Display_AdjustTempCard(-1);
                    } else if (state->temperature_mode == TEMPERATURE_MODE_ADJUST_TEMPERATURE) {
                        Display_AdjustTemperature(-1); // Decrease temperature
                    }
                    break;




        default:
            break;
    }
}

void Display_NavigateEnter(void)
{
    display_state_t* state = Display_GetState();

    switch (state->current_screen) {
        case SCREEN_SAVER:
            Display_SetScreen(MAIN_MENU);
            break;

        case MAIN_MENU:
            Display_HandleMainMenuNavigation(BTN_ENTER);
            break;

        case LIVE_DATA_MENU:
            Display_HandleLiveDataNavigation(BTN_ENTER);
            break;

        case TEMPERATURE_SCREEN:
            Display_HandleTemperatureNavigation(BTN_ENTER);
            break;

        case CELL_MONITOR_SCREEN:
        	Display_HandleCellNavigation(BTN_ENTER);
        	break;

//        case CELL_SET_DAISY_CHAIN:
//			Display_HandleDaisyChainNavigation(BTN_ENTER);
//			break;


        case CONTROL_MENU:
            Display_HandleControlMenuNavigation(BTN_ENTER);
            break;

        case TEST_MENU:
            Display_HandleTestMenuNavigation(BTN_ENTER);
            break;

        case SETTINGS_MENU:
            Display_HandleSettingsNavigation(BTN_ENTER);
            break;

        case CELL_BALANCE_MENU:
            Display_ExecuteCellBalance();
            break;

//        case VOLTAGE_SET_MENU:
//        	Display_AdjustCell();
//            break;

        case VOLTAGE_SET_MENU:
			if (state->voltage_mode == VOLTAGE_MODE_SELECT_CELL) {
				// Enter voltage adjustment mode
				state->voltage_mode = VOLTAGE_MODE_ADJUST_VOLTAGE;
			} else if (state->voltage_mode == VOLTAGE_MODE_ADJUST_VOLTAGE) {
				// Apply voltage setting and stay in adjustment mode
				Display_ExecuteVoltageSet();
			}
			break;



        case TEMP_SET_MENU:
                    if (state->temperature_mode == TEMPERATURE_MODE_SELECT_CARD) {
                        // Enter temperature adjustment mode
                        state->temperature_mode = TEMPERATURE_MODE_ADJUST_TEMPERATURE;
                        Display_ResetBlink(); // start caret visible
                        // stay on the same screen so DisplayUpdate redraws it
                    } else if (state->temperature_mode == TEMPERATURE_MODE_ADJUST_TEMPERATURE) {
                        // Apply temperature setting and stay in adjustment mode
                        Display_ExecuteTemperatureSet();
                    }
                    break;


        case AUTO_SEQUENCE_SCREEN:
            Display_ExecuteTestSequence();
            break;

        default:
            Display_SetScreen(MAIN_MENU);

            break;
    }
}

void Display_NavigateBack(void)
{
    display_state_t* state = Display_GetState();

    switch (state->current_screen) {
        case MAIN_MENU:
            Display_SetScreen(SCREEN_SAVER);
            break;

        case LIVE_DATA_MENU:
        case CONTROL_MENU:
        case TEST_MENU:
        case SETTINGS_MENU:
            Display_SetScreen(MAIN_MENU);
            break;

        case CELL_TEMPERATURE_SCREEN:
        case CARD_TEMPERATURE_SCREEN:
        case SYSTEM_TEMPERATURE_SCREEN:
        	Display_SetScreen(TEMPERATURE_SCREEN);
        	break;

        case CELL_MONITOR_CSU:
        case CARD_CELL_SCREEN:
        case CELL_MONITOR_DCCSU:
        	Display_SetScreen(CELL_MONITOR_SCREEN);
        	break;


        case PACK_OVERVIEW_SCREEN:
        case TEMPERATURE_SCREEN:
        case CURRENT_VOLTAGE_SCREEN:
        case CELL_MONITOR_SCREEN:
            Display_SetScreen(LIVE_DATA_MENU);
            break;



        case CELL_BALANCE_MENU:
        case RELAY_CONTROL_MENU:
            Display_SetScreen(CONTROL_MENU);
            break;


        case VOLTAGE_SET_MENU:
			if (state->voltage_mode == VOLTAGE_MODE_ADJUST_VOLTAGE) {

				state->voltage_mode = VOLTAGE_MODE_SELECT_CELL;
			} else {
				// Go back to control menu
				Display_SetScreen(CONTROL_MENU);
			}
			break;

        case TEMP_SET_MENU:
		   if (state->temperature_mode == TEMPERATURE_MODE_ADJUST_TEMPERATURE) {
			   state->temperature_mode = TEMPERATURE_MODE_SELECT_CARD;
		   } else {
			   // Go back to control menu
			   Display_SetScreen(CONTROL_MENU);
		   }
		   break;



        case AUTO_SEQUENCE_SCREEN:
        case MANUAL_TEST_SCREEN:
        case CALIBRATION_MENU:
        case OPEN_WIRE_TEST_SCREEN:
            Display_SetScreen(TEST_MENU);
            break;

        case SYSTEM_INFO_SCREEN:
        case DISPLAY_SETTINGS_SCREEN:
        case COMMUNICATION_SCREEN:
            Display_SetScreen(SETTINGS_MENU);
            break;

        default:
            Display_SetScreen(MAIN_MENU);
            break;
    }
}

/*==============================================================================
                          SCREEN-SPECIFIC NAVIGATION
==============================================================================*/
void Display_HandleMainMenuNavigation(button_t button)
{
    display_state_t* state = Display_GetState();

    if (button == BTN_ENTER) {
        switch (state->selected_item) {
            case 0: // Live Data
                Display_SetScreen(LIVE_DATA_MENU);
                break;
            case 1: // Control
                Display_SetScreen(CONTROL_MENU);
                break;
            case 2: // Test & Cal
                Display_SetScreen(TEST_MENU);
                break;
            case 3: // Settings
                Display_SetScreen(SETTINGS_MENU);
                break;
            case 4: // Diagnostics
                Display_SetScreen(DIAGNOSTIC_MENU);
                break;
            case 5: // About
                Display_SetScreen(ABOUT_SCREEN);
                break;
        }
    }
}

void Display_HandleLiveDataNavigation(button_t button)
{
    display_state_t* state = Display_GetState();

    if (button == BTN_ENTER) {
        switch (state->selected_item) {
            case 0: // Cell Monitor
                Display_SetScreen(CELL_MONITOR_SCREEN);
                break;
            case 1: // Pack Overview
                Display_SetScreen(PACK_OVERVIEW_SCREEN);
                break;
            case 2: // Temperature
                Display_SetScreen(TEMPERATURE_SCREEN);
                break;
            case 3: // Current/Voltage
                Display_SetScreen(CURRENT_VOLTAGE_SCREEN);
                break;
            case 4: // Back
                Display_SetScreen(MAIN_MENU);
                break;
        }
    }
}

void Display_HandleTemperatureNavigation(button_t button)
{
    display_state_t* state = Display_GetState();
    if (button == BTN_ENTER) {
        switch (state->selected_item) {
            case 0: // Cell Temperatures
                Display_SetScreen(CELL_TEMPERATURE_SCREEN);
                break;
            case 1: // Card Temperatures
                Display_SetScreen(CARD_TEMPERATURE_SCREEN);
                break;
            case 2: // System Temperatures
                Display_SetScreen(SYSTEM_TEMPERATURE_SCREEN);
                break;
            case 3: // Back
                Display_SetScreen(LIVE_DATA_MENU);
                break;
        }
    }
}



void Display_HandleCellNavigation(button_t button)
{
    display_state_t* state = Display_GetState();
    if (button == BTN_ENTER) {
        switch (state->selected_item) {
            case 0: // Cell csu
                Display_SetScreen(CELL_MONITOR_CSU);
                break;
            case 1: // Cell Cards
                Display_SetScreen(CARD_CELL_SCREEN);
                break;
            case 2: // DCCSU
                Display_SetScreen(CELL_MONITOR_DCCSU);
                break;
            case 3: // Back
                Display_SetScreen(LIVE_DATA_MENU);
                break;
        }
    }
}
//
//void Display_HandleDaisyChainNavigation(button_t button)
//{
//    display_state_t* state = Display_GetState();
//
//    if (button == BTN_ENTER) {
//        switch (state->selected_cell) {
//            case 0: // ON
//                Set_Daisy_Chain(1);
//                Display_ShowConfirmation("Daisy Chain ON");
//                break;
//            case 1: // OFF
//                Set_Daisy_Chain(0);
//                Display_ShowConfirmation("Daisy Chain OFF");
//                break;
//        }
//        Display_SetScreen(CELL_SET_DAISY_CHAIN);
//    }
//}



void Display_HandleControlMenuNavigation(button_t button)
{
    display_state_t* state = Display_GetState();

    if (button == BTN_ENTER) {
        switch (state->selected_item) {
            case 0: // Cell Balance
                Display_SetScreen(CELL_BALANCE_MENU);
                break;
            case 1: // Set Voltage
                Display_SetScreen(VOLTAGE_SET_MENU);
                break;
            case 2: // Set Temperature
                Display_SetScreen(TEMP_SET_MENU);
                break;
            case 3: // Relay Control
                Display_SetScreen(RELAY_CONTROL_MENU);
                break;
            case 4: // Back
                Display_SetScreen(MAIN_MENU);
                break;
        }
    }
}



void Display_HandleSetVoltageNavigation(button_t button){

    display_state_t* state = Display_GetState();

    if (button == BTN_ENTER) {
            switch (state->selected_item) {
            case 0:
                Display_SetScreen(VOLTAGE_SET_MENU);
                break;

            case 1:
            	display_state_t* state = Display_GetState();
                state->voltage_mode = VOLTAGE_MODE_ADJUST_VOLTAGE;
                Display_ResetBlink();                   // start caret visible
                Display_SetScreen(VOLTAGE_SET_MENU);    // stay on the same screen so Display_Update() redraws it
                break;


            case 2:

                    Display_ExecuteVoltageSet();
                    	break;
            }
        }

}

void Display_HandleSetTempNavigation(button_t button){
    display_state_t* state = Display_GetState();

    if (button == BTN_ENTER) {
        switch (state->selected_item) {
            case 0: // Navigate to temp set screen
                Display_SetScreen(TEMP_SET_MENU);
                break;

            case 1: // Enter temperature adjustment mode
                state->temperature_mode = TEMPERATURE_MODE_ADJUST_TEMPERATURE; // Fixed the mode name
                Display_ResetBlink();
                Display_SetScreen(TEMP_SET_MENU);
                break;

            case 2: // EXECUTE the temperature setting
                Display_ExecuteTemperatureSet(); // <-- THIS calls Set_Resistance
                break;
        }
    }
}


void Display_HandleTestMenuNavigation(button_t button)
{
    display_state_t* state = Display_GetState();

    if (button == BTN_ENTER) {
        switch (state->selected_item) {
            case 0: // Auto Sequence
                Display_SetScreen(AUTO_SEQUENCE_SCREEN);
                break;
            case 1: // Manual Test
                Display_SetScreen(MANUAL_TEST_SCREEN);
                break;
            case 2: // Calibration
                Display_SetScreen(CALIBRATION_MENU);
                break;
            case 3: // Open Wire Test
                Display_SetScreen(OPEN_WIRE_TEST_SCREEN);
                break;
            case 4: // Back
                Display_SetScreen(MAIN_MENU);
                break;
        }
    }
}

void Display_HandleSettingsNavigation(button_t button)
{
    display_state_t* state = Display_GetState();

    if (button == BTN_ENTER) {
        switch (state->selected_item) {
            case 0: // System Info
                Display_SetScreen(SYSTEM_INFO_SCREEN);
                break;
            case 1: // Display Settings
                Display_SetScreen(DISPLAY_SETTINGS_SCREEN);
                break;
            case 2: // Communication
                Display_SetScreen(COMMUNICATION_SCREEN);
                break;
            case 3: // Back
                Display_SetScreen(MAIN_MENU);
                break;
        }
    }
}


/*==============================================================================
                          VALUE ADJUSTMENT FUNCTIONS
==============================================================================*/

//
//void Display_AdjustTemperature(int8_t direction)
//{
//    display_state_t* state = Display_GetState();
//
//    if (direction > 0) {
//        state->target_resistance += 1;
//        if (state->target_resistance > 100) {
//            state->target_resistance = 100;
//        }
//    } else {
//        if (state->target_resistance > 1) {
//            state->target_resistance -= 1;
//        }
//    }
//}

void Display_AdjustTemperature(int8_t direction) {
    display_state_t *state = Display_GetState();

    // Define temperature levels (similar to voltage levels)
    static const float temperaturelevels[] = {-20.0f, -10.0f, 0.0f, 10.0f, 20.0f, 25.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f};
    static const uint8_t numlevels = sizeof(temperaturelevels) / sizeof(temperaturelevels[0]);

    // Find current temperature index
    uint8_t currentindex = 0;
    for (uint8_t i = 0; i < numlevels; i++) {
        if (fabs(state->target_temperature - temperaturelevels[i]) < 0.5f) {
            currentindex = i;
            break;
        }
    }

    // Adjust temperature level
    if (direction > 0) {
        currentindex++;
        if (currentindex >= numlevels) {
            currentindex = numlevels - 1;
        }
    } else if (currentindex > 0) {
        currentindex--;
    }

    state->target_temperature = temperaturelevels[currentindex];
}
void Display_AdjustCell(int8_t direction)
{
    display_state_t* state = Display_GetState();

    // Determine the maximum cell count based on current screen context
    uint8_t max_cells = MAX_CELL_COUNT; // Default to 24 cells

    // Adjust max cells based on current screen context
    switch (state->current_screen) {
        case CELL_MONITOR_CSU:
            // For CSU: 0-11 = 12-CSU (12 cells), 12-23 = 11-CSU (11 cells)
            max_cells = 23; // Total range for CSU navigation (0-23)
            break;

        case CARD_CELL_SCREEN:
            max_cells = MAX_CELL_COUNT; // Full 24 cells for card monitoring
            break;

        case CELL_MONITOR_DCCSU:
            max_cells = 23; // DC-CSU has 24 cells
            break;
//        case CELL_SET_DAISY_CHAIN:
//        	max_cells = 2;
//        	break;
        default:
            max_cells = MAX_CELL_COUNT;
            break;
    }

    // Ensure current selection is within bounds
    if (state->selected_cell >= max_cells) {
        state->selected_cell = max_cells - 1;
    }

    // Use the calculated max_cells instead of MAX_CELL_COUNT
    if (direction > 0) {
        state->selected_cell++;
        if (state->selected_cell >= max_cells) {  // Use max_cells here
            state->selected_cell = 0;
        }
    } else {
        if (state->selected_cell > 0) {
            state->selected_cell--;
        } else {
            state->selected_cell = max_cells - 1;  // Use max_cells here
        }
    }

    // Reset screensaver timer since user is actively navigating
    Display_ResetScreenSaver();
}


void Display_AdjustVoltage(int8_t direction)
{
    display_state_t* state = Display_GetState();

    static const float voltage_levels[] = {2.0f, 2.5f, 2.8f, 3.3f, 3.4f, 3.6f, 4.0f, 4.2f};
    static const uint8_t num_levels = sizeof(voltage_levels) / sizeof(voltage_levels[0]);

    // Find current voltage index
    uint8_t current_index = 0;
    for (uint8_t i = 0; i < num_levels; i++) {
        if (fabs(state->target_voltage - voltage_levels[i]) < 0.05f) {
            current_index = i;
            break;
        }
    }

    // Adjust voltage level
    if (direction > 0) {
        current_index++;
        if (current_index >= num_levels) {
            current_index = num_levels - 1;
        }
    } else {
        if (current_index > 0) {
            current_index--;
        }
    }

    state->target_voltage = voltage_levels[current_index];
}





void Display_AdjustTempCard(int8_t direction)
{
    display_state_t* state = Display_GetState();

    if (direction > 0) {
        state->selected_temp_card++;
        if (state->selected_temp_card >= MAX_TEMP_CARDS) {
            state->selected_temp_card = 0;
        }
    } else {
        if (state->selected_temp_card > 0) {
            state->selected_temp_card--;
        } else {
            state->selected_temp_card = MAX_TEMP_CARDS - 1;
        }
    }
}

/*==============================================================================
                          ACTION EXECUTION FUNCTIONS
==============================================================================*/
void Display_ExecuteCellBalance(void)
{
    display_state_t* state = Display_GetState();
    bms_data_t* bms_data = Display_GetBMSData();

    // Toggle balance state for selected cell
    bms_data->cell_balance_states[state->selected_cell] =
        !bms_data->cell_balance_states[state->selected_cell];

    // Call your balance control function
    Set_Balance_State(state->selected_cell + 1,
                     bms_data->cell_balance_states[state->selected_cell]);

    Display_ShowConfirmation("Balance Updated");
}

void Display_ExecuteVoltageSet(void)
{
    display_state_t* state = Display_GetState();

    // Call your voltage setting function
    Set_Output_Voltage(state->selected_cell, state->target_voltage);

    Display_ShowConfirmation("Voltage Set");
    Display_SetScreen(VOLTAGE_SET_MENU);  // Return to menu
}





void Display_ToggleCell(int8_t direction)
{
    display_state_t* state = Display_GetState();

    if (direction > 0) {
        state->selected_cell++;
        if (state->selected_cell >= MAX_CELL_COUNT) state->selected_cell = 0;
    } else {
        if (state->selected_cell == 0) state->selected_cell = MAX_CELL_COUNT - 1;
        else state->selected_cell--;
    }
}

void Display_ExecuteTemperatureSet(void)
{
    display_state_t* state = Display_GetState();
    bms_data_t* bms_data = Display_GetBMSData();

    // Convert target temperature to resistance using NTC formula
    state->target_resistance = ntc_resistance(state->target_temperature);

    // Call your temperature setting function
    TempCard_Set_Resistance(state->selected_temp_card, state->target_resistance);

    // Update the displayed temperature (since BMS may not immediately read back)
    bms_data->temp_card_C[state->selected_temp_card] = state->target_temperature;

    Display_ShowConfirmation("Temperature Set");
}

void Display_ExecuteTestSequence(void)
{
    display_state_t* state = Display_GetState();

    if (!state->test_sequence_running) {
        state->test_sequence_running = 1;
        state->current_test_step = 0;
        Voltage_Sequence_Automatic();
        Display_ShowConfirmation("Test Started");
    } else {
        state->test_sequence_running = 0;
        Display_ShowConfirmation("Test Stopped");
    }
}

/*==============================================================================
                          UTILITY FUNCTIONS
==============================================================================*/
uint8_t Display_GetMenuItemCount(menu_screen_t screen)
{
    switch (screen) {
        case MAIN_MENU:
            return MENU_ITEMS_MAIN;
        case LIVE_DATA_MENU:
            return MENU_ITEMS_LIVE_DATA;
        case CONTROL_MENU:
            return MENU_ITEMS_CONTROL;
        case TEST_MENU:
            return MENU_ITEMS_TEST;
        case SETTINGS_MENU:
            return MENU_ITEMS_SETTINGS;
        case TEMPERATURE_SCREEN:
            return MENU_ITEMS_TEMPERATURE;
        case CELL_MONITOR_SCREEN:
        	return MENU_ITEMS_CELL_MONITOR;

        default:
            return 1;
    }
}

void Display_ResetScreenSaver(void)
{
    display_state_t* state = Display_GetState();
    state->screensaver_timer = HAL_GetTick();
}

void Display_ShowConfirmation(const char* message)
{
    // Simple popup-style confirmation
	u8g2_t* display = &u8g2;

    u8g2_ClearBuffer(display);
    u8g2_DrawFrame(display, 10, 20, 108, 24);
    u8g2_DrawBox(display, 12, 22, 104, 20);

    u8g2_SetDrawColor(display, 0);
    u8g2_SetFont(display, u8g2_font_6x10_mr);
    u8g2_DrawStr(display, 15, 32, message);
    u8g2_SetDrawColor(display, 1);

    u8g2_SendBuffer(display);
    HAL_Delay(1000);
}

void Display_ShowError(const char* error_message)
{
    // Simple error display
	u8g2_t* display = &u8g2;

    u8g2_ClearBuffer(display);
    u8g2_DrawFrame(display, 5, 15, 118, 34);

    u8g2_SetFont(display, u8g2_font_6x10_mr);
    u8g2_DrawStr(display, 8, 25, "ERROR:");
    u8g2_DrawStr(display, 8, 35, error_message);
    u8g2_DrawStr(display, 8, 45, "Press any key...");

    u8g2_SendBuffer(display);
}

button_info_t* Display_GetButtonInfo(button_t button)
{
    if (button < 4) {
        return &button_states[button];
    }
    return NULL;
}
