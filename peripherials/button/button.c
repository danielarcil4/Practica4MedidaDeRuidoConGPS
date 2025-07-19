#include "button.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"
#include <stdint.h>
#include <stdio.h>

/**
 * @file button.c
 * @brief Implementation of button peripheral functions.
 */

#define BUTTON_PIN      10 ///< GPIO pin for the button
#define DEBOUNCE_DELAY  50  ///< Debounce delay in milliseconds

/**
 * @brief Initializes the button peripheral.
 * This function sets up the GPIO pin for the button and configures the interrupt.
 * It also initializes a timer for debouncing the button press.
 */
static void _button_callback(uint gpio, uint32_t events);

/**
 * @brief Timer callback function for debouncing the button press.
 * This function is called when the debounce timer expires.
 * It checks the button state and sets the valid press flag if the button is still pressed.
 * @param rt Pointer to the repeating timer structure.
 */
static bool _button_timer_callback(repeating_timer_t *rt);

static repeating_timer_t debounce_timer; ///< Timer for debouncing the button press
static volatile bool debounce_timer_active = false; ///< Flag to indicate if the debounce timer is active
static volatile bool button_irq_flag = false;     ///< Flag to indicate if the button interrupt has occurred
static volatile bool button_valid_press = false;  ///< Flag to indicate a valid button press

void init_button(void) {
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);  

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE, true, _button_callback);
}

static void _button_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_RISE) {
        // Inicia el timer de DEBOUNCE_DELAY ms
        gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE, false, _button_callback);
        add_repeating_timer_ms(DEBOUNCE_DELAY, _button_timer_callback, NULL, &debounce_timer);
    }
}

static bool _button_timer_callback(repeating_timer_t *rt) {
    if (gpio_get(BUTTON_PIN)) { 
        button_valid_press = true;
        gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE, true, _button_callback);
        return false;  // Detiene el timer
    }
    return true;  // Mantiene el timer activo
}

bool button_pressed(void) {
    if (button_valid_press) {
        button_valid_press = false;
        return true;
    }
    return false;
}

