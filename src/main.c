#include <stdio.h>
#include "pwm.h"
#include "gpio.h"
#include "motor_control.h"
#include "led.h"
#include "timer.h"
#include "wifi.h"
#include "blynk.h"
/*PWM config*/
#define PWM_TIMER 0      /*High-speed timer 0*/
#define PWM_CHANNEL 0    /*High-speed channel 0*/
#define PWM_RES_BITS 8   /*Resoluton 8 bits*/
#define PWM_GPIO 18      /*GPIO for pwm*/
#define PWM_FREQ_HZ 1000 /*PWM freqency = 1kHz*/

/*Blynk constants*/
#define TAG "BLYNK"
#define BLYNK_AUTH_TOKEN "db9XgYhqT0PLeu2xoq0VV7v0Z3asoG6X"
#define BLYNK_VIRTUAL_PIN_GAS_LEVEL "V0"
#define BLYNK_VIRTUAL_PIN_SPEED "V1"
#define BLYNK_VIRTUAL_PIN_ENABLE_SET_SPEED "V2"

void app_main(void)
{
    pwm_init(PWM_TIMER, PWM_CHANNEL, PWM_RES_BITS, PWM_GPIO, PWM_FREQ_HZ);
    Timer_Init(0, 1, 80, 1000);
    gpio_adc_setup(ADC_UNIT_1, 35);
    adc_configure(ADC_UNIT_1, 7, ADC_WIDTH_12Bit, ADC_ATTEN_11DB, 10);
    gpio_led_setup(21);
    gpio_led_setup(22);
    gpio_led_setup(23);
    wifi_init_sta("MinhDuong", "minhduong08");
    Timer_Delay(0, 1, 5000);

    while (1)
    {
        uint16_t adcValue = adc_read(ADC_UNIT_1);
        ledControl(21, 22, 23, adcValue, 500, 600);
        blynk_send_int(adcValue, BLYNK_AUTH_TOKEN, BLYNK_VIRTUAL_PIN_GAS_LEVEL, TAG);
        blynk_send_int(123, BLYNK_AUTH_TOKEN, BLYNK_VIRTUAL_PIN_SPEED, TAG);
        Timer_Delay(0, 1, 500);
    }
}
