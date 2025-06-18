#ifndef __BUTTON_H__
#define __BUTTON_H__
#include <stdint.h>
#include "hardware/irq.h"
#include "hardware/gpio.h"

void button_init(uint8_t gpio_pin, uint8_t gpio_dir);
void button_set_callback(uint gpio_button, uint32_t event_mask, bool enabled, gpio_irq_callback_t callback);


#endif // __BUTTON_H__