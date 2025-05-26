#include "adc_driver.h"
#include "driver/adc.h"
#include "gpio_config.h"  // dùng pinMap.adcGas

static adc1_channel_t gpio_to_adc1_channel(gpio_num_t gpio) {
    switch (gpio) {
        case GPIO_NUM_36: return ADC1_CHANNEL_0;
        case GPIO_NUM_37: return ADC1_CHANNEL_1;
        case GPIO_NUM_38: return ADC1_CHANNEL_2;
        case GPIO_NUM_39: return ADC1_CHANNEL_3;
        case GPIO_NUM_32: return ADC1_CHANNEL_4;
        case GPIO_NUM_33: return ADC1_CHANNEL_5;
        case GPIO_NUM_34: return ADC1_CHANNEL_6;
        case GPIO_NUM_35: return ADC1_CHANNEL_7;
        default: return -1;
    }
}

void adc_init(void) {
    adc1_channel_t ch = gpio_to_adc1_channel(pinMap.adcGas);
    if (ch != -1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(ch, ADC_ATTEN_DB_11);  // 0–3.3V
    }
}

int adc_read_gas(void) {
    adc1_channel_t ch = gpio_to_adc1_channel(pinMap.adcGas);
    if (ch != -1) {
        return adc1_get_raw(ch);  // 0 - 4095
    }
    return -1;
}
