#include "pwm_address.h"
#include "gpio_pwm.h"
#include "gpio_address.h"

void gpio_pwm_setup(uint8_t gpio_num, uint8_t signal_index) {
    /* Set GPIO output signal to PWM signal (idx = 71 from HS_CH0) */
    GPIO_FUNC_OUT_SEL_CFG_REG(gpio_num) = 71 + signal_index;

    // Enable GPIO output
    if (gpio_num < 32) {
        /*for gpio 0-31*/
        GPIO_ENABLE_W1TS_REG = (1u << gpio_num);
    } else {
        /* for gpio 32 so on*/
        GPIO_ENABLE1_REG |= (1u << (gpio_num - 32));
    }

    // Configure IO_MUX to enable output driver (FUNC = 2)
    volatile uint32_t* io_mux_reg = (volatile uint32_t*)gpio_io_mux_addr[gpio_num];
    /*clear func*/
    *io_mux_reg &= ~(0x7 << 12);
    /*set func*/
    *io_mux_reg |= (2 << 12);
}
