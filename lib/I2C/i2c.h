#ifndef I2C_H
#define I2C_H

#include <stdint.h>

/**
 * @brief Initialize the I2C peripheral with specified SDA and SCL pins.
 * 
 * @param sda GPIO number for SDA line.
 * @param scl GPIO number for SCL line.
 */
void i2c_init(int sda, int scl);

/**
 * @brief Write a sequence of bytes to the I2C slave device.
 * 
 * @param data Pointer to the data buffer to send.
 * @param length Number of bytes to send.
 */
void i2c_master_write_byte_new(uint8_t *data, uint8_t length);

#endif /* I2C_H */
