#include "pwm_address.h"
#include "gpio.h"
#include "gpio_address.h"
#include "i2c_address.h"
#include "ADC.h"
#include "gpio_config.h"

/**
 * @brief Configure a GPIO pin to output a PWM signal.
 * 
 * This function sets the GPIO function output select register to map
 * the GPIO to a PWM signal based on the given channel index.
 * It also enables the GPIO output and configures the IO_MUX for output.
 * 
 * @param gpio_num The GPIO number to configure.
 * @param channel_index The PWM channel index offset for signal mapping.
 */
void gpio_pwm_setup(uint8_t gpio_num, uint8_t channel_index)
{
    /* Set GPIO output signal to PWM signal (idx = 71 from HS_CH0 + channel offset) */
    GPIO_FUNC_OUT_SEL_CFG_REG(gpio_num) = 71 + channel_index;

    /* Enable GPIO output */
    if (gpio_num < 32)
    {
        /* For GPIO 0-31, set bit in GPIO_ENABLE_W1TS_REG to enable output */
        GPIO_ENABLE_W1TS_REG = (1u << gpio_num);
    }
    else
    {
        /* For GPIO 32 and above, set bit in GPIO_ENABLE1_REG */
        GPIO_ENABLE1_REG |= (1u << (gpio_num - 32));
    }

    /* Configure IO_MUX to enable output driver and select function 2 (GPIO output) */
    volatile uint32_t *io_mux_reg = (volatile uint32_t *)gpio_io_mux_addr[gpio_num];

    /* Clear FUNC bits [14:12] */
    *io_mux_reg &= ~(0x7 << 12);

    /* Set FUNC bits to 2 to select GPIO output function */
    *io_mux_reg |= (2 << 12);
}

/**
 * @brief Configure a given RTC IO pin for analog input through RTC ADC path.
 * 
 * This configures the RTC IO register and IO_MUX to disable output driver,
 * enable input, disable pull-up and pull-down resistors, and select function 0 (RTC analog).
 * 
 * @param adc The ADC unit (not used in this function, but for completeness).
 * @param rtc_pin The RTC pin number to configure.
 */
void gpio_adc_setup(adc_unit_t adc, int rtc_pin)
{
    /* Validate RTC pin number */
    if (rtc_pin < 0 || rtc_pin >= 40)
        return;

    uint8_t gpio = rtc_pin_to_gpio[rtc_pin];
    volatile uint32_t *rtc_reg = rtc_io_reg[rtc_pin];
    volatile uint32_t *mux_reg = (volatile uint32_t *)gpio_io_mux_addr[gpio];
    if (!mux_reg)
        return;

    /* Step 1: Configure RTC IO register */
    uint32_t val = *rtc_reg;

    /* Clear PAD_DRIVER bit (bit 2) to 0 for normal output (required for analog input) */
    val &= ~(1 << 2);
    *rtc_reg = val;

    /* Step 2: Configure IO_MUX register */
    val = *mux_reg;

    /* Clear MCU_SEL bits [14:12] */
    val &= ~(7 << 12);

    /* Set MCU_SEL to 0 to select function 0 (RTC analog function) */
    val |= (0 << 12);

    /* Enable input (FUN_IE = bit 9) */
    val |= (1 << 9);

    /* Disable pull-up (FUN_WPU = bit 8) */
    val &= ~(1 << 8);

    /* Disable pull-down (FUN_WPD = bit 7) */
    val &= ~(1 << 7);

    *mux_reg = val;
}

/**
 * @brief Configure a GPIO pin as a push-pull output for LED control.
 * 
 * This sets the IO_MUX register to function 2 (GPIO),
 * disables pull-up and pull-down resistors, and enables the GPIO output driver.
 * 
 * @param gpio_num The GPIO number to configure.
 */
void gpio_led_setup(int gpio_num)
{
    /* Validate GPIO number - only configure for valid pins */
    if (gpio_num < 0 || gpio_num >= 19)
        return;

    volatile uint32_t *mux_reg = (volatile uint32_t *)gpio_io_mux_addr[gpio_num];
    if (!mux_reg)
        return;

    /* Read current IO_MUX register */
    uint32_t val = *mux_reg;

    /* Clear MCU_SEL bits [14:12] */
    val &= ~(7 << 12);

    /* Set MCU_SEL to 2 to select GPIO function */
    val |= (2 << 12);

    /* Disable pull-up and pull-down resistors */
    val &= ~(1 << 8);  // FUN_WPU = 0
    val &= ~(1 << 7);  // FUN_WPD = 0

    /* Write back IO_MUX configuration */
    *mux_reg = val;

    /* Enable output driver */
    if (gpio_num < 32)
        GPIO_ENABLE_W1TS_REG = (1 << gpio_num);
    else
        GPIO_ENABLE1_W1TS_REG = (1 << (gpio_num - 32));
}

/**
 * @brief Configure two GPIO pins for I2C communication (SDA and SCL).
 * 
 * This function sets the GPIO function select registers to I2C,
 * configures input selects, enables GPIO outputs, and configures IO_MUX registers
 * for open-drain I2C signals.
 * 
 * @param sda GPIO number for I2C SDA line.
 * @param scl GPIO number for I2C SCL line.
 */
void gpio_i2c_setup(int sda, int scl)
{
    /* Set output function select registers for SCL and SDA */
    REG_WRITE(GPIO_FUNC_OUT_SEL_CFG_REG(scl), I2CEXT0_SCL_OUT_IDX);
    REG_WRITE(GPIO_FUNC_OUT_SEL_CFG_REG(sda), I2CEXT0_SDA_OUT_IDX);

    /* Configure input function select registers with input enabled (bit 7 = 1) */
    REG_WRITE(GPIO_FUNC_IN_SEL_CFG_REG(I2CEXT0_SCL_IN_IDX), (scl) | (1 << 7));
    REG_WRITE(GPIO_FUNC_IN_SEL_CFG_REG(I2CEXT0_SDA_IN_IDX), (sda) | (1 << 7));

    /* Enable GPIO output driver for SCL and SDA */
    REG_WRITE(GPIO_ENABLE_W1TS_REG, BIT(scl));
    REG_WRITE(GPIO_ENABLE_W1TS_REG, BIT(sda));

    /* Configure GPIO PIN registers:
       Set bit 2 (OE override) for output enable */
    REG_WRITE(GPIO_PIN_REG(scl), BIT(2));
    REG_WRITE(GPIO_PIN_REG(sda), BIT(2));

    /* Configure IO_MUX registers:
       Set input enable (bit 9), pull-up enable (bit 8),
       and select function 2 (GPIO) */
    REG_WRITE(gpio_io_mux_addr[scl], (BIT(9)) | (BIT(8)) | (2 << 12));
    REG_WRITE(gpio_io_mux_addr[sda], (BIT(9)) | (BIT(8)) | (2 << 12));
}
