#ifndef __BUTTON_H__
#define __BUTTON_H__

/**
 * @file button.h
 * @brief Header file for button peripheral functions.
 *
 * This file contains function declarations and type definitions for handling button presses.
 */

#include <stdbool.h>

/**
 * @brief Initializes the button peripheral.
 * This function sets up the GPIO pin for the button and configures the interrupt.
 */
void init_button(void);

/**
 * @brief Button interrupt service routine (ISR).
 * This function is called when the button is pressed.
 */
bool button_pressed(void);

#endif // __BUTTON_H__