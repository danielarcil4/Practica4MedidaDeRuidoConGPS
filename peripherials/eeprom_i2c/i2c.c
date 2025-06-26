#include "eeprom_plataform.h"

plataform_t my_i2c_init(i2c_inst_t *i2c_handle, uint32_t frequency)
{
    i2c_init(i2c_handle, frequency);
    plataform_t i2c_handler = {
        .i2c_write_blocking = my_i2c_write_blocking,
        .i2c_read_blocking = my_i2c_read_blocking,
        .i2c_handle = i2c_handle
    };

    return i2c_handler;
}

int my_i2c_write_blocking(void *i2c_handle, uint8_t addr, const uint8_t *src, size_t len)
{
	return i2c_write_blocking((i2c_inst_t *)i2c_handle, addr, src, len, false);
}

int my_i2c_read_blocking(void *i2c_handle, uint8_t addr, uint8_t *dst, size_t len)
{
	return i2c_read_blocking((i2c_inst_t *)i2c_handle, addr, dst, len, false);
}
