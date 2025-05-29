#include "pin.h"

#define RTC_IO_BASE 0x3FF48400
#define IO_MUX_BASE 0x3FF49000
#define GPIO_BASE 0x3FF44000

#define GPIO_ENABLE_W1TS_REG (*(volatile uint32_t *)(GPIO_BASE + 0x24))
#define GPIO_ENABLE1_W1TS_REG (*(volatile uint32_t *)(GPIO_BASE + 0x30))

/* RTCIO_RTC_GPIO_PINx_REG (pin 0 to 17) */
static volatile uint32_t *const rtc_io_reg[18] = {
    (volatile uint32_t *)(RTC_IO_BASE + 0x28), (volatile uint32_t *)(RTC_IO_BASE + 0x2C),
    (volatile uint32_t *)(RTC_IO_BASE + 0x30), (volatile uint32_t *)(RTC_IO_BASE + 0x34),
    (volatile uint32_t *)(RTC_IO_BASE + 0x38), (volatile uint32_t *)(RTC_IO_BASE + 0x3C),
    (volatile uint32_t *)(RTC_IO_BASE + 0x40), (volatile uint32_t *)(RTC_IO_BASE + 0x44),
    (volatile uint32_t *)(RTC_IO_BASE + 0x48), (volatile uint32_t *)(RTC_IO_BASE + 0x4C),
    (volatile uint32_t *)(RTC_IO_BASE + 0x50), (volatile uint32_t *)(RTC_IO_BASE + 0x54),
    (volatile uint32_t *)(RTC_IO_BASE + 0x58), (volatile uint32_t *)(RTC_IO_BASE + 0x5C),
    (volatile uint32_t *)(RTC_IO_BASE + 0x60), (volatile uint32_t *)(RTC_IO_BASE + 0x64),
    (volatile uint32_t *)(RTC_IO_BASE + 0x68), (volatile uint32_t *)(RTC_IO_BASE + 0x6C)};

/* IO_MUX register map */
static volatile uint32_t *const io_mux_gpio_reg[40] = {
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

/* RTC pin to GPIO mapping (based on ESP32 TRM) */
static const uint8_t rtc_pin_to_gpio[18] = {
    36, 37, 38, 39, 34, 35, 25, 26,
    33, 32, 4, 0, 2, 15, 13, 12, 14, 27};

/**
 * Configure a given RTC IO pin for analog input via RTC ADC path.
 */
void adc_configure_pin(adc_unit_t adc, int rtc_pin)
{
    if (rtc_pin < 0 || rtc_pin >= 18)
        return;

    uint8_t gpio = rtc_pin_to_gpio[rtc_pin];
    volatile uint32_t *rtc_reg = rtc_io_reg[rtc_pin];
    volatile uint32_t *mux_reg = io_mux_gpio_reg[gpio];
    if (!mux_reg)
        return;

    /*RTC IO register */
    uint32_t val = *rtc_reg;
    val &= ~(1 << 2); // PAD_DRIVER = 0 (normal output — required for analog)
    *rtc_reg = val;

    /* Step 2: configure IO_MUX */
    val = *mux_reg;
    val &= ~(7 << 12); // Clear MCU_SEL (bits [14:12])
    val |= (0 << 12);  // Select Function 0 (RTC analog)

    val |= (1 << 9);  // FUN_IE = 1: input enable
    val &= ~(1 << 8); // FUN_WPU = 0: disable pull-up
    val &= ~(1 << 7); // FUN_WPD = 0: disable pull-down

    *mux_reg = val;
}

/**
 * Configure a GPIO pin for output (push-pull).
 */
void gpio_output_configure_pin(int gpio_num)
{
    if (gpio_num < 0 || gpio_num > 39)
        return;

    volatile uint32_t *mux_reg = io_mux_gpio_reg[gpio_num];
    if (!mux_reg)
        return;

    uint32_t val = *mux_reg;

    /* Step 1: Configure IO_MUX register */
    val &= ~(7 << 12); // Clear MCU_SEL bits [14:12]
    val |= (2 << 12);  // MCU_SEL = 2 (GPIO function)
    val &= ~(1 << 8);  // FUN_WPU = 0 (disable pull-up)
    val &= ~(1 << 7);  // FUN_WPD = 0 (disable pull-down)
    // Optional: set FUN_DRV here if needed (bit [11:10])

    *mux_reg = val;

    /* Step 2: Enable output driver via GPIO_ENABLE_W1TS */
    if (gpio_num < 32)
        GPIO_ENABLE_W1TS_REG = (1 << gpio_num);
    else
        GPIO_ENABLE1_W1TS_REG = (1 << (gpio_num - 32));
}
