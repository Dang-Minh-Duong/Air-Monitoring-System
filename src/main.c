// #include <stdio.h>
// #include "ADC.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// void app_main(void)
// {
//     // Cấu hình ADC1 channel 0 (GPIO36), độ phân giải 12-bit, suy giảm 11dB, chu kỳ lấy mẫu = 10
//     adc_configure(ADC_UNIT_2, 4, ADC_WIDTH_12Bit, ADC_ATTEN_11DB, 10);

//     while (1)
//     {
//         // Đọc giá trị ADC
//         uint16_t value = adc_read(ADC_UNIT_2);

//         // In kết quả ra UART
//         printf("ADC1 Channel 0 = %d\n", value);

//         // Delay 500ms
//         vTaskDelay(pdMS_TO_TICKS(500));
//     }
// }


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
    pwm_init(0, 0, 1000, 10, 18);

    while (1) {
        // Tăng độ sáng
        for (uint8_t duty = 0; duty <= 100; duty += 5) {
            pwm_set_duty_cycle(0, duty);  // channel 2
            delay_ms(500);
        }

        // Giảm độ sáng
        for (int8_t duty = 100; duty >= 0; duty -= 5) {
            pwm_set_duty_cycle(0, duty);
            delay_ms(500);
        }
    }
}