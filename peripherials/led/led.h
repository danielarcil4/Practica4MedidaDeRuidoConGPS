#ifndef __LED_H__
#define __LED_H__
#include <stdint.h>

/**
 * @file led.h
 * @brief Header file for LED control functions.
 * This file contains the function declarations and macros for controlling LEDs.
 */

#define YELLOW_LED      19 ///< GPIO pin for yellow LED
#define GREEN_LED       20 ///< GPIO pin for green LED
#define ORANGE_LED      18 ///< GPIO pin for orange LED
#define RED_LED         17 ///< GPIO pin for red LED

#define ON              1 ///< LED state ON
#define OFF             0 ///< LED state OFF

/**
 * @brief Initializes the LEDs.
 * This function sets up the GPIO pins for the LEDs.
 */
void leds_init(void);

/**
 * @brief Sets the state of a specific LED.
 * This function turns on or off a specified LED.
 * @param led The GPIO pin number of the LED to control.
 * @param state The state to set the LED to (ON or OFF).
 * @note The state should be either ON (1) or OFF (0).
 */
void set_led(uint8_t led, uint8_t state);

/**
 * @brief Blinks a specific LED a specified number of times.
 * This function makes the specified LED blink a certain number of times.
 * @param led The GPIO pin number of the LED to blink.
 * @param times The number of times to blink the LED.
 */
void blink_led(uint8_t led, uint32_t times);

/**
 * @brief Stops blinking a specific LED.
 * This function stops the blinking of the specified LED.
 */
void stop_blink_led(uint8_t led);

#endif // __LED_H__