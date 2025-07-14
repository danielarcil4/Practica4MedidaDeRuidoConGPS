#include "led.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define TIMER_INTERVAL_MS 1000 // Interval for blinking in milliseconds

volatile bool red_led_state = false;

static repeating_timer_t blink_timer;

bool blink_led_callback(repeating_timer_t *timer) {
    static bool led_state = false;
    led_state = !led_state; // Toggle the LED state
    set_led(timer->user_data, led_state ? ON : OFF);
    return true; // Keep the timer running
}


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

    add_repeating_timer_ms(times, blink_led_callback, (void *)led, &blink_timer);
}

void stop_blink_led(uint8_t led) {
    cancel_repeating_timer(&blink_timer);
    set_led(led, OFF); // Ensure the LED is turned off when stopping the blink
}