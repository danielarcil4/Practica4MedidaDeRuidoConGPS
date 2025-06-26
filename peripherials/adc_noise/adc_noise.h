#ifndef __ADC_NOISE_H
#define __ADC_NOISE_H

#include <stdint.h>

// Function to read the ADC value from a specified channel
void my_adc_init(void);
float measure_noise(void);

#endif // __ADC_NOISE_H