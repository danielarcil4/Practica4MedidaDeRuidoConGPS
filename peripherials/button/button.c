#include "button.h"

void button_init(uint8_t gpio_pin, uint8_t gpio_dir) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, gpio_dir);
    gpio_pull_down(gpio_pin); // Enable pull-down resistor on the button GPIO
}

void button_set_callback(uint gpio_button, uint32_t event_mask, bool enabled, gpio_irq_callback_t callback) {
    gpio_set_irq_enabled_with_callback(gpio_button, event_mask, true, callback);
}