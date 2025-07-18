#ifndef __ADC_NOISE_H
#define __ADC_NOISE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    uint8_t count;
    float sum;
    float intensidad;
    bool data_ready;
    bool taking_data;
} adc_acc_t;


// Function to read the ADC value from a specified channel
void my_adc_init(void);
void measure_noise(adc_acc_t *adc);

#endif // __ADC_NOISE_H