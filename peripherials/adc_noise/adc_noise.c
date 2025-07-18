#include "adc_noise.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include <math.h>

#define ADC             26
#define ADC_CHANNEL     0 // ADC0 corresponds to GPIO26
#define TIME_TO_SAMPLE  100
#define NUM_SAMPLING    10

const float conversion_factor = 3.3f / (1 << 12);

static inline void finish_measurement(adc_acc_t *adc);
static repeating_timer_t sampling_timer;

void my_adc_init() {
    adc_init();
    adc_gpio_init(ADC);
    adc_select_input(ADC_CHANNEL);
}

static bool sampling_callback(repeating_timer_t *rt) {
    adc_acc_t *adc = (adc_acc_t *)rt->user_data;
    adc->sum += (adc_read()*conversion_factor);
    adc->count++;
    // Retorna true para seguir, false para detener
    if(adc->count == NUM_SAMPLING){
        finish_measurement(adc);
        adc->data_ready = true;
        return false;
    }
    return true;
}

void measure_noise(adc_acc_t *adc) {
    add_repeating_timer_ms(TIME_TO_SAMPLE, sampling_callback, adc, &sampling_timer);
}

static inline void finish_measurement(adc_acc_t *adc) {
    float avg = adc->sum / NUM_SAMPLING;
    // Fórmula dB (simplificada)
    adc->intensidad = 20 * log10(avg);
}
