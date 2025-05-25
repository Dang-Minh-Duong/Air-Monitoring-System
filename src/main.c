#include "pwm.h"
#include <stdint.h>

// Delay thủ công bằng vòng lặp (không chính xác nhưng đủ test PWM)
void delay_ms(uint32_t ms) {
    volatile uint32_t count;
    while (ms--) {
        count = 80000;  // khoảng 1ms nếu CPU 80MHz
        while (count--) {
            __asm__ __volatile__("nop");
        }
    }
}

void app_main() {
    // Khởi tạo PWM: timer1, channel2, 1kHz, 10-bit, GPIO23
    pwm_init(1, 2, 1000, 10, 23);

    while (1) {
        // Tăng độ sáng
        for (uint8_t duty = 0; duty <= 100; duty += 5) {
            pwm_set_duty_cycle(2, duty);  // channel 2
            delay_ms(50);
        }

        // Giảm độ sáng
        for (int8_t duty = 100; duty >= 0; duty -= 5) {
            pwm_set_duty_cycle(2, duty);
            delay_ms(50);
        }
    }
}
