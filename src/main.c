#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define PWM_GPIO        18              // Chân xuất PWM
#define PWM_FREQ_HZ     1000            // Tần số 1kHz
#define PWM_DUTY        4096            // Duty 50% (tối đa 8191 cho 13-bit)
#define PWM_RES         LEDC_TIMER_13_BIT
#define PWM_TIMER       LEDC_TIMER_0
#define PWM_MODE        LEDC_HIGH_SPEED_MODE
#define PWM_CHANNEL     LEDC_CHANNEL_0

void app_main(void) {
    // 1. Cấu hình Timer cho PWM
    ledc_timer_config_t timer_conf = {
        .duty_resolution = PWM_RES,
        .freq_hz = PWM_FREQ_HZ,
        .speed_mode = PWM_MODE,
        .timer_num = PWM_TIMER,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf);

    // 2. Cấu hình Channel để xuất PWM
    ledc_channel_config_t channel_conf = {
        .channel    = PWM_CHANNEL,
        .duty       = PWM_DUTY,
        .gpio_num   = PWM_GPIO,
        .speed_mode = PWM_MODE,
        .hpoint     = 0,
        .timer_sel  = PWM_TIMER
    };
    ledc_channel_config(&channel_conf);

    // 3. Thay đổi duty cycle runtime (nếu cần)
    while (1) {
        for (int duty = 0; duty <= 8191; duty += 512) {
            ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty);
            ledc_update_duty(PWM_MODE, PWM_CHANNEL);
            vTaskDelay(pdMS_TO_TICKS(300));
        }
    }
}
