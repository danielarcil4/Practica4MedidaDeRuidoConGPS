#include <stdio.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include "peripherials/led/led.h"
#include "peripherials/power/power.h"
#include "peripherials/button/button.h"
#include "peripherials/terminal/terminal.h"
#include "peripherials/adc_noise/adc_noise.h"
#include "peripherials/gps_driver/gps_driver.h"

// UART defines for GPS
#define UART_ID uart1

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5

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
void state_success_write(void);

// Variables globales
state_func_t current_state = state_wait_gps;
char last_noise_db [5] = "0";
gps_data_t gps_data; ///< GPS data structure to store parsed data
bool error_flag = false;

bool red_led_state = false;

int main()
{
    stdio_init_all();

    // Wait some time for USB serial connection
	sleep_ms(3000);

    // Initialize all peripherals
    initialize_all_peripherials();

    while(1){
        current_state();
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
}

/************************************************************STATE_FUNTIONS*******************************************************************/

void state_wait_gps() {
    if (gps_has_fix()) {
        set_led(GREEN_LED, ON);
        // desactivar timers
        current_state = state_idle;
        red_led_state = false; // Reset red LED state
        stop_blink_led(RED_LED); // Stop blinking red LED

        /*printf("Valid GPS data received. Latitude: %.6f %c, Longitude: %.6f %c\n",
                   gps_data.latitude, gps_data.ns,
                   gps_data.longitude, gps_data.ew);*/
                   
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

    //adc_acc_t *adc = {0}; falta un if para que no se ejecute varias veces
    last_noise_db = measure_noise(adc);
    last_location = gps_data; //concatenar todos los datos como char
    current_state = state_saving;
}

void state_saving() {
    set_led(YELLOW_LED, OFF);

    if (save_measurement(last_noise_db, last_location)) { // usar una bandera para saber si ya termino de guardar los datos
        blink_led(ORANGE_LED, 500);
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
    delay_ms(3000); // ejecutarlo con interrupciones
    set_led(RED_LED, OFF);
    error_flag = false;

    current_state = gps_has_fix() ? state_idle : state_wait_gps;
}

void state_terminal() {
    handle_terminal();  // DUMP
    current_state = gps_has_fix() ? state_idle : state_wait_gps;
}

void state_success_write(){
    // Led naranja parpadea brevemente
    blink_led(ORANGE_LED, 1);
}

void test_adc(){
    adc_acc_t adc = {0};
    measure_noise(&adc);
    while(!adc.data_ready){

    }
    adc.data_ready = false;
    printf("medida del adc: %f", adc.intensidad);
}
