#include "gps_driver.h"

void init_gps(uart_inst_t * uart_id, uint8_t tx_pin, uint8_t rx_pin) {
    uart_init(uart_id, 9600);
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);
}

bool gps_has_fix(gps_data_t current_data) {
    return current_data.fix_quality;
}

gps_data_t get_gps_data(gps_data_t data) {
    // parsing NMEA desde UART

    // Leer y llenar la estructura
    return data;
}
