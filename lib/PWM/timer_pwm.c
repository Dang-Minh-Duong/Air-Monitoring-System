#include "pwm.h"

// ===== Thanh ghi điều khiển ngoại vi =====
#define DPORT_PERIP_CLK_EN_REG       (*(volatile uint32_t*)0x3FF000C0)     // Bật clock cho các ngoại vi
#define GPIO_ENABLE_W1TS_REG         (*(volatile uint32_t*)0x3FF44024)     // Cho phép output GPIO < 32
#define GPIO_ENABLE1_REG             (*(volatile uint32_t*)0x3FF4402C)     // Cho phép output GPIO >= 32
#define GPIO_FUNC0_OUT_SEL_CFG_REG   ((volatile uint32_t*)0x3FF44530)      // Định tuyến tín hiệu output

// ===== Bảng ánh xạ IO_MUX theo GPIO =====
static const uint32_t gpio_io_mux_addr[] = {
    [0]  = 0x3FF49044, [1]  = 0x3FF49088, [2]  = 0x3FF49040, [3]  = 0x3FF49084,
    [4]  = 0x3FF49048, [5]  = 0x3FF4906C, [12] = 0x3FF49034, [13] = 0x3FF49038,
    [14] = 0x3FF49030, [15] = 0x3FF4903C, [16] = 0x3FF4904C, [17] = 0x3FF49050,
    [18] = 0x3FF49070, [19] = 0x3FF49074, [21] = 0x3FF4907C, [22] = 0x3FF49080,
    [23] = 0x3FF4908C, [25] = 0x3FF49024, [26] = 0x3FF49028, [27] = 0x3FF4902C,
    [32] = 0x3FF4901C, [33] = 0x3FF49020, [34] = 0x3FF49014, [35] = 0x3FF49018,
    [36] = 0x3FF49004, [37] = 0x3FF49008, [38] = 0x3FF4900C, [39] = 0x3FF49010,
};

// ===== LEDC Timer và Channel base =====
#define LEDC_HSTIMER_CONF_REG(n)     (*(volatile uint32_t*)(0x3FF59140 + (n) * 0x8))    // Timer config
#define LEDC_HSCH_CONF0_REG(n)       (*(volatile uint32_t*)(0x3FF59000 + (n) * 0x14))   // Channel config 0
#define LEDC_HSCH_HPOINT_REG(n)      (*(volatile uint32_t*)(0x3FF59004 + (n) * 0x14))   // High point
#define LEDC_HSCH_DUTY_REG(n)        (*(volatile uint32_t*)(0x3FF59008 + (n) * 0x14))   // Duty
#define LEDC_HSCH_CONF1_REG(n)       (*(volatile uint32_t*)(0x3FF5900C + (n) * 0x14))   // Channel config 1

// ===== Khởi tạo PWM =====
void pwm_init(uint8_t timer_num, uint8_t channel_num, uint8_t resolution_bits, uint8_t gpio_num, uint32_t freq_hz) {
    // --- Bật clock cho LEDC ---
    DPORT_PERIP_CLK_EN_REG |= (1 << 11); // LEDC_EN (bit 11)

    // --- Cấu hình timer ---
    volatile uint32_t* timer_conf = &LEDC_HSTIMER_CONF_REG(timer_num);

    *timer_conf &= ~(0x1F);                      // Xóa các bit độ phân giải cũ [0:4]
    *timer_conf |= (resolution_bits & 0x1F);     // Ghi độ phân giải mới (max 0x1F)

    uint32_t divider = 80000000 / (freq_hz * (1 << resolution_bits)); // Tính divider để tạo tần số 5kHz
    *timer_conf &= ~(0x3FFFF << 5);             // Xóa bit [22:5]
    *timer_conf |= ((divider) << 13);  // Ghi divider vào [22:5]

    // --- Cấu hình channel ---
    LEDC_HSCH_CONF0_REG(channel_num) &= ~(0b11);            // Xóa chọn timer cũ
    LEDC_HSCH_CONF0_REG(channel_num) |= (timer_num & 0x3);  // Chọn timer mới (0~3)

    LEDC_HSCH_CONF0_REG(channel_num) |= (1 << 2);           // Bật channel (bit EN)

    LEDC_HSCH_HPOINT_REG(channel_num) = 1;                  // Hpoint = 1

    LEDC_HSCH_DUTY_REG(channel_num) &= ~(0xffffff);
    LEDC_HSCH_DUTY_REG(channel_num) = (20 << 4);            // Duty mặc định (dịch trái 4 bit)
    

    
    GPIO_FUNC0_OUT_SEL_CFG_REG[gpio_num] = 71 + channel_num; 

    if (gpio_num < 32) {
        GPIO_ENABLE_W1TS_REG |= (1 << gpio_num);          
    } else {
        GPIO_ENABLE1_REG |= (1 << (gpio_num - 32));        
    }

    volatile uint32_t* io_mux_reg = (volatile uint32_t*)gpio_io_mux_addr[gpio_num];
    *io_mux_reg &= ~(0b111 << 12);                          
    *io_mux_reg |= (2 << 12);                               

    LEDC_HSCH_CONF1_REG(channel_num) |= (1 << 31);          
    // --- Cho phép timer chạy ---
    *timer_conf &= ~(1 << 23); // Bit 23 là PAUSE, ghi 0 để chạy
}

// ===== Thay đổi duty runtime =====
void pwm_set_duty(uint8_t channel_num, uint32_t duty_value) {
    LEDC_HSCH_DUTY_REG(channel_num) = (duty_value << 4);     // Dịch trái 4 bit
    LEDC_HSCH_CONF1_REG(channel_num) |= (1 << 31);           // Cập nhật duty mới
}