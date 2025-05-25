#include <stdio.h>
#include "ADC.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    // Cấu hình ADC1 channel 0 (GPIO36), độ phân giải 12-bit, suy giảm 11dB, chu kỳ lấy mẫu = 10
    adc_configure(ADC_UNIT_2, 4, ADC_WIDTH_12Bit, ADC_ATTEN_11DB, 5);

    while (1)
    {
        // Đọc giá trị ADC
        uint16_t value = adc_read(ADC_UNIT_2);

        // In kết quả ra UART
        printf("ADC1 Channel 0 = %d\n", value);

        // Delay 500ms
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
