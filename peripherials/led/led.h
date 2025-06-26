#ifndef __LED_H__
#define __LED_H__
#include <stdint.h>

#define YELLOW_LED      10
#define GREEN_LED       11
#define ORANGE_LED      12
#define RED_LED         13

#define ON              1
#define OFF             0

void leds_init(void);
void set_led(uint8_t led, uint8_t state);
void blink_led(uint8_t led, uint32_t times);

#endif // __LED_H__