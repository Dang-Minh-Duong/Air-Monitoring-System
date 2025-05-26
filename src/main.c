// #include <stdio.h>
// #include "ADC.h"
// #include "pin.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// void app_main(void)
// {
    
//     adc_configure_pin(ADC_UNIT_2, 14);  /* RTC_IO pin 0 (GPIO36) cho ADC1_CHANNEL_0 */
//     adc_configure(ADC_UNIT_2, 4, ADC_WIDTH_12Bit, ADC_ATTEN_11DB, 10);

//     while (1)
//     {
        
//         uint16_t value = adc_read(ADC_UNIT_2);

//         // In kết quả ra UART
//         printf("ADC1 Channel 0 = %d\n", value);

//         // Delay 500ms
//         vTaskDelay(pdMS_TO_TICKS(500));
//     }
// }
#include <stdio.h>
#include <stdint.h>
#include "pwm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Cấu hình thông số PWM
#define PWM_TIMER      0
#define PWM_CHANNEL    0
#define PWM_RES_BITS   10               // Độ phân giải 10-bit (0~1023)
#define PWM_GPIO       26               // GPIO bất kỳ hỗ trợ output
#define PWM_FREQ_HZ    5000             // Tần số 5 kHz

void app_main(void) {

    pwm_init(PWM_TIMER, PWM_CHANNEL, PWM_RES_BITS, PWM_GPIO, PWM_FREQ_HZ);
    
    uint32_t duty = 0;
    uint32_t max_duty = (1 << PWM_RES_BITS);

    while (1) {
        pwm_set_duty(PWM_CHANNEL, duty);

        duty += max_duty / 10;
        if (duty > max_duty) duty = 0;

        vTaskDelay(pdMS_TO_TICKS(500)); // Chờ 500ms
    }
}