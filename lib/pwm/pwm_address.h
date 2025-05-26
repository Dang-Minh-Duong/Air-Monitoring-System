#define LEDC_BASE 0x3FF59000

// Configuration registers
#define LEDC_CONF_REG             (*(volatile uint32_t *)(0x3FF59190))

// High-speed channel configuration registers
#define LEDC_HSCH0_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x00))
#define LEDC_HSCH0_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x0C))
#define LEDC_HSCH1_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x14))
#define LEDC_HSCH1_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x20))
#define LEDC_HSCH2_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x28))
#define LEDC_HSCH2_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x34))
#define LEDC_HSCH3_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x3C))
#define LEDC_HSCH3_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x48))
#define LEDC_HSCH4_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x50))
#define LEDC_HSCH4_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x5C))
#define LEDC_HSCH5_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x64))
#define LEDC_HSCH5_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x70))
#define LEDC_HSCH6_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x78))
#define LEDC_HSCH6_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x84))
#define LEDC_HSCH7_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x8C))
#define LEDC_HSCH7_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x98))

// Low-speed channel configuration registers
#define LEDC_LSCH0_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xA0))
#define LEDC_LSCH0_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xAC))
#define LEDC_LSCH1_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xB4))
#define LEDC_LSCH1_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xC0))
#define LEDC_LSCH2_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xC8))
#define LEDC_LSCH2_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xD4))
#define LEDC_LSCH3_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xDC))
#define LEDC_LSCH3_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xE8))
#define LEDC_LSCH4_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xF0))
#define LEDC_LSCH4_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xFC))
#define LEDC_LSCH5_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x104))
#define LEDC_LSCH5_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x110))
#define LEDC_LSCH6_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x118))
#define LEDC_LSCH6_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x124))
#define LEDC_LSCH7_CONF0_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x12C))
#define LEDC_LSCH7_CONF1_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x138))

// Duty-cycle registers for high-speed channels
#define LEDC_HSCH0_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x08))
#define LEDC_HSCH1_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x1C))
#define LEDC_HSCH2_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x30))
#define LEDC_HSCH3_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x44))
#define LEDC_HSCH4_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x58))
#define LEDC_HSCH5_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x6C))
#define LEDC_HSCH6_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x80))
#define LEDC_HSCH7_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x94))

// Duty-cycle registers for low-speed channels
#define LEDC_LSCH0_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xA8))
#define LEDC_LSCH1_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xBC))
#define LEDC_LSCH2_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xD0))
#define LEDC_LSCH3_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xE4))
#define LEDC_LSCH4_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0xF8))
#define LEDC_LSCH5_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x10C))
#define LEDC_LSCH6_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x120))
#define LEDC_LSCH7_DUTY_REG      (*(volatile uint32_t *)(LEDC_BASE + 0x134))

// High-level comparator registers (used for PWM timing)
#define LEDC_HSCH0_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0x04))
#define LEDC_HSCH1_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0x18))
#define LEDC_HSCH2_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0x2C))
#define LEDC_HSCH3_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0x40))
#define LEDC_HSCH4_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0x54))
#define LEDC_HSCH5_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0x68))
#define LEDC_HSCH6_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0x7C))
#define LEDC_HSCH7_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0x90))

#define LEDC_LSCH0_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0xA4))
#define LEDC_LSCH1_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0xB8))
#define LEDC_LSCH2_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0xCC))
#define LEDC_LSCH3_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0xE0))
#define LEDC_LSCH4_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0xF4))
#define LEDC_LSCH5_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0x108))
#define LEDC_LSCH6_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0x11C))
#define LEDC_LSCH7_HPOINT_REG    (*(volatile uint32_t *)(LEDC_BASE + 0x130))

// Timer configuration registers for high-speed timers
#define LEDC_HSTIMER0_CONF_REG   (*(volatile uint32_t *)(0x3FF59140))
#define LEDC_HSTIMER1_CONF_REG   (*(volatile uint32_t *)(0x3FF59148))
#define LEDC_HSTIMER2_CONF_REG   (*(volatile uint32_t *)(0x3FF59150))
#define LEDC_HSTIMER3_CONF_REG   (*(volatile uint32_t *)(0x3FF59158))

// Timer configuration registers for low-speed timers
#define LEDC_LSTIMER0_CONF_REG   (*(volatile uint32_t *)(0x3FF59160))
#define LEDC_LSTIMER1_CONF_REG   (*(volatile uint32_t *)(0x3FF59168))
#define LEDC_LSTIMER2_CONF_REG   (*(volatile uint32_t *)(0x3FF59170))
#define LEDC_LSTIMER3_CONF_REG   (*(volatile uint32_t *)(0x3FF59178))

// Interrupt registers
#define LEDC_INT_RAW_REG         (*(volatile uint32_t *)(0x3FF59180))
#define LEDC_INT_ST_REG          (*(volatile uint32_t *)(0x3FF59184))
#define LEDC_INT_ENA_REG         (*(volatile uint32_t *)(0x3FF59188))
#define LEDC_INT_CLR_REG         (*(volatile uint32_t *)(0x3FF5918C))




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