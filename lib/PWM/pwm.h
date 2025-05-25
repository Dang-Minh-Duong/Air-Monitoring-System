/* pwm.h – Khai báo hàm PWM (LEDC high-speed, khai báo hoàn toàn bằng thanh ghi) */
#ifndef PWM_H
#define PWM_H

#include <stdint.h>

/* Hàm khởi tạo PWM:
 * timer_num: số timer (0–3)
 * channel: số kênh (0–7)
 * freq_hz: tần số PWM (Hz)
 * duty_res: độ phân giải duty (số bit)
 * gpio_num: chân xuất PWM
 */
void pwm_init(uint8_t timer_num, uint8_t channel, uint32_t freq_hz, uint8_t duty_res, uint8_t gpio_num);

/* Hàm cập nhật duty cycle (% 0–100) cho kênh đã init */
void pwm_set_duty_cycle(uint8_t channel, uint32_t duty_percent);

#endif // PWM_H
