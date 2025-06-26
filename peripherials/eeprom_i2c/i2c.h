#ifndef EEPROM_PLATFORM_H
#define EEPROM_PLATFORM_H
#include <stdint.h>

#include "hardware/i2c.h"

typedef struct {
    int (*i2c_write_blocking)(void *i2c_handle, uint8_t addr, const uint8_t *src, size_t len);
	int (*i2c_read_blocking)(void *i2c_handle, uint8_t addr, uint8_t *src, size_t len);
	void *i2c_handle;
}plataform_t;

plataform_t my_i2c_init(void);

int my_i2c_write_blocking(void *i2c_handle, uint8_t addr, const uint8_t *src, size_t len);

int my_i2c_read_blocking(void *i2c_handle, uint8_t addr, uint8_t *dst, size_t len);

#endif // EEPROM_PLATFORM_H