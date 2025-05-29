#ifndef GPIO_ADDRESS_H
#define GPIO_ADDRESS_H

#include <stdint.h>

/**
 * @brief IO_MUX register addresses for supported GPIO pins.
 * Used to set function select and drive strength for each pin.
 */
static const uint32_t gpio_io_mux_addr[] = {
    [0]  = 0x3FF49044, [1]  = 0x3FF49088, [2]  = 0x3FF49040, [3]  = 0x3FF49084,
    [4]  = 0x3FF49048, [5]  = 0x3FF4906C, [12] = 0x3FF49034, [13] = 0x3FF49038,
    [14] = 0x3FF49030, [15] = 0x3FF4903C, [16] = 0x3FF4904C, [17] = 0x3FF49050,
    [18] = 0x3FF49070, [19] = 0x3FF49074, [21] = 0x3FF4907C, [22] = 0x3FF49080,
    [23] = 0x3FF4908C, [25] = 0x3FF49024, [26] = 0x3FF49028, [27] = 0x3FF4902C,
    [32] = 0x3FF4901C, [33] = 0x3FF49020, [34] = 0x3FF49014, [35] = 0x3FF49018,
    [36] = 0x3FF49004, [37] = 0x3FF49008, [38] = 0x3FF4900C, [39] = 0x3FF49010,
};

/**
 * @brief GPIO output signal configuration register.
 * Each GPIO can be configured to output internal signals.
 * @param n GPIO number.
 */
#define GPIO_FUNC_OUT_SEL_CFG_REG(n) (*(volatile uint32_t*)(0x3FF44530 + (n) * 0x4))

/**
 * @brief GPIO output enable write-1-to-set register (for GPIO 0–31).
 */
#define GPIO_ENABLE_W1TS_REG         (*(volatile uint32_t*)0x3FF44024)

/**
 * @brief GPIO output enable register (for GPIO 32–39).
 */
#define GPIO_ENABLE1_REG             (*(volatile uint32_t*)0x3FF4402C)

#endif // GPIO_ADDRESS_H
