#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pwm.h"

// Cấu hình PWM
#define PWM_TIMER      0      // High-speed timer số 0
#define PWM_CHANNEL    0      // High-speed channel số 0
#define PWM_RES_BITS   8      // Độ phân giải 8-bit
#define PWM_GPIO       18     // Chân GPIO dùng để xuất PWM
#define PWM_FREQ_HZ    1000   // Tần số PWM = 1kHz

void app_main(void) {

    pwm_init(PWM_TIMER, PWM_CHANNEL, PWM_RES_BITS, PWM_GPIO, PWM_FREQ_HZ);

    while (1) {
        // Tăng dần từ 0% đến 100%
        for (int duty = 0; duty <= 100; duty += 10) {
           
            pwm_set_duty_percent(PWM_CHANNEL, PWM_RES_BITS, duty);
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        // Giảm dần từ 100% về 0%
        for (int duty = 100; duty >= 0; duty -= 10) {
         
            pwm_set_duty_percent(PWM_CHANNEL, PWM_RES_BITS, duty);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}