#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#include "driver/gpio.h"

/* 
 * Định nghĩa các chân GPIO dùng cho LED
 */
#define LED_POWER_GPIO      GPIO_NUM_2    /* LED nguồn */
#define LED_STATUS_GPIO     GPIO_NUM_4    /* LED trạng thái */
#define LED_ERROR_GPIO      GPIO_NUM_5    /* LED báo lỗi */

/* 
 * Định nghĩa các chân GPIO dùng cho motor
 */
#define MOTOR_IN1_GPIO      GPIO_NUM_18   /* Chân IN1 motor */
#define MOTOR_IN2_GPIO      GPIO_NUM_19   /* Chân IN2 motor */

/* 
 * Chân GPIO dùng để xuất xung PWM điều khiển tốc độ motor
 */
#define MOTOR_PWM_GPIO      GPIO_NUM_16   /* Chân PWM motor */

/* 
 * Hàm cấu hình chân GPIO cho LED làm output
 */
void gpio_config_led_output(gpio_num_t pin);

/* 
 * Hàm cấu hình chân GPIO cho motor làm output
 */
void gpio_config_motor_output(gpio_num_t pin);

#endif /* GPIO_CONFIG_H */
