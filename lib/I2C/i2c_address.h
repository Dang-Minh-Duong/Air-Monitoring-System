#ifndef I2C_ADDRESS_H
#define I2C_ADDRESS_H

#include <stdint.h>

/* 
 * Base address of the I2C0 peripheral registers on the ESP32 chip.
 * All register addresses are calculated as offsets from this base.
 */
#define I2C_BASE_ADDR        0x3FF53000

/*
 * I2C clock timing registers:
 * Configure the low and high period of the SCL clock line.
 * These settings affect the I2C bus speed.
 */
#define I2C_SCL_LOW_PERIOD   (I2C_BASE_ADDR + 0x00)   /* SCL low period configuration register */
#define I2C_SCL_HIGH_PERIOD  (I2C_BASE_ADDR + 0x38)   /* SCL high period configuration register */

/*
 * I2C FIFO and command registers:
 * - FIFO register is the data buffer for read/write.
 * - CMD registers hold I2C command sequences for transactions.
 */
#define I2C_FIFO_REG         (I2C_BASE_ADDR + 0x100)  /* FIFO data register */
#define I2C_CMD_REG(n)       (I2C_BASE_ADDR + 0x58 + ((n) * 4))  /* Command register at index n */

/*
 * Control and status registers:
 * - CTRL_REG: Control register for enabling/disabling I2C features.
 * - FIFO_CONF_REG: FIFO configuration register.
 * - INT_RAW_REG: Raw interrupt status register.
 * - INT_CLR_REG: Interrupt clear register.
 * - STATUS_REG: Current status register of I2C peripheral.
 */
#define I2C_CTRL_REG         (I2C_BASE_ADDR + 0x04)   /* Control register */
#define I2C_FIFO_CONF_REG    (I2C_BASE_ADDR + 0x18)   /* FIFO configuration register */
#define I2C_INT_RAW_REG      (I2C_BASE_ADDR + 0x20)   /* Raw interrupt status register */
#define I2C_INT_CLR_REG      (I2C_BASE_ADDR + 0x24)   /* Interrupt clear register */
#define I2C_STATUS_REG       (I2C_BASE_ADDR + 0x2C)   /* Status register */

/*
 * Default I2C slave device address.
 * This 7-bit address is used for addressing the slave device.
 */
#define I2C_ADDR             0x27

/*
 * Write bit for I2C transactions.
 * Used to indicate a write operation (0).
 */
#define WRITE_BIT            0x0

/*
 * Macro to calculate bit mask by shifting 1 left by x positions.
 * Useful for setting or clearing specific bits in registers.
 */
#define BIT(x)               (1U << (x))

/*
 * Macros for volatile memory access.
 * Used to read from and write to hardware registers.
 */
#define REG_WRITE(addr, val) (*((volatile unsigned int *)(addr)) = (val))
#define REG_READ(addr)       (*((volatile unsigned int *)(addr)))

/*
 * GPIO matrix signal indexes for routing I2C signals on the ESP32.
 * These constants correspond to internal signal numbers for SDA and SCL lines.
 * Reference: ESP32 datasheet, Table 4-2, page 11.
 */
#define I2CEXT0_SCL_OUT_IDX       29  
#define I2CEXT0_SDA_OUT_IDX       30  
#define I2CEXT0_SCL_IN_IDX        29  
#define I2CEXT0_SDA_IN_IDX        30  

#endif /* I2C_ADDRESS_H */
