#include "memory_driver.h"

void init_memory() {
    // Inicializar SD o Flash externa
}

bool save_measurement(float dB, gps_data_t gps) {
    char buffer[128];
    sprintf(buffer, "Lat:%.6f,Lng:%.6f,dB:%.2f\n", gps.latitude, gps.longitude, dB);
    return write_to_storage(buffer);
}

bool write_to_storage(const char* data) {
    // Implementación específica (ej: FATFS)
    return true;
}
