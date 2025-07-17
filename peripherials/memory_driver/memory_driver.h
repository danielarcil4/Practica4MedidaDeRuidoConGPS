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

void eeprom_init();

bool eeprom_write_byte(uint16_t mem_addr, uint8_t data);
bool eeprom_write_page(uint16_t mem_addr, const uint8_t *data, uint8_t len);

bool eeprom_read_byte(uint16_t mem_addr, uint8_t *data);
bool eeprom_read_data(uint16_t mem_addr, uint8_t *buffer, uint8_t len);

#endif // __MEMORY_DRIVER_H__  