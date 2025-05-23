#include "pwm.h"

// Hàm delay đơn giản
void delay(volatile int count) {
    while(count--) __asm__("nop");
}

int main() {
    pwm_config_t pwm_cfg = {
        .timer_num = 0,
        .channel_num = 0,
        .gpio_num = 18,
        .freq_hz = 1000,
        .bit_num = 8,
        .duty_percent = 50
    };

    pwm_init(&pwm_cfg);

    while(1) {
        delay(1000000);
    }

    return 0;
}
