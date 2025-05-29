#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "driver/gpio.h"
#include "driver/ledc.h"  /* Thư viện PWM LEDC của ESP32 */

/* 
 * Cấu trúc định nghĩa motor với các chân điều khiển và thông số PWM
 */
typedef struct {
    gpio_num_t in1_pin;           /* Chân GPIO điều khiển IN1 (chiều quay motor) */
    gpio_num_t in2_pin;           /* Chân GPIO điều khiển IN2 (chiều quay ngược) */
    gpio_num_t pwm_pin;           /* Chân GPIO xuất xung PWM điều khiển tốc độ motor */
    ledc_channel_t pwm_channel;   /* Kênh PWM (0 - 7) */
    ledc_timer_t pwm_timer;       /* Timer PWM (0 - 3) */
    uint32_t pwm_freq_hz;         /* Tần số PWM (Hz), ví dụ 20000 Hz) */
    uint8_t pwm_duty_resolution; /* Độ phân giải duty cycle (bit), ví dụ LEDC_TIMER_8_BIT */
} motor_t;

/* 
 * Hàm khởi tạo motor:
 * - Cấu hình các chân GPIO IN1, IN2 làm output
 * - Cấu hình chân PWM làm output PWM với các tham số cấu hình đã cho
 */
void motor_init(const motor_t *motor);

/* 
 * Hàm điều khiển motor chạy tiến (IN1 = HIGH, IN2 = LOW)
 */
void motor_forward(const motor_t *motor);

/* 
 * Hàm điều khiển motor chạy lùi (IN1 = LOW, IN2 = HIGH)
 */
void motor_backward(const motor_t *motor);

/* 
 * Hàm dừng motor (IN1 = LOW, IN2 = LOW)
 * Đồng thời tắt PWM (tốc độ 0)
 */
void motor_stop(const motor_t *motor);

/* 
 * Hàm điều chỉnh tốc độ motor bằng PWM
 * @param duty_percent: giá trị phần trăm độ rộng xung PWM từ 0 đến 100
 */
void motor_set_speed(const motor_t *motor, uint32_t duty_percent);

#endif /* MOTOR_DRIVER_H */
