/*
 * Menu.c
 *
 *  Created on: Apr 10, 2025
 *      Author: pavank
 */

#include "Menu.h"
#include <stdlib.h>

static MenuItem* currentMenuItem = NULL;
static MenuItem* rootMenu = NULL;
static MenuState menuState = MENU_STATE_NORMAL;

// Initialize the menu system with default items
void Menu_Init(void) {
    // Create root menu
    rootMenu = Menu_AddItem(NULL, "Main Menu", NULL);

    // Add main menu items
    MenuItem* settingsMenu = Menu_AddItem(rootMenu, "Settings", NULL);
    MenuItem* statusMenu = Menu_AddItem(rootMenu, "Status", NULL);
    MenuItem* testMenu = Menu_AddItem(rootMenu, "Test", NULL);

    // Add settings submenu items
    Menu_AddItem(settingsMenu, "Brightness", NULL);
    Menu_AddItem(settingsMenu, "Contrast", NULL);

    // Add status submenu items
    Menu_AddItem(statusMenu, "Battery", NULL);
    Menu_AddItem(statusMenu, "Temperature", NULL);

    // Add test submenu items
    Menu_AddItem(testMenu, "Self Test", NULL);
    Menu_AddItem(testMenu, "Calibration", NULL);

    // Set current menu item to root
    currentMenuItem = rootMenu;
}

// Add a new menu item
MenuItem* Menu_AddItem(MenuItem* parent, char* text, void (*handler)(void)) {
    MenuItem* newItem = (MenuItem*)malloc(sizeof(MenuItem));
    newItem->text = text;
    newItem->handler = handler;
    newItem->next = NULL;
    newItem->prev = NULL;
    newItem->parent = parent;
    newItem->child = NULL;

    if (parent != NULL) {
        if (parent->child == NULL) {
            parent->child = newItem;
        } else {
            MenuItem* sibling = parent->child;
            while (sibling->next != NULL) {
                sibling = sibling->next;
            }
            sibling->next = newItem;
            newItem->prev = sibling;
        }
    }

    return newItem;
}

// Draw the current menu
void Menu_Draw(void) {
    u8g2_ClearBuffer(&u8g2);

    // Draw title
    u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
    if (currentMenuItem->parent) {
        u8g2_DrawStr(&u8g2, 0, 12, currentMenuItem->parent->text);
    } else {
        u8g2_DrawStr(&u8g2, 0, 12, currentMenuItem->text);
    }

    // Draw menu items
    MenuItem* item = currentMenuItem->parent ? currentMenuItem->parent->child : currentMenuItem->child;
    int y = 30;
    int count = 0;

    while (item != NULL && count < 3) {
        if (item == currentMenuItem) {
            // Draw selection indicator
            u8g2_DrawFrame(&u8g2, 0, y-12, 128, 15);
        }
        u8g2_DrawStr(&u8g2, 4, y, item->text);
        item = item->next;
        y += 16;
        count++;
    }

    u8g2_SendBuffer(&u8g2);
}

// Handle button inputs
void Menu_Handler(uint16_t button) {
    if (menuState == MENU_STATE_NORMAL) {
        switch (button) {
            case BUTTON_UP:
                if (currentMenuItem->prev) {
                    currentMenuItem = currentMenuItem->prev;
                    Menu_Draw();
                }
                break;

            case BUTTON_DOWN:
                if (currentMenuItem->next) {
                    currentMenuItem = currentMenuItem->next;
                    Menu_Draw();
                }
                break;

            case BUTTON_ENTER:
                if (currentMenuItem->child) {
                    currentMenuItem = currentMenuItem->child;
                    Menu_Draw();
                } else if (currentMenuItem->handler) {
                    currentMenuItem->handler();
                }
                break;

            case BUTTON_BACK:
                if (currentMenuItem->parent) {
                    currentMenuItem = currentMenuItem->parent;
                    Menu_Draw();
                }
                break;
        }
    }
}
