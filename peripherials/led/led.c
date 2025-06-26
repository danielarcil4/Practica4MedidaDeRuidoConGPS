#include "led.h"
#include "hardware/gpio.h"

void leds_init() {
    gpio_init(YELLOW_LED);
    gpio_set_dir(YELLOW_LED, GPIO_OUT);

    gpio_init(GREEN_LED);
    gpio_set_dir(GREEN_LED, GPIO_OUT);

    gpio_init(ORANGE_LED);
    gpio_set_dir(ORANGE_LED, GPIO_OUT);

    gpio_init(RED_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
}

void set_led(uint8_t led, uint8_t state) {
    if (state == ON) {
        gpio_put(led, 1);
    } else {
        gpio_put(led, 0);
    }
}


// cambiar sleeps por timers
void blink_led(uint8_t led, uint32_t times) {
    for (uint32_t i = 0; i < times; i++) {
        set_led(led, ON);
        //sleep_ms(500); // LED on for 500 ms
        set_led(led, OFF);
        //sleep_ms(500); // LED off for 500 ms
    }
}