#include <stdio.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include "peripherials/led/led.h"
#include "peripherials/power/power.h"
#include "peripherials/button/button.h"
#include "peripherials/terminal/terminal.h"
#include "peripherials/adc_noise/adc_noise.h"
#include "peripherials/gps_driver/gps_driver.h"
#include "peripherials/memory_driver/memory_driver.h"

void initialize_all_peripherials(void);

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

void test_memory(void);

int main()
{
    stdio_init_all();
    // Wait some time for USB serial connection
	sleep_ms(3000);

    // Initialize all peripherals
    initialize_all_peripherials();

    /**
     * TODO: Delete the following code
     */

    test_memory();
    //******************************* */

    while(1){
        /*
        if(button_pressed()){
            current_state = state_measuring;
        }
        */
    }
}

void test_memory() {
    // Prueba de escritura y lectura
    if(eeprom_write_byte(0x002A, 0x5A)){
        printf("Escritura exitosa\n");
    } else {
        printf("Error de escritura\n");
    } // Escribe 0x5A en 0x002A
    uint8_t value;
    if (eeprom_read_byte(0x002A, &value)) {
        printf("Leído: 0x%02X\n", value);
    } else {
        printf("Error de lectura\n");
    }

    // Escribir una página
    uint8_t page[4] = {0x11, 0x22, 0x33, 0x44};
    eeprom_write_page(0x0030, page, 4);

    uint8_t read_back[4];
    eeprom_read_data(0x0030, read_back, 4);

    for (int i = 0; i < 4; i++) {
        printf("Byte %d: 0x%02X\n", i, read_back[i]);
    }
}

void initialize_all_peripherials(){
    // Initialize the terminal
    //init_terminal();
    // Initialize the GPS
    //init_gps(); 
    // Initialize button with GPIO and IRQ
    //init_button(); 

    // Initialize the LEDs
    //leds_init();

    // Initialize the ADC
    //my_adc_init(); // Initialize ADC on GPIO26 (ADC0)

    // Initialize the I2C interface
    //plataform_t i2c_handler = my_i2c_init(); // Initialize I2C with 40kHz frequency
    // Initialize the EEPROM
    eeprom_init(); // Initialize EEPROM with I2C port and SDA/SCL pins
}

/************************************************************STATE_FUNTIONS*******************************************************************/
/*
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

    if (button_pressed) {
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

    if (button_pressed) {
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
*/