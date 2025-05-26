#include "pin.h"

#define RTC_IO_BASE 0x3FF48400

/* RTCIO_RTC_GPIO_PINx_REG (pin 0 to 17) */
static volatile uint32_t *const rtc_io_reg[18] = {
    (volatile uint32_t *)(RTC_IO_BASE + 0x28), /* PIN0 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x2C), /* PIN1 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x30), /* PIN2 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x34), /* PIN3 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x38), /* PIN4 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x3C), /* PIN5 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x40), /* PIN6 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x44), /* PIN7 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x48), /* PIN8 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x4C), /* PIN9 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x50), /* PIN10 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x54), /* PIN11 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x58), /* PIN12 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x5C), /* PIN13 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x60), /* PIN14 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x64), /* PIN15 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x68), /* PIN16 */
    (volatile uint32_t *)(RTC_IO_BASE + 0x6C)  /* PIN17 */
};

/**
 * Configure a given RTC IO pin as analog input for ADC.
 */
void adc_configure_pin(adc_unit_t adc, int rtc_pin)
{
    /* check valid */
    if (rtc_pin < 0 || rtc_pin >= 18)
        return;

    volatile uint32_t *reg = rtc_io_reg[rtc_pin];
    uint32_t val = *reg;

    val &= ~(1 << 7);  /* Disable pull-up */
    val &= ~(1 << 6);  /* Disable pull-down */
    val |= (1 << 13);  /* Enable input (FUN_IE = 1) */
    val &= ~(3 << 12); /* FUN_SEL = 0 (analog RTC function) */
    val &= ~(1 << 27); /* Disable hold */

    *reg = val;
}
