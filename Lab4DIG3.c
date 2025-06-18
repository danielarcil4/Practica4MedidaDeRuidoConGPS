#include <stdio.h>

#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "peripherials/eeprom_i2c/eeprom_plataform.h"
#include "peripherials/adc_noise/adc_noise.h"
#include "peripherials/button/button.h"
#include "peripherials/led/led.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

#define GPIO_GREEN_LED  9
#define GPIO_ORANGE_LED 10
#define GPIO_RED_LED    11
#define GPIO_BUTTON     12
#define GPIO_SDA        0
#define GPIO_SCL        1
#define I2C_HANDLE      i2c0 // Use i2c0 as the I2C handle
#define GPIO_ADC        26
#define ADC_CHANNEL    0 // ADC0 corresponds to GPIO26

bool button_pressed = false;

typedef void(*current_state)(void);

static void button_isr_gpio(void)
{
	button_pressed = true;
}

int main()
{
    stdio_init_all();

    // Wait some time for USB serial connection
	sleep_ms(3000);

    // Initialize the button GPIO
    gpio_init(GPIO_BUTTON);
    gpio_set_dir(GPIO_BUTTON, GPIO_IN);
    gpio_pull_down(GPIO_BUTTON); // Enable pull-up resistor on the button GPIO
    //gpio_set_irq_enabled_with_callback(GPIO_BUTTON, GPIO_IRQ_EDGE_RISE, true, button_isr_gpio);

    // Initialize the LEDs
    led_init(GPIO_GREEN_LED, GPIO_OUT);
    led_init(GPIO_ORANGE_LED, GPIO_OUT);
    led_init(GPIO_RED_LED, GPIO_OUT);

    // Initialize the ADC
    my_adc_init(GPIO_ADC, ADC_CHANNEL); // Initialize ADC on GPIO26 (ADC0)

    // Initialize the I2C interface
    plataform_t i2c_handler = my_i2c_init(I2C_HANDLE, 40000); // Initialize I2C with 40kHz frequency

    while(1){
        if(button_pressed){
            button_pressed = false;

            const float conversion_factor = 3.3f / (1 << 12);
            uint16_t result = adc_read();
            printf("Raw value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);
            sleep_ms(500);
        }
    }
}
