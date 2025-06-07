#ifndef TIMER_REGS_H
#define TIMER_REGS_H

/** Base addresses for ESP32 Timer Groups */
#define TG0_BASE_ADDR 0x3FF5F000
#define TG1_BASE_ADDR 0x3FF60000

/** Register offsets for each timer (0x24 bytes apart) */
#define TIMER_CONFIG_OFFSET     0x0000
#define TIMER_COUNT_LO_OFFSET   0x0008
#define TIMER_COUNT_HI_OFFSET   0x000C
#define TIMER_ALARM_LO_OFFSET   0x0010
#define TIMER_ALARM_HI_OFFSET   0x0014
#define TIMER_LOAD_LO_OFFSET    0x0018
#define TIMER_LOAD_HI_OFFSET    0x001C
#define TIMER_LOAD_OFFSET       0x0020

/** Bit masks for timer configuration */
#define TIMER_ENABLE_BIT        (1UL << 31)
#define TIMER_AUTO_RELOAD_BIT   (1UL << 29)
#define TIMER_ALARM_EN          (1UL << 10)
#define TIMER_PRESCALER_MASK    0xFFFF
#define TIMER_PRESCALER_SHIFT   13

#endif // TIMER_REGS_H
