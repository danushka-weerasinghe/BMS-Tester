/*
 * Menu.h
 *
 *  Created on: Apr 10, 2025
 *      Author: pavank
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "display.h"
//#include "main.h"

// Menu item structure
typedef struct MenuItem {
    char* text;
    void (*handler)(void);
    struct MenuItem* next;
    struct MenuItem* prev;
    struct MenuItem* parent;
    struct MenuItem* child;
} MenuItem;

// Menu states
typedef enum {
    MENU_STATE_NORMAL,
    MENU_STATE_EDIT
} MenuState;

// Button definitions
#define BUTTON_ENTER  0  // PUSH_BUTTON_01
#define BUTTON_UP     1  // PUSH_BUTTON_02
#define BUTTON_DOWN   2  // PUSH_BUTTON_03
#define BUTTON_BACK   3  // PUSH_BUTTON_04

// Function prototypes
void Menu_Init(void);
void Menu_Handler(uint16_t button);
void Menu_Draw(void);
MenuItem* Menu_AddItem(MenuItem* parent, char* text, void (*handler)(void));

#endif /* INC_MENU_H_ */
