#ifndef __GPS_H__
#define __GPS_H__
#include <stdint.h>
#include <string.h>
#include "hardware/gpio.h"
#include "hardware/uart.h"

typedef struct {
    float latitude;   // Latitude in degrees
    float longitude;  // Longitude in degrees
    float altitude;   // Altitude in meters
    float speed;      // Speed in m/s
    float course;     // Course over ground in degrees
    uint8_t fix_quality; // GPS fix quality (0 = no fix, 1 = GPS fix, 2 = DGPS fix)
} gps_data_t;

void init_gps(uart_inst_t * uart_id, uint8_t tx_pin, uint8_t rx_pin);
bool gps_has_fix();
gps_data_t get_gps_data();

#endif // __GPS_H__