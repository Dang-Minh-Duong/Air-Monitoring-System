#include <stdio.h>
#include <stdint.h>
#include "pwm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Cấu hình thông số PWM
#define PWM_TIMER      0
#define PWM_CHANNEL    0
#define PWM_RES_BITS   10               
#define PWM_GPIO       18               
#define PWM_FREQ_HZ    5000             

void app_main(void) {

    pwm_init(PWM_TIMER, PWM_CHANNEL, PWM_RES_BITS, PWM_GPIO, PWM_FREQ_HZ);
    
    uint32_t duty = 0;
    uint32_t max_duty = (1 << PWM_RES_BITS);

    while (1) {
        pwm_set_duty(PWM_CHANNEL, duty);

        duty += max_duty / 10;
        if (duty > max_duty) duty = 0;

        vTaskDelay(pdMS_TO_TICKS(500)); // Chờ 500ms
    }
}
