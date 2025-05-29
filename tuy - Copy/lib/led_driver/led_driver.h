#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include "driver/gpio.h"  /* Cần để sử dụng kiểu gpio_num_t */

#ifdef __cplusplus
extern "C" {
#endif

/* Kiểu enum để xác định LED sáng ở mức HIGH hay LOW */
typedef enum {
    LED_ACTIVE_HIGH,  /* LED sáng khi GPIO ở mức 1 */
    LED_ACTIVE_LOW  /* LED sáng khi GPIO ở mức 0 */
    //LED_ACTIVE_LEVEL 
} led_active_t;

/* Cấu trúc LED: gồm chân GPIO và kiểu active (HIGH/LOW) */
typedef struct {
    gpio_num_t pin;            /* Chân GPIO điều khiển LED */
    led_active_t active_level; /* Mức logic để LED sáng */
} led_t;

/* Khởi tạo LED: cấu hình chân GPIO làm output */
void led_init(const led_t* led);

/* Bật LED (sáng) theo cấu hình active_level */
void led_on(const led_t* led);

/* Tắt LED theo cấu hình active_level */
void led_off(const led_t* led);

/* Đảo trạng thái LED (toggle) */
void led_toggle(const led_t* led);

#ifdef __cplusplus
}
#endif

#endif // LED_DRIVER_H
