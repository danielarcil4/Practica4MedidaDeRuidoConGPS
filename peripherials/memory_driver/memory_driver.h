#ifndef __MEMORY_DRIVER_H__
#define __MEMORY_DRIVER_H__

#include "hardware/i2c.h"
#include <stdint.h>
#include <stdbool.h>

#define EEPROM_PAGE_SIZE 16
#define EEPROM_BLOCK_SIZE 256
#define EEPROM_TOTAL_SIZE 2048

// Base I2C address for block 0
#define SLAVE_ADD 0x0A


#define OFFSET_DATOS 4
#define BLOQUE_NOISE 0x01
#define BLOQUE_LAT 0x02
#define BLOQUE_LON 0x03
#define MAX_CAPTURAS 63  // índice 0 reservado
#define DIR_INDEX 0XFFF1
#define DIR_BASE 0xFF


void eeprom_init();
void save_measurement(char * datos);

bool eeprom_write_byte(uint16_t mem_addr, uint8_t data);
bool eeprom_write_page(uint16_t mem_addr, const uint8_t *data, uint8_t len);

bool eeprom_read_byte(uint16_t mem_addr, uint8_t *data);
bool eeprom_read_data(uint16_t mem_addr, uint8_t *buffer, uint8_t len);

bool eeprom_read_measurement(uint16_t mem_addr, float *noise, float *lat, float *lon);
bool eeprom_write_measurement(uint16_t mem_addr, float noise, float lat, float lon);

void eeprom_set_index(uint8_t index);
bool eeprom_get_index(uint8_t *data);
/*
uint8_t eeprom_get_index();
void eeprom_set_index(uint8_t index);
void eeprom_guardar_captura(float v1, float v2, float v3);
void eeprom_ver_datos();
void eeprom_flush();
*/

#endif // __MEMORY_DRIVER_H__  