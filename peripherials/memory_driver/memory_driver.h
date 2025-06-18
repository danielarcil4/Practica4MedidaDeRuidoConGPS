#ifndef __MEMORY_DRIVER_H__
#define __MEMORY_DRIVER_H__
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "peripherials/gps_driver/gps_driver.h"

void init_memory();
bool save_measurement(float dB, gps_data_t gps);
bool write_to_storage(const char* data);

#endif // __MEMORY_DRIVER_H__  