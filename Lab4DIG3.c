/**
 * @file Lab4DIG3.c
 * @brief Main file for the Lab 4 project.
 * This file contains the main function and state machine for the project.
 * It initializes all peripherals and handles the main loop for measuring noise with GPS.
 * \author Santiago Vargas, Santiago Arcila
 * \version 1.0
 * \date 2025-07-19
 * \TODO: Implement error handling and improve state management.
 */


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

#include <string.h>

#define UART_ID uart1 ///< UART instance to use
#define UART_TX_PIN 4 ///< UART TX pin
#define UART_RX_PIN 5 ///< UART RX pin

/**
 * @brief Initializes all peripherals used in the project.
 * This function sets up the terminal, GPS, button, LEDs, ADC, and EEPROM.
 */
void initialize_all_peripherials(void);

typedef void(*state_func_t)(void); ///< Type definition for state functions

/**
 * @brief State idle function.
 * This function is called when the system is idle, waiting for user input or GPS fix.
 * It checks for button presses and terminal commands to transition to other states.
 * Turns on the green LED when in idle state.
 * If a GPS fix is not available, it transitions to the wait GPS state.
 */
void state_idle(void);

/**
 * @brief State measuring function.
 * This function is called when the system is measuring noise levels and GPS data.
 * It handles the ADC measurements and prepares data for saving.
 * Turns on the yellow LED while measuring.
 * If an error occurs, it transitions to the error state.
 */
void state_measuring(void);

/**
 * @brief State saving function.
 * This function is called when the system is saving the measured data.
 */

void state_saving(void);

/**
 * @brief State error function.
 * This function is called when an error occurs in the system.
 * It handles the error state by turning on the red LED and waiting for a GPS fix.
 */
void state_error(void);

/**
 * @brief State wait GPS function.
 * This function is called when the system is waiting for a GPS fix.
 * It turns on the red LED to indicate waiting and checks for GPS fix status.
 */
void state_wait_gps(void);

/**
 * @brief State terminal function.
 * This function is called when the system is in terminal mode.
 * It handles terminal commands and transitions back to idle state if a GPS fix is available.
 */
void state_terminal(void);

/**
 * @brief State success write function.
 * This function is called when data is successfully written to memory.
 * It blinks the orange LED briefly to indicate success.
 * This function is called after a successful write operation.
 */
void state_success_write(void);

void data_to_string();
void test_adc(void);
void test_memory(void);

// Variables globales
state_func_t current_state = state_wait_gps;///< Pointer to the current state function
bool error_flag = false; ///< Flag to indicate if an error has occurred

bool red_led_state = false; ///< Flag to indicate if the red LED is blinking
char datos [30]={0};
adc_acc_t adc = {0}; ///< ADC data structure for noise measurement
gps_data_t gps_data = {0}; ///< GPS data structure to hold GPS information

int main()
{
    stdio_init_all();

    // Wait some time for USB serial connection
	sleep_ms(3000);

    // Initialize all peripherals
    initialize_all_peripherials();
    //set index = 0
    printf("Initializing...\n");
    //eeprom_set_index(0);
    while(1){
        //test_adc();
        current_state();

        //test_memory();

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
    init_terminal();
    // Initialize the GPS
    //init_gps();
    init_gps(UART_ID, UART_TX_PIN, UART_RX_PIN, &gps_data); 
    // Initialize button with GPIO and IRQ
    init_button(); 

    // Initialize the LEDs
    leds_init();

    // Initialize the ADC
    my_adc_init(); // Initialize ADC on GPIO26 (ADC0)

    // Initialize the EEPROM
    eeprom_init();
}

/************************************************************STATE_FUNTIONS*******************************************************************/

void state_wait_gps() {
    printf("Waiting for GPS fix...\n");
    if (gps_has_fix()) {
        set_led(GREEN_LED, ON);
        // desactivar timers
        current_state = state_idle;
        red_led_state = false; // Reset red LED state
        stop_blink_led(RED_LED); // Stop blinking red LED

    } else if (!red_led_state){
        //activar timers
        red_led_state = true;
        blink_led(RED_LED, 1000); // Blink red LED to indicate waiting for GPS fix
        //blink red calls a timer
    }
}

void state_idle() {
    //set_led(GREEN_LED, ON); //Comenté esta linea porque el led verde ya se encendió en otra parte antes de pasaar a esta funcion
    if (!gps_has_fix()) {
        set_led(GREEN_LED, OFF);
        current_state = state_wait_gps;
        return;
    }

    if (button_pressed) {
        set_led(GREEN_LED, OFF);
        set_led(YELLOW_LED, ON);
        current_state = state_measuring;
        return;
    }

    if (terminal_has_command()) {
        current_state = state_terminal;
        return;
    }    
    
}

void state_measuring() {
    if (!gps_has_fix()) {
        current_state = state_error;
        return;
    }

    if (button_pressed) {
        error_flag = true;
        current_state = state_error;
        return;
    }
    /**
     * TODO: Cambiar las siguientes funciones
     * Tiene que ser polling más interrupts, 
     * Debe activar una especie de timer para hacer la medición de datos del adc
     * Y al mismo tiempo, leer el GPS.
     * 
     * ¿Qué inconveniente tiene estar leyendo el puerto UART del GPS
     * al mismo tiempo que se lee el ADC?
     * ¿el UART es muy pesado/lento y generaría problemas de latencia?
     * ¿ocurriría que mientras está leyendo del ADC, 
     * se activa el UART y se pierde la lectura del ADC?
     * 
     * ¿Qué hacer?
     * ¿Utilizar DMA? ¿Utilizar FIFO para UART?
     */

    if(!adc.taking_data){
        measure_noise(&adc);
        
    }
    if(adc.data_ready){
        adc.data_ready = false;
        adc.taking_data = false; // Reset taking_data flag
        //data_to_string(gps_data, adc.intensidad, datos);
        printf("Data ready: %f dB, Lat: %.6f, Lon: %.6f\n", adc.intensidad, gps_data.latitude, gps_data.longitude);
        current_state = state_saving;
    }

}

void state_saving() {
    set_led(YELLOW_LED, OFF);

    // if (save_measurement(datos)) { // usar una bandera para saber si ya termino de guardar los datos
    //     blink_led(ORANGE_LED, 500);
    // } else {
    //     error_flag = true;
    //     current_state = state_error;
    //     return;
    // }

    current_state = state_idle;
}

void state_error() {
    set_led(YELLOW_LED, OFF);
    set_led(GREEN_LED, OFF);
    set_led(RED_LED, ON);
    sleep_ms(3000); // ejecutarlo con interrupciones
    set_led(RED_LED, OFF);
    error_flag = false;

    current_state = gps_has_fix() ? state_idle : state_wait_gps;
}

void state_terminal() {
    //handle_terminal();  // DUMP
    current_state = gps_has_fix() ? state_idle : state_wait_gps;
}

void state_success_write(){
    // Led naranja parpadea brevemente100
    blink_led(ORANGE_LED, 1);
}


void data_to_string() {
    datos[0] = '\0'; // Limpiar el buffer

    // Formatear cada entrada, puedes cambiar el formato según lo que necesites
    sprintf(datos, "%.2f;%.6f;%c;%.6f;%c;%c;", adc.intensidad,
        gps_data.latitude, gps_data.ns, 
        gps_data.longitude, gps_data.ew, 
        gps_data.status);
}

void test_adc(){
    gps_data_t gps_array = {
        12.345678, 
        'N', 
        98.765432, 
        'E', 
        'A'
    };
    // if(!adc.taking_data){
    //     measure_noise(&adc);
    // }
    // if(adc.data_ready){
    //     adc.data_ready = false;
    //     adc.taking_data = false; // Reset taking_data flag
    //     printf("ADC Data: %.2f dB\n", adc.intensidad);
    //     printf("GPS Data: Lat: %.6f %c, Lon: %.6f %c, Status: %c\n", 
    //         gps_data.latitude, gps_data.ns, 
    //         gps_data.longitude, gps_data.ew, 
    //         gps_data.status);
    // }

    adc_acc_t adc = {0};
    measure_noise(&adc);
    while(!adc.data_ready){

    }
    adc.data_ready = false;
    printf("medida del adc: %f", adc.intensidad);

    
}

