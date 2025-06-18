#ifndef __LED_H__
#define __LED_H__
#include <stdint.h>
#include "hardware/gpio.h"

void led_init(uint8_t gpio_pin, uint8_t gpio_dir);

#endif // __LED_H__