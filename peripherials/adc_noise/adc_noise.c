#include "adc_noise.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include <math.h>

#define ADC             26
#define ADC_CHANNEL     0 // ADC0 corresponds to GPIO26

static repeating_timer_t sampling_timer;

void my_adc_init() {
    adc_init();
    adc_gpio_init(ADC);
    adc_select_input(ADC_CHANNEL);
}

static bool sampling_callback(repeating_timer_t *rt) {
    adc_acc_t *acc = (adc_acc_t *)rt->user_data;
    acc->sum += adc_read();
    acc->count++;
    // Retorna true para seguir, false para detener
    return acc->count < 16000;
}

float measure_noise() {
    adc_acc_t acc = {0};
    add_repeating_timer_ms(-1, sampling_callback, &acc, &sampling_timer);
    uint8_t dB = 0.0;
    
    return dB;
}

void finish_measurement(uint8_t sum) {
    float avg = sum / 16000.0;
    // Fórmula dB (simplificada)
    float dB = 20 * log10(avg);
}
