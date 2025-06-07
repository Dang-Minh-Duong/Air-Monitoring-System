#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void i2c_init(void);
void i2c_master_write_byte_new(uint8_t *data, uint8_t length);

#endif // I2C_H
