#ifndef __ADC_NOISE_H
#define __ADC_NOISE_H
#include <stdint.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Function to read the ADC value from a specified channel
void my_adc_init(uint8_t gpio_pin, uint8_t adc_channel);
float measure_noise(void);

#endif // __ADC_NOISE_H