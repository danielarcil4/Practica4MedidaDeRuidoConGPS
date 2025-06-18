#include "led.h"

void led_init(uint8_t gpio_pin, uint8_t gpio_dir) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, gpio_dir);
}
