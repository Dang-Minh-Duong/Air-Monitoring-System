#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "gpio_config.h"
#include "led_driver.h"
#include "motor_driver.h"

/* 
 * Khai báo các LED với chân GPIO và mức active (LOW hoặc HIGH)
 */
led_t led_power = {
    .pin = LED_POWER_GPIO,        /* Chân GPIO của LED nguồn */
    .active_level = LED_ACTIVE_HIGH /* LED sáng khi GPIO ở mức LOW */
};

led_t led_status = {
    .pin = LED_STATUS_GPIO,       /* Chân GPIO LED trạng thái */
    .active_level = LED_ACTIVE_LOW
};

led_t led_error = {
    .pin = LED_ERROR_GPIO,        /* Chân GPIO LED báo lỗi */
    .active_level = LED_ACTIVE_HIGH /* LED sáng khi GPIO ở mức HIGH */
};

/* 
 * Khai báo cấu hình motor với chân IN1, IN2
 * Nếu bạn muốn dùng PWM, hãy mở rộng cấu trúc motor_t trong motor_driver.h và khởi tạo thêm pwm_pin, pwm_channel...
 */
motor_t motor = {
    .in1_pin = MOTOR_IN1_GPIO,    /* Chân điều khiển motor IN1 */
    .in2_pin = MOTOR_IN2_GPIO ,    /* Chân điều khiển motor IN2 */
    .pwm_pin = GPIO_NUM_16,           // Pin PWM xuất ra (gắn LED hoặc motor ENA)
    .pwm_channel = 0,                 // LEDC channel (0–7)
    .pwm_timer = 0,                   // LEDC timer (0–3)
    .pwm_freq_hz = 5000,              // Tần số PWM (5kHz là chuẩn)
    .pwm_duty_resolution = 8          // Độ phân giải PWM (0–255)
};

/* 
 * Nhiệm vụ điều khiển LED: nháy lần lượt các LED với khoảng delay 500 ms
 */
void led_task(void *pvParameters) {
    /* Khởi tạo các LED trước khi chạy */
    led_init(&led_power);
    led_init(&led_status);
    led_init(&led_error);

    while (1) {
        led_toggle(&led_power);     /* Đảo trạng thái LED nguồn */
        vTaskDelay(pdMS_TO_TICKS(500));

        led_toggle(&led_status);    /* Đảo trạng thái LED trạng thái */
        vTaskDelay(pdMS_TO_TICKS(500));

        led_toggle(&led_error);     /* Đảo trạng thái LED lỗi */
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/* 
 * Nhiệm vụ điều khiển motor:
 * - Khởi tạo motor
 * - Chạy motor tiến trong 5 giây
 * - Dừng motor
 * - Xóa task khi kết thúc
 */
void motor_task(void *pvParameters) {
    motor_init(&motor);

    motor_forward(&motor);  /* Chạy motor tới */

    motor_set_speed(&motor, 5);  // 30.0%

    vTaskDelay(pdMS_TO_TICKS(5000));  /* Chạy 5 giây */

    motor_stop(&motor);     /* Dừng motor */

    vTaskDelete(NULL);      /* Xóa task này */
}

/* 
 * Hàm chính khởi tạo các task FreeRTOS
 */
void app_main(void) {
    /* Tạo task điều khiển LED */
    xTaskCreate(led_task, "LED Task", 2048, NULL, 1, NULL);

    /* Tạo task điều khiển motor */
    xTaskCreate(motor_task, "Motor Task", 2048, NULL, 1, NULL);
}
