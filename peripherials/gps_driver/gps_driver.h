#ifndef __GPS_H__
#define __GPS_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float latitude;   // Latitude in degrees
    float longitude;  // Longitude in degrees
    float altitude;   // Altitude in meters
    float speed;      // Speed in m/s
    float course;     // Course over ground in degrees
    uint8_t fix_quality; // GPS fix quality (0 = no fix, 1 = GPS fix, 2 = DGPS fix)
} gps_data_t;

void init_gps(void);
bool gps_has_fix(gps_data_t current_data);
gps_data_t get_gps_data(gps_data_t current_data);

#endif // __GPS_H__