#include "pwm_address.h"
#include "gpio.h"
#include "gpio_address.h"


void gpio_pwm_setup(uint8_t gpio_num, uint8_t signal_index)
{
    /* Set GPIO output signal to PWM signal (idx = 71 from HS_CH0) */
    GPIO_FUNC_OUT_SEL_CFG_REG(gpio_num) = 71 + signal_index;

    // Enable GPIO output
    if (gpio_num < 32)
    {
        /*for gpio 0-31*/
        GPIO_ENABLE_W1TS_REG = (1u << gpio_num);
    }
    else
    {
        /* for gpio 32 so on*/
        GPIO_ENABLE1_REG |= (1u << (gpio_num - 32));
    }

    // Configure IO_MUX to enable output driver (FUNC = 2)
    volatile uint32_t *io_mux_reg = (volatile uint32_t *)gpio_io_mux_addr[gpio_num];
    /*clear func*/
    *io_mux_reg &= ~(0x7 << 12);
    /*set func*/
    *io_mux_reg |= (2 << 12);
}
/**
 * Configure a given RTC IO pin for analog input via RTC ADC path.
 */
void adc_configure_pin(adc_unit_t adc, int rtc_pin)
{
    if (rtc_pin < 0 || rtc_pin >= 18)
        return;

    uint8_t gpio = rtc_pin_to_gpio[rtc_pin];
    volatile uint32_t *rtc_reg = rtc_io_reg[rtc_pin];
    volatile uint32_t *mux_reg = (volatile uint32_t *)gpio_io_mux_addr[gpio];
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

    volatile uint32_t *mux_reg = (volatile uint32_t *)gpio_io_mux_addr[gpio_num];
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
