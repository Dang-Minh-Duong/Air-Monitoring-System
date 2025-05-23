#ifndef PWM_H
#define PWM_H

#include <stdint.h>

// Cấu hình PWM
typedef struct {
    uint8_t timer_num;     // 0..3
    uint8_t channel_num;   // 0..7
    uint8_t gpio_num;      // GPIO chân xuất PWM
    uint32_t freq_hz;      // Tần số PWM mong muốn
    uint8_t bit_num;       // Độ phân giải bit PWM (max 15)
    uint8_t duty_percent;  // Chu kỳ xung, từ 0 đến 100%
} pwm_config_t;

// Khai báo hàm khởi tạo PWM
void pwm_init(const pwm_config_t *cfg);

#endif // PWM_H
