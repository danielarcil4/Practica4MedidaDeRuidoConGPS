#include "adc_noise.h"
#include "hardware/adc.h"
#include <math.h>
#include <unistd.h>

#define ADC             26
#define ADC_CHANNEL     0 // ADC0 corresponds to GPIO26

void my_adc_init() {
    adc_init();
    adc_gpio_init(ADC);
    adc_select_input(ADC_CHANNEL);
}

float measure_noise() {
    uint32_t sum = 0;
    for (int i = 0; i < 16000; i++) {
        sum += adc_read();
        sleep_us(625); // para una muestra cada 62.5us ~ 16kHz
    }

    float avg = sum / 16000.0;
    // Fórmula dB (simplificada)
    float dB = 20 * log10(avg);
    return dB;
}
