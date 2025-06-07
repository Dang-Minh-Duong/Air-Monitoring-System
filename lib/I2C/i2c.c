#include "i2c_address.h"
#include "gpio_address.h"
#include "timer.h"
#include "gpio_config.h"
#include "i2c.h"

/**
 * @brief Initialize I2C peripheral with given SDA and SCL GPIO pins.
 * 
 * Enables I2C peripheral clock, configures clock timing, FIFO, interrupts,
 * and sets up the GPIO pins for I2C function.
 * 
 * @param sda GPIO number for I2C SDA line.
 * @param scl GPIO number for I2C SCL line.
 */
void i2c_init(int sda, int scl) {
    /* Enable I2C peripheral clock (assuming bit 7 enables I2C) */
    REG_WRITE(DPORT_PERIP_CLK_EN_REG, BIT(7));

    /* Configure I2C clock timing: set SCL low and high periods */
    REG_WRITE(I2C_SCL_LOW_PERIOD, 40);
    REG_WRITE(I2C_SCL_HIGH_PERIOD, 40);

    /* Configure FIFO: set some control bit (bit 10) */
    REG_WRITE(I2C_FIFO_CONF_REG, BIT(10));

    /* Clear all I2C interrupts */
    REG_WRITE(I2C_INT_CLR_REG, 0xFFFFFFFF);

    /* Enable I2C controller, setting bit 4 (enable master?) */
    REG_WRITE(I2C_CTRL_REG, BIT(4));

    /* Configure GPIO pins for I2C SDA and SCL signals */
    gpio_i2c_setup(sda, scl);
}

/**
 * @brief Send a byte array via I2C master write operation.
 * 
 * This function writes the I2C slave address with write bit,
 * writes the data bytes into FIFO, configures I2C command sequence
 * for start, write, stop, and triggers the transfer.
 * 
 * It then waits until the transfer is complete.
 * 
 * @param data Pointer to data buffer to send.
 * @param length Number of bytes to send.
 */
void i2c_master_write_byte_new(uint8_t *data, uint8_t length) {
    /* Clear all I2C interrupts */
    REG_WRITE(I2C_INT_CLR_REG, 0xFFFFFFFF);

    /* Write slave address + write bit (0) to FIFO */
    REG_WRITE(I2C_FIFO_REG, (I2C_ADDR << 1) | WRITE_BIT);

    /* Write data bytes to FIFO */
    for (uint8_t i = 0; i < length; i++) {
        REG_WRITE(I2C_FIFO_REG, data[i]);
    }

    /* Configure I2C command sequence */
    REG_WRITE(I2C_CMD_REG(0), (0x0 << 11));        // Start condition (OPCODE=0)

    REG_WRITE(I2C_CMD_REG(1),
        (0x1 << 11)   |    // OPCODE=1: WRITE command
        (1 << 10)     |    // ACK_CHECK_EN: Enable ACK check after each byte
        (0 << 8)      |    // ACK_EXP: Expect ACK=0 (ACK received)
        (0 << 7)      |    // ACK_VAL: No forced ACK/NAK
        (length & 0xFF)    // Number of bytes to write
    );

    REG_WRITE(I2C_CMD_REG(2), (0x3 << 11));        // Stop condition (OPCODE=3)

    /* Enable I2C master transmission */
    REG_WRITE(I2C_CTRL_REG, BIT(5));

    /* Wait for transmission to complete */
    while (REG_READ(I2C_STATUS_REG) & BIT(9)) {
        Timer_Delay(0, 1, 1000); /* Delay 1ms to avoid busy waiting */
    }
}
