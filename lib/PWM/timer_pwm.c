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
#define DPORT_LEDC_RST         (1 << 11)   // bit 12: LEDC_RST

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
#define GPIO_PIN_MUX_REG(pin) (*(volatile uint32_t*)(0x3FF49000 + (pin)*4))

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
    conf0 |= (1 << 31);               // LEDC_CLK_EN = 1 (bật clock cho channel):contentReference[oaicite:10]{index=10}:contentReference[oaicite:11]{index=11}
    conf0 |= (timer_num & 0x3) << 0;  // LEDC_TIMER_SEL = timer_num (bit 0-1)
    conf0 |= (1 << 2);                // LEDC_SIG_OUT_EN = 1 (cho phép output):contentReference[oaicite:12]{index=12}
    conf0 |= (0 << 3);                // LEDC_IDLE_LV = 0 (mức output khi idle) (tuỳ chọn)
    LEDC_HSCH_CONF0_REG(channel) = conf0;
    LEDC_HSCH_CONF1_REG(channel) = (1 << 31);

    // 3.2 Thiết lập điểm bật (HPOINT) tại 0: nghĩa là ngõ ra bật cao ngay từ đầu chu kỳ
    LEDC_HSCH_HPOINT_REG(channel) = 0;

    // 4. Cấu hình chân GPIO output qua GPIO matrix
    // Lấy signal index cho channel: CH0->71, CH1->72, ..., CH7->78:contentReference[oaicite:13]{index=13}
    uint32_t sig_index = 4 + channel;
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
    GPIO_PIN_MUX_REG(gpio_num) = (GPIO_PIN_MUX_REG(gpio_num) & ~0x7) | (2 << 0);
}

void pwm_set_duty_cycle(uint8_t channel, uint32_t duty_percent) {
    if (duty_percent > 100) duty_percent = 100;
    // Tính giá trị duty (bằng một giá trị 20-bit cố định, có 4 bit thập phân)
    // full = 2^resolution. Trước đó resolution đã gán cho timer; tại đây lấy giá trị từ thanh ghi HSTIMERx_DUTY_RES
    // Đơn giản: ta dùng 2^duty_res từ init
    // = ((1<<duty_res)-1)*duty_percent/100, sau đó nhân 16 (dịch trái 4) để có giá trị ghi vào DUTY_REG.
    // Giả sử duty_res trong phạm vi 1-20 (4 bit trừ đi)
    uint32_t max_count = (1 << (LEDC_HSTIMER_CONF_REG(0) & 0x1F)) - 1; // dùng RES từ timer 0 chẳng hạn (giả định cùng dùng)
    // Tuy nhiên dễ nhất: sử dụng công thức phần trăm dựa trên 2^res:
    // Xác định res đã thiết lập:
    uint32_t res = LEDC_HSTIMER_CONF_REG(0) & 0x1F;
    max_count = (1 << res) - 1;
    uint32_t duty_val = (max_count * duty_percent) / 100;
    uint32_t duty_reg = duty_val << 4;  // shift 4 bit thập phân:contentReference[oaicite:15]{index=15}
    LEDC_HSCH_DUTY_REG(channel) = duty_reg;
    // (Không cần viết HPOINT lại vì đã ở 0)
    // (Giá trị này sẽ cập nhật vào kênh tự động theo hardware)
}
