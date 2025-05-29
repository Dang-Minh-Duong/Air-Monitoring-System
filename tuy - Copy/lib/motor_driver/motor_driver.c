#include <stdint.h>
#include "motor_driver.h"

// ===== Định nghĩa thanh ghi và địa chỉ =====
#define DPORT_PERIP_CLK_EN_REG       (*(volatile uint32_t*)0x3FF000C0)     // Clock enable register
#define GPIO_ENABLE_W1TS_REG         (*(volatile uint32_t*)0x3FF44024)     // Enable GPIO output (GPIO0~31)
#define GPIO_ENABLE1_W1TS_REG        (*(volatile uint32_t*)0x3FF4402C)     // Enable GPIO output (GPIO32~39)
#define GPIO_OUT_W1TS_REG            (*(volatile uint32_t*)0x3FF44008)     // GPIO output set register (0~31)
#define GPIO_OUT_W1TC_REG            (*(volatile uint32_t*)0x3FF4400C)     // GPIO output clear register (0~31)
#define GPIO_OUT1_W1TS_REG           (*(volatile uint32_t*)0x3FF44010)     // GPIO output set register (32~39)
#define GPIO_OUT1_W1TC_REG           (*(volatile uint32_t*)0x3FF44014)     // GPIO output clear register (32~39)
#define GPIO_FUNC_OUT_SEL_CFG_REG(n) (*(volatile uint32_t*)(0x3FF44530 + (n)*4))  // GPIO output function select

// IO_MUX base address for GPIO config (for FUNC_SEL and other pin config)
static const uint32_t gpio_io_mux_addr[] = {
    0x3FF49044, 0x3FF49088, 0x3FF49040, 0x3FF49084, 0x3FF49048,
    0x3FF4906C, 0, 0, 0, 0, 0, 0, 0x3FF49034, 0x3FF49038, 0x3FF49030,
    0x3FF4903C, 0x3FF4904C, 0x3FF49050, 0x3FF49070, 0x3FF49074,
    0, 0x3FF4907C, 0x3FF49080, 0x3FF4908C,
    0, 0x3FF49024, 0x3FF49028, 0x3FF4902C
};

// LEDC (PWM) registers base for high-speed channels
#define LEDC_HSTIMER_CONF_REG(n) (*(volatile uint32_t*)(0x3FF59140 + (n)*8))
#define LEDC_HSCH_CONF0_REG(n)   (*(volatile uint32_t*)(0x3FF59000 + (n)*20))
#define LEDC_HSCH_HPOINT_REG(n)  (*(volatile uint32_t*)(0x3FF59004 + (n)*20))
#define LEDC_HSCH_DUTY_REG(n)    (*(volatile uint32_t*)(0x3FF59008 + (n)*20))
#define LEDC_HSCH_CONF1_REG(n)   (*(volatile uint32_t*)(0x3FF5900C + (n)*20))

// ===== Hàm khởi tạo PWM cho motor =====
void motor_init(const motor_t *motor) {
    // 1. Enable clock cho LEDC module (bit 11)
    DPORT_PERIP_CLK_EN_REG |= (1 << 11);

    // 2. Cấu hình chân GPIO cho IN1 và IN2 làm output bình thường
    if (motor->in1_pin < 32) {
        GPIO_ENABLE_W1TS_REG = (1 << motor->in1_pin);
    } else {
        GPIO_ENABLE1_W1TS_REG = (1 << (motor->in1_pin - 32));
    }
    if (motor->in2_pin < 32) {
        GPIO_ENABLE_W1TS_REG = (1 << motor->in2_pin);
    } else {
        GPIO_ENABLE1_W1TS_REG = (1 << (motor->in2_pin - 32));
    }

    // Thiết lập IO_MUX cho in1_pin và in2_pin để FUNC_SEL=0 (GPIO)
    if (motor->in1_pin < sizeof(gpio_io_mux_addr)/sizeof(uint32_t) && gpio_io_mux_addr[motor->in1_pin] != 0) {
        volatile uint32_t *reg = (volatile uint32_t*)gpio_io_mux_addr[motor->in1_pin];
        *reg &= ~(0x7 << 0); // FUNC_SEL=0 (GPIO)
    }
    if (motor->in2_pin < sizeof(gpio_io_mux_addr)/sizeof(uint32_t) && gpio_io_mux_addr[motor->in2_pin] != 0) {
        volatile uint32_t *reg = (volatile uint32_t*)gpio_io_mux_addr[motor->in2_pin];
        *reg &= ~(0x7 << 0); // FUNC_SEL=0 (GPIO)
    }

    // 3. Cấu hình chân PWM cho LEDC:
    // - Gán timer cho channel
    LEDC_HSCH_CONF0_REG(motor->pwm_channel) &= ~0x3;  // Xóa bits timer_sel [1:0]
    LEDC_HSCH_CONF0_REG(motor->pwm_channel) |= (motor->pwm_timer & 0x3);

    // - Cho phép output
    LEDC_HSCH_CONF0_REG(motor->pwm_channel) |= (1 << 2); // output_en=1

    // - Cấu hình timer:
    volatile uint32_t *timer_conf = &LEDC_HSTIMER_CONF_REG(motor->pwm_timer);
    *timer_conf &= ~(0x1F); // xóa resolution bits [4:0]
    *timer_conf |= motor->pwm_duty_resolution;

    // Tính divider = 80MHz / (freq * 2^resolution)
    uint32_t divider = 80000000 / (motor->pwm_freq_hz * (1 << motor->pwm_duty_resolution));
    // Clear bits [22:5] và set lại divider
    *timer_conf &= ~(((1 << 18) - 1) << 5);
    *timer_conf |= (divider << 13);

    // Cấu hình chân GPIO cho PWM:
    // Đặt chân ra LEDC output
    GPIO_FUNC_OUT_SEL_CFG_REG(motor->pwm_pin) = 71 + motor->pwm_channel;

    // Bật output GPIO:
    if (motor->pwm_pin < 32) {
        GPIO_ENABLE_W1TS_REG = (1 << motor->pwm_pin);
    } else {
        GPIO_ENABLE1_W1TS_REG = (1 << (motor->pwm_pin - 32));
    }

    // Cấu hình IO_MUX chân PWM FUNC_SEL=2 (LEDC)
    if (motor->pwm_pin < sizeof(gpio_io_mux_addr)/sizeof(uint32_t) && gpio_io_mux_addr[motor->pwm_pin] != 0) {
        volatile uint32_t *reg = (volatile uint32_t*)gpio_io_mux_addr[motor->pwm_pin];
        *reg &= ~(0x7 << 12);
        *reg |= (2 << 12); // FUNC_SEL=2 là LEDC
    }

    // Đặt duty ban đầu = 0
    LEDC_HSCH_DUTY_REG(motor->pwm_channel) = 0;
    LEDC_HSCH_CONF1_REG(motor->pwm_channel) |= (1U << 31); // cập nhật duty

    // Bỏ chế độ pause timer
    *timer_conf &= ~(1U << 24);
}

// ===== Hàm chạy motor tiến =====
void motor_forward(const motor_t *motor) {
    // IN1 = HIGH
    if (motor->in1_pin < 32) {
        GPIO_OUT_W1TS_REG = (1 << motor->in1_pin);
    } else {
        GPIO_OUT1_W1TS_REG = (1 << (motor->in1_pin - 32));
    }

    // IN2 = LOW
    if (motor->in2_pin < 32) {
        GPIO_OUT_W1TC_REG = (1 << motor->in2_pin);
    } else {
        GPIO_OUT1_W1TC_REG = (1 << (motor->in2_pin - 32));
    }
}

// ===== Hàm chạy motor lùi =====
void motor_backward(const motor_t *motor) {
    // IN1 = LOW
    if (motor->in1_pin < 32) {
        GPIO_OUT_W1TC_REG = (1 << motor->in1_pin);
    } else {
        GPIO_OUT1_W1TC_REG = (1 << (motor->in1_pin - 32));
    }

    // IN2 = HIGH
    if (motor->in2_pin < 32) {
        GPIO_OUT_W1TS_REG = (1 << motor->in2_pin);
    } else {
        GPIO_OUT1_W1TS_REG = (1 << (motor->in2_pin - 32));
    }
}

// ===== Hàm dừng motor =====
void motor_stop(const motor_t *motor) {
    // IN1 = LOW, IN2 = LOW
    if (motor->in1_pin < 32) {
        GPIO_OUT_W1TC_REG = (1 << motor->in1_pin);
    } else {
        GPIO_OUT1_W1TC_REG = (1 << (motor->in1_pin - 32));
    }

    if (motor->in2_pin < 32) {
        GPIO_OUT_W1TC_REG = (1 << motor->in2_pin);
    } else {
        GPIO_OUT1_W1TC_REG = (1 << (motor->in2_pin - 32));
    }

    // Tắt PWM (duty = 0)
    LEDC_HSCH_DUTY_REG(motor->pwm_channel) = 0;
    LEDC_HSCH_CONF1_REG(motor->pwm_channel) |= (1U << 31);
}

// ===== Hàm set tốc độ motor bằng PWM duty cycle (%) =====
void motor_set_speed(const motor_t *motor, uint32_t duty_percent) {
    if (duty_percent > 100) duty_percent = 100;

    uint32_t max_duty = (1 << motor->pwm_duty_resolution) - 1;
    uint32_t duty = (duty_percent * max_duty) / 100;

    LEDC_HSCH_DUTY_REG(motor->pwm_channel) = duty << 4;  // ESP32 LEDC dùng 4-bit lùi (shift 4)
    LEDC_HSCH_CONF1_REG(motor->pwm_channel) |= (1U << 31); // cập nhật duty
}
