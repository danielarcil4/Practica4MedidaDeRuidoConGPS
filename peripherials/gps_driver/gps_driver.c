#include "gps_driver.h"

#include "hardware/gpio.h"
#include "hardware/uart.h"

#define GPS_UART_ID uart0 // Use UART0 for GPS communication
#define TX 4          // GPIO pin for TX
#define RX 5          // GPIO pin for RX
#define BAUD_RATE 9600 // Standard GPS baud rate

void init_gps() {
    uart_init(GPS_UART_ID, BAUD_RATE);
    gpio_set_function(TX, GPIO_FUNC_UART);
    gpio_set_function(RX, GPIO_FUNC_UART);
}

bool gps_has_fix(gps_data_t current_data) {
    return current_data.fix_quality;
}

gps_data_t get_gps_data(gps_data_t data) {
    // parsing NMEA desde UART

    // Leer y llenar la estructura
    return data;
}
