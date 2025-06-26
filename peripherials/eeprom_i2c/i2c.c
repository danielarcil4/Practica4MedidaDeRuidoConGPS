#include "i2c.h"

#define SDA             0
#define SCL             1
#define I2C_HANDLE      i2c0 // Use i2c0 as the I2C handle
#define I2C_FREQUENCY 40000 // 40kHz

plataform_t my_i2c_init()
{
    i2c_init(I2C_HANDLE, I2C_FREQUENCY);
    plataform_t i2c_handler = {
        .i2c_write_blocking = my_i2c_write_blocking,
        .i2c_read_blocking = my_i2c_read_blocking,
        .i2c_handle = I2C_HANDLE
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
