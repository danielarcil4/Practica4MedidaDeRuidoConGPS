#include "adc_noise.h"
#include "hardware/adc.h"

void my_adc_init(uint8_t gpio_pin, uint8_t adc_channel) {
    adc_init();
    adc_gpio_init(gpio_pin);
    adc_select_input(adc_channel);
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
