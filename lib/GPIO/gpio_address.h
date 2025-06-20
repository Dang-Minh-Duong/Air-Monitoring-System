#ifndef GPIO_ADDRESS_H
#define GPIO_ADDRESS_H

#include <stdint.h>

/**
 * @brief Base addresses for GPIO and IO_MUX registers.
 */
#define GPIO_BASE          0x3FF44000
#define IO_MUX_BASE_ADDR   0x3FF49000
#define RTC_IO_BASE        0x3FF48400

/**
 * @brief GPIO output enable write-1-to-set register (for GPIO 0–31).
 */
#define GPIO_ENABLE_W1TS_REG      (*(volatile uint32_t *)(GPIO_BASE + 0x24))

/**
 * @brief GPIO output enable register (for GPIO 32–39).
 */
#define GPIO_ENABLE1_REG          (*(volatile uint32_t *)(GPIO_BASE + 0x2C))

/**
 * @brief GPIO output enable write-1-to-set register (for GPIO 32–39).
 */
#define GPIO_ENABLE1_W1TS_REG     (*(volatile uint32_t *)(GPIO_BASE + 0x30))

/**
 * @brief GPIO output signal configuration register.
 * Each GPIO can be configured to output internal signals.
 * @param n GPIO number.
 */
#define GPIO_FUNC_OUT_SEL_CFG_REG(n)   (*(volatile uint32_t *)(0x3FF44530 + (n) * 0x4))

/**
 * @brief GPIO input signal configuration register.
 * @param y input signal index.
 */
#define GPIO_FUNC_IN_SEL_CFG_REG(y)    (*(volatile uint32_t *)(GPIO_BASE + 0x0130 + ((y) * 4)))

/**
 * @brief GPIO pin register base address (for reading/writing pin states).
 * @param n GPIO number.
 */
#define GPIO_PIN_REG(n)           (GPIO_BASE + 0x0088 + ((n) * 4))

/**
 * @brief Other GPIO registers (for output set/clear, clock enable).
 */
#define GPIO_OUT_W1TS_REG         (*(volatile uint32_t *)(GPIO_BASE + 0x08))
#define GPIO_OUT_W1TC_REG         (*(volatile uint32_t *)(GPIO_BASE + 0x0C))
#define DPORT_PERIP_CLK_EN_REG    (*(volatile uint32_t *)0x3FF000C0)

/**
 * @brief IO_MUX register addresses for supported GPIO pins.
 * Used to set function select and drive strength for each pin.
 * Indexed by GPIO number.
 */
static const uint32_t gpio_io_mux_addr[] = {
    [0] = 0x3FF49044,
    [1] = 0x3FF49088,
    [2] = 0x3FF49040,
    [3] = 0x3FF49084,
    [4] = 0x3FF49048,
    [5] = 0x3FF4906C,
    [12] = 0x3FF49034,
    [13] = 0x3FF49038,
    [14] = 0x3FF49030,
    [15] = 0x3FF4903C,
    [16] = 0x3FF4904C,
    [17] = 0x3FF49050,
    [18] = 0x3FF49070,
    [19] = 0x3FF49074,
    [21] = 0x3FF4907C,
    [22] = 0x3FF49080,
    [23] = 0x3FF4908C,
    [25] = 0x3FF49024,
    [26] = 0x3FF49028,
    [27] = 0x3FF4902C,
    [32] = 0x3FF4901C,
    [33] = 0x3FF49020,
    [34] = 0x3FF49014,
    [35] = 0x3FF49018,
    [36] = 0x3FF49004,
    [37] = 0x3FF49008,
    [38] = 0x3FF4900C,
    [39] = 0x3FF49010,
};

/**
 * @brief RTC pin to GPIO mapping (based on ESP32 TRM).
 * Maps RTC ADC pins (0-17) to GPIO numbers.
 */
static const uint8_t rtc_pin_to_gpio[18] = {
    36, 37, 38, 39, 34, 35, 25, 26,
    33, 32, 4, 0, 2, 15, 13, 12, 14, 27
};

/**
 * @brief RTC IO register pointers.
 * RTCIO_RTC_GPIO_PINx_REG (pin 0 to 17).
 * Used for configuring RTC GPIO registers.
 */
static volatile uint32_t *const rtc_io_reg[18] = {
    (volatile uint32_t *)(RTC_IO_BASE + 0x28), (volatile uint32_t *)(RTC_IO_BASE + 0x2C),
    (volatile uint32_t *)(RTC_IO_BASE + 0x30), (volatile uint32_t *)(RTC_IO_BASE + 0x34),
    (volatile uint32_t *)(RTC_IO_BASE + 0x38), (volatile uint32_t *)(RTC_IO_BASE + 0x3C),
    (volatile uint32_t *)(RTC_IO_BASE + 0x40), (volatile uint32_t *)(RTC_IO_BASE + 0x44),
    (volatile uint32_t *)(RTC_IO_BASE + 0x48), (volatile uint32_t *)(RTC_IO_BASE + 0x4C),
    (volatile uint32_t *)(RTC_IO_BASE + 0x50), (volatile uint32_t *)(RTC_IO_BASE + 0x54),
    (volatile uint32_t *)(RTC_IO_BASE + 0x58), (volatile uint32_t *)(RTC_IO_BASE + 0x5C),
    (volatile uint32_t *)(RTC_IO_BASE + 0x60), (volatile uint32_t *)(RTC_IO_BASE + 0x64),
    (volatile uint32_t *)(RTC_IO_BASE + 0x68), (volatile uint32_t *)(RTC_IO_BASE + 0x6C)
};

#endif
