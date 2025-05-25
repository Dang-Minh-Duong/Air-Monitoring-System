/* pwm.c – Định nghĩa hàm PWM (config toàn bộ bằng ghi nhớ theo TRM ESP32) */
#include "pwm.h"
#include <math.h>
#include <stdint.h>

/* Địa chỉ cơ sở của module LEDC (theo TRM: DR_REG_LEDC_BASE = 0x3FF59000) */
#define LEDC_BASE            0x3FF59000

/* Địa chỉ DPORT để bật xung và reset module LEDC */
#define DPORT_PERIP_CLK_EN_REG  (*(volatile uint32_t*)(0x3FF000C0))
#define DPORT_PERIP_RST_EN_REG  (*(volatile uint32_t*)(0x3FF000C4))
#define DPORT_LEDC_CLK_EN      (1 << 11)    // bit 11: LEDC_CLK_EN
#define DPORT_LEDC_RST         (1 << 11)   // bit 11: LEDC_RST

/* Macro truy cập thanh ghi LEDC HSTIMER_CONF */
#define LEDC_HSTIMER_CONF_REG(x) (*(volatile uint32_t*)(LEDC_BASE + 0x140 + (x)*8))

/* Macro địa chỉ thanh ghi LEDC HSTIMER_VALUE (đọc bộ đếm; không cần dùng ở đây) */
#define LEDC_HSTIMER_VALUE_REG(x) (*(volatile uint32_t*)(LEDC_BASE + 0x144 + (x)*8))

/* Macro địa chỉ thanh ghi LEDC channel config */
#define LEDC_HSCH_CONF0_REG(ch) (*(volatile uint32_t*)(LEDC_BASE + (ch)*0x14))
#define LEDC_HSCH_CONF1_REG(ch) (*(volatile uint32_t*)(LEDC_BASE + 0x0C + (ch)*0x14))

/* Macro địa chỉ thanh ghi LEDC HPOINT và DUTY cho kênh HS */
#define LEDC_HSCH_HPOINT_REG(ch) (*(volatile uint32_t*)(LEDC_BASE + 0x04 + (ch)*0x14))
#define LEDC_HSCH_DUTY_REG(ch)   (*(volatile uint32_t*)(LEDC_BASE + 0x08 + (ch)*0x14))

/* Địa chỉ GPIO matrix và IO MUX */
#define GPIO_FUNC_OUT_SEL_CFG_REG(pin) (*(volatile uint32_t*)(0x3FF44530 + (pin)*4))
#define GPIO_ENABLE_REG            (*(volatile uint32_t*)(0x3FF44020))
#define GPIO_ENABLE1_REG           (*(volatile uint32_t*)(0x3FF4402C))
static const uint32_t gpio_io_mux_addr[] = {
    [0]  = 0x3FF49044,
    [1]  = 0x3FF49088,
    [2]  = 0x3FF49040,
    [3]  = 0x3FF49084,
    [4]  = 0x3FF49048,
    [5]  = 0x3FF4906C,
    [12] = 0x3FF49034,
    [13] = 0x3FF49038,
    [14] = 0x3FF49030,
    [15] = 0x3FF4903C,
    [16] = 0x3FF4904C,
    [17] = 0x3FF49050,
    [18] = 0x3FF49070,
    [19] = 0x3FF49074,
    [21] = 0x3FF4907C,
    [22] = 0x3FF49080,
    [23] = 0x3FF4908C,
    [25] = 0x3FF49024,
    [26] = 0x3FF49028,
    [27] = 0x3FF4902C,
    [32] = 0x3FF4901C,
    [33] = 0x3FF49020,
    [34] = 0x3FF49014,
    [35] = 0x3FF49018,
    [36] = 0x3FF49004,
    [37] = 0x3FF49008,
    [38] = 0x3FF4900C,
    [39] = 0x3FF49010,
};
static inline volatile uint32_t* get_gpio_mux_reg(uint8_t gpio_num) {
    if (gpio_num >= sizeof(gpio_io_mux_addr)/sizeof(gpio_io_mux_addr[0]) ||
        gpio_io_mux_addr[gpio_num] == 0) {
        return NULL; // GPIO không có IO_MUX hợp lệ
    }
    return (volatile uint32_t*)gpio_io_mux_addr[gpio_num];
}


void pwm_init(uint8_t timer_num, uint8_t channel, uint32_t freq_hz, uint8_t duty_res, uint8_t gpio_num) {
    // 1. Bật xung và reset LEDC qua DPORT
    DPORT_PERIP_CLK_EN_REG |= DPORT_LEDC_CLK_EN;       // enable clock for LEDC:contentReference[oaicite:4]{index=4}
    DPORT_PERIP_RST_EN_REG |= DPORT_LEDC_RST;         // reset LEDC module:contentReference[oaicite:5]{index=5}
    DPORT_PERIP_RST_EN_REG &= ~DPORT_LEDC_RST;        // release LEDC reset

    // 2. Tính toán hệ số chia (divider) và thiết lập HSTIMER
    //  f_LEDC_CLK (APB clock = 80MHz),  LECD_HSTIMERx_DUTY_RES = duty_res
    //  Tính LEDC_CLK_DIVx (float) = f_LEDC_CLK / (freq_hz * 2^duty_res)
    double f_clock = 80000000.0;
    double div = f_clock / (freq_hz * (1 << duty_res));
    if (div < 1.0) div = 1.0;
    if (div > 1023.996) div = 1023.996;  // giới hạn của 10.8 bit fixed-point

    uint32_t A = (uint32_t)div;
    uint32_t B = (uint32_t)((div - A) * 256.0 + 0.5);
    uint32_t clk_div_val = (A << 8) | (B & 0xFF);

    // Thiết lập timer config: chọn APB, reset=1, pause=0, divider và độ rộng counter (DUTY_RES)
    uint32_t hstimer_conf = 0;
    hstimer_conf |= (1 << 25);                 // LEDC_TICK_SEL_HSTIMERx = 1 (APB_CLK):contentReference[oaicite:6]{index=6}
    hstimer_conf |= (0 << 23);                 // LEDC_HSTIMERx_PAUSE = 0 (chạy)
    hstimer_conf |= (1 << 24);                 // LEDC_HSTIMERx_RST = 1 (reset timer):contentReference[oaicite:7]{index=7}
    hstimer_conf |= ((clk_div_val & 0x3FFFF) << 5); // LEDC_CLK_DIV_NUM_HSTIMERx [22:5]:contentReference[oaicite:8]{index=8}
    hstimer_conf |= (duty_res & 0x1F);         // LEDC_HSTIMERx_DUTY_RES [4:0] (số bit counter):contentReference[oaicite:9]{index=9}

    LEDC_HSTIMER_CONF_REG(timer_num) = hstimer_conf;
    // Xóa bit reset (cho phép chạy) sau khi cấu hình xong
    LEDC_HSTIMER_CONF_REG(timer_num) &= ~(1 << 24); // LEDC_HSTIMERx_RST = 0

    // 3. Cấu hình kênh PWM (channel)
    // 3.1 Gán kênh với timer: LEDC_TIMER_SEL (2 bit) tại conf0
    uint32_t conf0 = 0;         
    conf0 |= (timer_num & 0x3) << 0;  // LEDC_TIMER_SEL = timer_num (bit 0-1)
    conf0 |= (1 << 2);                // LEDC_SIG_OUT_EN = 1 (cho phép output):contentReference[oaicite:12]{index=12}
    conf0 |= (0 << 3);                // LEDC_IDLE_LV = 0 (mức output khi idle) (tuỳ chọn)
    LEDC_HSCH_CONF0_REG(channel) = conf0;

    LEDC_HSCH_CONF1_REG(channel) = (1 << 31);
    // 3.2 Thiết lập điểm bật (HPOINT) tại 0: nghĩa là ngõ ra bật cao ngay từ đầu chu kỳ
    LEDC_HSCH_HPOINT_REG(channel) = 0;

    // 4. Cấu hình chân GPIO output qua GPIO matrix
    // Lấy signal index cho channel: CH0->71, CH1->72, ..., CH7->78:contentReference[oaicite:13]{index=13}
    uint32_t sig_index = 71 + channel;
    // Ghi vào GPIO matrix: chân gpio_num xuất tín hiệu sig_index, OEN_SEL=0 (peripheral control)
    uint32_t out_sel = sig_index;      // OUT_SEL = sig_index
    // Bit 11: OEN_SEL = 0 (dùng OE từ LEDC), Bit 10: OEN_INV_SEL = 0, Bit 9: INV_SEL = 0
    // Đặt ôn SEL, INV theo mặc định (0)
    GPIO_FUNC_OUT_SEL_CFG_REG(gpio_num) = out_sel;
    // Tắt output enable của GPIO tích hợp để LEDC điều khiển chân
    if (gpio_num < 32) {
        GPIO_ENABLE_REG &= ~(1U << gpio_num);
    } else {
        GPIO_ENABLE1_REG &= ~(1U << (gpio_num - 32));
    }
    // Định hướng chức năng IO MUX cho chân thành GPIO (chế độ chức năng 2=GPIO)
    volatile uint32_t* mux_reg = get_gpio_mux_reg(gpio_num);
    if (mux_reg) {
        *mux_reg = (*mux_reg & ~(0x3 << 12)) | (2 << 12);
    }

}

void pwm_set_duty_cycle(uint8_t channel, uint32_t duty_percent) {
    if (duty_percent > 100) duty_percent = 100;

    // Lấy timer_num từ channel: LEDC_HSCH_CONF0_REG(channel) bit 1:0 = TIMER_SEL
    uint32_t conf0 = LEDC_HSCH_CONF0_REG(channel);
    uint8_t timer_num = conf0 & 0x3;

    // Lấy resolution từ timer_num: LEDC_HSTIMER_CONF_REG(timer_num) bit 4:0 = DUTY_RES
    uint32_t hstimer_conf = LEDC_HSTIMER_CONF_REG(timer_num);
    uint8_t res = hstimer_conf & 0x1F;

    // Tính giá trị đếm max và duty
    uint32_t max_count = (1U << res) - 1;
    uint32_t duty_val = ((max_count * duty_percent) / 100) << 4;  // dịch trái 4 bit: fixed-point

    // Ghi vào thanh ghi duty của kênh
    LEDC_HSCH_DUTY_REG(channel) = duty_val;
}

