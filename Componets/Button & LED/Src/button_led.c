/*
 * button_led.c
 *
 *  Created on: Feb 28, 2025
 *      Author: Pavan Kavinda
 */

#include "button_led.h"
#include "main.h"
#include "gpio_expander.h"

uint8_t mode;
uint8_t id;

uint32_t currentTime;
uint32_t previousTime;

/* Push button array */
const uint16_t BUTTON_PINS[] = {PUSH_BUTTON_01_Pin, PUSH_BUTTON_02_Pin, PUSH_BUTTON_03_Pin, PUSH_BUTTON_04_Pin};

/* LED configuration array */
LED_Config leds[NUM_LEDS] = {
    {GPIOH, LED_01_Pin}, // LED_1
    {GPIOH, LED_02_Pin}, // LED_2
    {GPIOB, LED_03_Pin}, // LED_3
    {GPIOG, LED_04_Pin}, // LED_4
    {GPIOE, LED_05_Pin}, // LED_5
    {GPIOE, LED_06_Pin}, // LED_6
    {GPIOB, LED_07_Pin}, // LED_7
    {GPIOG, LED_08_Pin}, // LED_8
    {GPIOG, LED_09_Pin}  // LED_9
};

LED_Config comm_leds[NUM_COMM_LEDS] = {
    {GPIOI, LED_PC_G_Pin}, // PC_G
    {GPIOI, LED_PC_Y_Pin}, // PC_Y
    {GPIOI, LED_DC_G_Pin}, // DC_G
    {GPIOI, LED_DC_Y_Pin}  // DC_Y
};

LED_Config temp_leds[NUM_TEMP_LEDS] = {
    {GPIOF, CELL12_TEMP_03_LED_Pin}, // CELL12_TEMP_01
    {GPIOC, CELL12_TEMP_02_LED_Pin}, // CELL12_TEMP_02
    {GPIOA, CELL12_TEMP_01_LED_Pin}, // CELL12_TEMP_03
    {GPIOH, CELL11_TEMP_03_LED_Pin}, // CELL11_TEMP_01
    {GPIOH, CELL11_TEMP_02_LED_Pin}, // CELL11_TEMP_02
    {GPIOH, CELL11_TEMP_01_LED_Pin}  // CELL11_TEMP_03
};

Cell_LED_Config cell_leds[NUM_CELL_LEDS] = {
    {&hi2c2, GPIO_EXPANDER_ID_01, CELL_01_LED_01},  // CELL12_CELL1
    {&hi2c2, GPIO_EXPANDER_ID_01, CELL_02_LED_01},  // CELL12_CELL2
    {&hi2c2, GPIO_EXPANDER_ID_01, CELL_03_LED_01},  // CELL12_CELL3
    {&hi2c2, GPIO_EXPANDER_ID_02, CELL_01_LED_01},  // CELL12_CELL4
    {&hi2c2, GPIO_EXPANDER_ID_02, CELL_02_LED_01},  // CELL12_CELL5
    {&hi2c2, GPIO_EXPANDER_ID_02, CELL_03_LED_01},  // CELL12_CELL6
    {&hi2c2, GPIO_EXPANDER_ID_03, CELL_01_LED_01},  // CELL12_CELL7
    {&hi2c2, GPIO_EXPANDER_ID_03, CELL_02_LED_01},  // CELL12_CELL8
    {&hi2c2, GPIO_EXPANDER_ID_03, CELL_03_LED_01},  // CELL12_CELL9
    {&hi2c2, GPIO_EXPANDER_ID_04, CELL_01_LED_01},  // CELL12_CELL10
    {&hi2c2, GPIO_EXPANDER_ID_04, CELL_02_LED_01},  // CELL12_CELL11
    {&hi2c2, GPIO_EXPANDER_ID_04, CELL_03_LED_01},  // CELL12_CELL12
    {&hi2c3, GPIO_EXPANDER_ID_01, CELL_01_LED_01},  // CELL11_CELL1
    {&hi2c3, GPIO_EXPANDER_ID_01, CELL_02_LED_01},  // CELL11_CELL2
    {&hi2c3, GPIO_EXPANDER_ID_01, CELL_03_LED_01},  // CELL11_CELL3
    {&hi2c3, GPIO_EXPANDER_ID_02, CELL_01_LED_01},  // CELL11_CELL4
    {&hi2c3, GPIO_EXPANDER_ID_02, CELL_02_LED_01},  // CELL11_CELL5
    {&hi2c3, GPIO_EXPANDER_ID_02, CELL_03_LED_01},  // CELL11_CELL6
    {&hi2c3, GPIO_EXPANDER_ID_03, CELL_01_LED_01},  // CELL11_CELL7
    {&hi2c3, GPIO_EXPANDER_ID_03, CELL_02_LED_01},  // CELL11_CELL8
    {&hi2c3, GPIO_EXPANDER_ID_03, CELL_03_LED_01},  // CELL11_CELL9
    {&hi2c3, GPIO_EXPANDER_ID_04, CELL_01_LED_01},  // CELL11_CELL10
    {&hi2c3, GPIO_EXPANDER_ID_04, CELL_02_LED_01},  // CELL11_CELL11
    {&hi2c3, GPIO_EXPANDER_ID_04, CELL_03_LED_01}   // CELL11_CELL12
};

GPIO_PinState cell_led_states[NUM_CELL_LEDS];

// Initialize all LEDs (set to OFF)
void LED_Init(void) {

    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, GPIO_PIN_RESET);
    }

    for (uint8_t i = 0; i < NUM_COMM_LEDS; i++) {
        HAL_GPIO_WritePin(comm_leds[i].port, comm_leds[i].pin, GPIO_PIN_RESET);
    }

    for (uint8_t i = 0; i < NUM_TEMP_LEDS; i++) {
        HAL_GPIO_WritePin(temp_leds[i].port, temp_leds[i].pin, GPIO_PIN_SET); // Active low
    }

    // Initialize cell LED states and set hardware LEDs to OFF
    for (uint8_t i = 0; i < NUM_CELL_LEDS; i++) {
        cell_led_states[i] = GPIO_PIN_SET; // Assume LEDs are off (active-low logic)
        I2C_HandleTypeDef *i2c_handle = cell_leds[i].i2c_handle;
        uint8_t expander_id = cell_leds[i].expander_id;
        uint16_t pin = cell_leds[i].pin;

        // Ensure LEDs are turned off at initialization
        Expander_SetPinState(i2c_handle, expander_id, pin, GPIO_PIN_SET); // Active low
    }

}

// Set a specific LED
void LED_Set(LED_ID led, LED_Set_State state) {
    if (led <= NUM_LEDS) {
        HAL_GPIO_WritePin(leds[led - 1].port, leds[led - 1].pin, (state == High) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

// Toggle a specific LED
void LED_Toggle(LED_ID led) {
    if (led <= NUM_LEDS) {
        HAL_GPIO_TogglePin(leds[led - 1].port, leds[led - 1].pin);
    }
}

void LED_RS485(CommLED_ID led, LED_Set_State state) {
    if (led < NUM_COMM_LEDS) {
        HAL_GPIO_WritePin(comm_leds[led].port, comm_leds[led].pin, (state == High) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

void LED_ToggleRS485(CommLED_ID led) {
    if (led < NUM_COMM_LEDS) {
        HAL_GPIO_TogglePin(comm_leds[led].port, comm_leds[led].pin);
    }
}

void LED_Temp(TempLED_ID led, LED_Set_State state) {
    if (led < NUM_TEMP_LEDS) {
        HAL_GPIO_WritePin(temp_leds[led].port, temp_leds[led].pin, (state == High) ? GPIO_PIN_RESET : GPIO_PIN_SET); // Active low logic
    }
}

void LED_ToggleTemp(TempLED_ID led) {
    if (led < NUM_TEMP_LEDS) {
        HAL_GPIO_TogglePin(temp_leds[led].port, temp_leds[led].pin);
    }
}

void LED_Cell(CellLED_ID cell, LED_Set_State state) {
    if (cell < NUM_CELL_LEDS) {
        I2C_HandleTypeDef *i2c_handle = cell_leds[cell].i2c_handle;
        uint8_t expander_id = cell_leds[cell].expander_id;
        uint16_t pin = cell_leds[cell].pin;

        // Send command to the appropriate GPIO expander
        Expander_SetPinState(i2c_handle, expander_id, pin,
                             (state == High) ? GPIO_PIN_RESET : GPIO_PIN_SET);

        cell_led_states[cell] = (state == High) ? GPIO_PIN_RESET : GPIO_PIN_SET;
    }
}

void LED_ToggleCell(CellLED_ID cell) {
    if (cell < NUM_CELL_LEDS) {
        I2C_HandleTypeDef *i2c_handle = cell_leds[cell].i2c_handle;
        uint8_t expander_id = cell_leds[cell].expander_id;
        uint16_t pin = cell_leds[cell].pin;

        // Toggle the state in the software state array
        cell_led_states[cell] = (cell_led_states[cell] == GPIO_PIN_RESET) ? GPIO_PIN_SET : GPIO_PIN_RESET;

        // Set the new state on the GPIO expander
        Expander_SetPinState(i2c_handle, expander_id, pin, cell_led_states[cell]);
    }
}

// Set all LEDs to a specific state
void LED_SetAll(LED_Set_State state) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, state == High ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
    for (uint8_t i = 0; i < NUM_COMM_LEDS; i++) {
        HAL_GPIO_WritePin(comm_leds[i].port, comm_leds[i].pin, state == High ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
    for (uint8_t i = 0; i < NUM_TEMP_LEDS; i++) {
        HAL_GPIO_WritePin(temp_leds[i].port, temp_leds[i].pin, state == High ? GPIO_PIN_RESET : GPIO_PIN_SET); // Active low
    }
    for (uint8_t i = 0; i < NUM_CELL_LEDS; i++) {
        I2C_HandleTypeDef *i2c_handle = cell_leds[i].i2c_handle;
        uint8_t expander_id = cell_leds[i].expander_id;
        uint16_t pin = cell_leds[i].pin;

        // Send command to the GPIO expander for each cell LED
        Expander_SetPinState(i2c_handle, expander_id, pin, state == High ? GPIO_PIN_RESET : GPIO_PIN_SET); // Active low
        // Update cell LED state array
        cell_led_states[i] = (state == High) ? GPIO_PIN_RESET : GPIO_PIN_SET;
    }
}

// Toggle all LEDs
void LED_ToggleAll(void) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        HAL_GPIO_TogglePin(leds[i].port, leds[i].pin);
    }
}

void LED_Test_All(void) {
    // Turn ON Standard LEDs in sequence
    for (LED_ID led = LED_1; led <= NUM_LEDS; led++) {
        LED_Set(led, High);
        HAL_Delay(100);
        LED_Set(led, Low);
    }

    // Blink Communication LEDs twice
    for (int i = 0; i < 2; i++) {
        for (CommLED_ID comm_led = PC_G; comm_led < NUM_COMM_LEDS; comm_led++) {
            LED_RS485(comm_led, High);
        }
        HAL_Delay(200);
        for (CommLED_ID comm_led = PC_G; comm_led < NUM_COMM_LEDS; comm_led++) {
            LED_RS485(comm_led, Low);
        }
        HAL_Delay(200);
    }

    // Activate all Temperature LEDs momentarily
    for (TempLED_ID temp_led = CELL12_TEMP_01; temp_led < NUM_TEMP_LEDS; temp_led++) {
        LED_Temp(temp_led, High);
    }
    HAL_Delay(500); // Hold for 500ms
    for (TempLED_ID temp_led = CELL12_TEMP_01; temp_led < NUM_TEMP_LEDS; temp_led++) {
        LED_Temp(temp_led, Low);
    }

    // Sequentially turn ON and OFF Cell LEDs
    for (CellLED_ID cell_led = CELL12_CELL1; cell_led < NUM_CELL_LEDS; cell_led++) {
        LED_Cell(cell_led, High);
        HAL_Delay(100); // Delay between LEDs for visibility
        LED_Cell(cell_led, Low);
    }
}

uint8_t DIP_GetMode(void)
{
    mode = 0;

    /* Read MODE switches from GPIOG (first 4 switches) */
    if(HAL_GPIO_ReadPin(GPIOG, DIP_SWITCH_MODE_04_Pin) == GPIO_PIN_RESET) mode |= 0x01;
    if(HAL_GPIO_ReadPin(GPIOG, DIP_SWITCH_MODE_03_Pin) == GPIO_PIN_RESET) mode |= 0x02;
    if(HAL_GPIO_ReadPin(GPIOG, DIP_SWITCH_MODE_02_Pin) == GPIO_PIN_RESET) mode |= 0x04;
    if(HAL_GPIO_ReadPin(GPIOG, DIP_SWITCH_MODE_01_Pin) == GPIO_PIN_RESET) mode |= 0x08;

    return mode;
}

uint8_t DIP_GetID(void)
{
    id = 0;

    /* Read ID switches from GPIOD (last 4 switches) */
    if(HAL_GPIO_ReadPin(GPIOD, DIP_SWITCH_04_Pin) == GPIO_PIN_RESET) id |= 0x01;
    if(HAL_GPIO_ReadPin(GPIOD, DIP_SWITCH_03_Pin) == GPIO_PIN_RESET) id |= 0x02;
    if(HAL_GPIO_ReadPin(GPIOD, DIP_SWITCH_02_Pin) == GPIO_PIN_RESET) id |= 0x04;
    if(HAL_GPIO_ReadPin(GPIOD, DIP_SWITCH_01_Pin) == GPIO_PIN_RESET) id |= 0x08;

    return id;
}

void Push_ButtonHandler(uint16_t GPIO_Pin)
{
    currentTime = HAL_GetTick();

    for(int j = 0; j < 4; j++) {
        if((GPIO_Pin == BUTTON_PINS[j]) && (currentTime - previousTime > 150)) {
//        	Menu_Handler(j);
            LED_Toggle(j+1);  // Keep your existing LED toggle
            previousTime = currentTime;
            break;
        }
    }
}
