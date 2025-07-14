#include "button.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"
#include <stdint.h>

#define BUTTON_PIN      14
#define DEBOUNCE_DELAY  50  // ms

// Prototipo de la función de callback
static void _button_callback(uint gpio, uint32_t events);
static bool _button_timer_callback(repeating_timer_t *rt);

// Estado interno del botón
static repeating_timer_t debounce_timer;
static volatile bool debounce_timer_active = false;
static volatile bool button_irq_flag = false;     // IRQ 
static volatile bool button_valid_press = false;  // flanco confirmado

void init_button(void) {
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);  

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE, true, _button_callback);
}

static void _button_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_RISE) {
        // Inicia el timer de DEBOUNCE_DELAY ms
        add_repeating_timer_ms(DEBOUNCE_DELAY, _button_timer_callback, NULL, &debounce_timer);
    }
}

static bool _button_timer_callback(repeating_timer_t *rt) {
    if (!gpio_get(BUTTON_PIN)) { 
        button_valid_press = true;
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

