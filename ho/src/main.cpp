#include <stdio.h>

#include "gpio_config.h"
#include "led_driver.h"
#include "motor_driver.h"
#include "adc_driver.h"
#include "uart_driver.h"

extern "C" void app_main() {
    loadPinMap();

    //led_init();
    motor_init();//lỗi
    adc_init();
    uart2_init();

    //led_set(true, false); // Bật LED đỏ

    motor_set_speed(128); // 50% tốc độ(lỗi)

    int gas = adc_read_gas();

    char msg[64];
    //sprintf(msg, "Gas Value: %d\r\n", gas);//lỗi
    uart2_send(msg);
}
