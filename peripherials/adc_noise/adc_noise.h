#ifndef __ADC_NOISE_H
#define __ADC_NOISE_H

#include <stdint.h>

typedef struct {
    uint32_t count;
    uint32_t sum;
} adc_acc_t;

// Function to read the ADC value from a specified channel
void my_adc_init(void);
float measure_noise(void);

#endif // __ADC_NOISE_H