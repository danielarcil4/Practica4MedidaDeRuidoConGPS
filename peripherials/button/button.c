#include "button.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"
#include <stdint.h>

#define BUTTON_PIN      14
#define DEBOUNCE_DELAY  50  // ms

// Estado interno del botón
static volatile bool button_irq_flag = false;     // IRQ activada
static volatile bool button_valid_press = false;  // flanco confirmado
static volatile uint8_t debounce_counter = 0;

void button_callback(uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) {
        button_irq_flag = true;
        debounce_counter = 0; // empieza conteo de debounce
    }
}

void init_button(void) {
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);  

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_gpio_callback);
}

// Esta función debe llamarse cada 1ms desde un timer IRQ
void button_timer_irq_handler(void) {
    if (button_irq_flag) {
        debounce_counter++;

        if (debounce_counter >= DEBOUNCE_DELAY) {
            if (gpio_get(BUTTON_PIN)) { 
                button_valid_press = true;
            }
            button_irq_flag = false;
        }
    }
}

bool button_pressed(void) {
    if (button_valid_press) {
        button_valid_press = false;
        return true;
    }
    return false;
}
