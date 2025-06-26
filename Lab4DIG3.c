#include <stdio.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include "peripherials/led/led.h"
#include "peripherials/power/power.h"
#include "peripherials/button/button.h"
#include "peripherials/terminal/terminal.h"
#include "peripherials/adc_noise/adc_noise.h"
#include "peripherials/gps_driver/gps_driver.h"
#include "peripherials/eeprom_i2c/i2c.h"

#define YELLOW_LED      10
#define GREEN_LED       11
#define ORANGE_LED      12
#define RED_LED         13
#define BUTTON          14

#define SDA             0
#define SCL             1
#define I2C_HANDLE      i2c0 // Use i2c0 as the I2C handle
#define ADC             26
#define ADC_CHANNEL     0 // ADC0 corresponds to GPIO26

#define ON              1
#define OFF             0

bool button_pressed = false;

void initialize_all_peripherials(void);

void button_isr_gpio(void);

// Function prototypes for state functions
typedef void(*state_func_t)(void);

void state_idle(void);
void state_measuring(void);
void state_saving(void);
void state_error(void);
void state_wait_gps(void);
void state_terminal(void);

// Variables globales
state_func_t current_state = state_wait_gps;
float last_noise_db = 0.0;
gps_data_t last_location;
bool error_flag = false;

int main()
{
    stdio_init_all();

    // Wait some time for USB serial connection
	sleep_ms(3000);

    // Initialize all peripherals
    initialize_all_peripherials();

    while(1){
        if(button_pressed){
            button_pressed = false;

            measure_noise();
        }
    }
}

void initialize_all_peripherials(){
    // Initialize the terminal
    init_terminal();
    // Initialize the power management
    init_power();
    // Initialize the button
    button_init(BUTTON, GPIO_IN);
    
    // Initialize the GPS
    init_gps(uart0, 4, 5); // Initialize GPS on UART0 with TX on GPIO4 and RX on GPIO5
    // Initialize the EEPROM I2C interface
    

    // Initialize the LEDs
    led_init(GREEN_LED, GPIO_OUT);
    led_init(ORANGE_LED, GPIO_OUT);
    led_init(RED_LED, GPIO_OUT);
    led_init(YELLOW_LED, GPIO_OUT);

    // Initialize the ADC
    my_adc_init(ADC, ADC_CHANNEL); // Initialize ADC on GPIO26 (ADC0)

    // Initialize the I2C interface
    plataform_t i2c_handler = my_i2c_init(I2C_HANDLE, 40000); // Initialize I2C with 40kHz frequency
}

/************************************************************STATE_FUNTIONS*******************************************************************/

static void button_isr_gpio(void)
{
	button_pressed = true;
}

void state_wait_gps() {
    if (gps_has_fix()) {
        set_led(GREEN_LED, ON);
        current_state = state_idle;
    } else {
        blink_led(RED_LED, 1);  // 1 Hz
    }
}

void state_idle() {
    set_led(GREEN_LED, ON);

    if (!gps_has_fix()) {
        current_state = state_wait_gps;
        return;
    }

    if (button_pressed()) {
        current_state = state_measuring;
        return;
    }

    if (terminal_has_command()) {
        current_state = state_terminal;
        return;
    }
}

void state_measuring() {
    set_led(GREEN_LED, OFF);
    set_led(YELLOW_LED, ON);

    if (!gps_has_fix()) {
        current_state = state_error;
        return;
    }

    if (button_pressed()) {
        error_flag = true;
        current_state = state_error;
        return;
    }

    last_noise_db = measure_noise();
    last_location = get_gps_data();

    if (!last_location.valid) {
        error_flag = true;
        current_state = state_error;
    } else {
        current_state = state_saving;
    }
}

void state_saving() {
    set_led(YELLOW_LED, OFF);

    if (save_measurement(last_noise_db, last_location)) {
        blink_led(ORANGE_LED, 1);
    } else {
        error_flag = true;
        current_state = state_error;
        return;
    }

    current_state = state_idle;
}

void state_error() {
    set_led(YELLOW_LED, OFF);
    set_led(GREEN_LED, OFF);
    set_led(RED_LED, ON);
    delay_ms(3000);
    set_led(RED_LED, OFF);
    error_flag = false;

    current_state = gps_has_fix() ? state_idle : state_wait_gps;
}

void state_terminal() {
    handle_terminal();  // DUMP
    current_state = gps_has_fix() ? state_idle : state_wait_gps;
}