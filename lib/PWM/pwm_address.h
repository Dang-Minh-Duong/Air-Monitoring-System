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