#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pwm.h"
#include "gpio.h"
#include "motor_control.h"
#include "led.h"
// Cấu hình PWM
#define PWM_TIMER 0      // High-speed timer số 0
#define PWM_CHANNEL 0    // High-speed channel số 0
#define PWM_RES_BITS 8   // Độ phân giải 8-bit
#define PWM_GPIO 18      // Chân GPIO dùng để xuất PWM
#define PWM_FREQ_HZ 1000 // Tần số PWM = 1kHz

void app_main(void)
{

    pwm_init(PWM_TIMER, PWM_CHANNEL, PWM_RES_BITS, PWM_GPIO, PWM_FREQ_HZ);
    adc_configure_pin(ADC_UNIT_2, 14);
    adc_configure(ADC_UNIT_2, 4, ADC_WIDTH_12Bit, ADC_ATTEN_11DB, 10);
    gpio_output_configure_pin(21);
    gpio_output_configure_pin(22);
    gpio_output_configure_pin(23);

    while (1)
    {
        uint16_t adcValue = adc_read(ADC_UNIT_2);
        printf("ADC = %u\n", adcValue);
        ledControl(21, 22, 23, adcValue, 200, 400);
        motor_control(adcValue, 200, 400, PWM_CHANNEL, PWM_RES_BITS);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}