#include "timer.h"
#include <Arduino.h>

// Base address for Timer Group 0
#define TIMG0_BASE              0x3FF5F000

// Timer 0 Register Offsets
#define TIMG_T0CONFIG       (*(volatile uint32_t *)(TIMG0_BASE + 0x0000))
#define TIMG_T0LO           (*(volatile uint32_t *)(TIMG0_BASE + 0x0048))
#define TIMG_T0HI           (*(volatile uint32_t *)(TIMG0_BASE + 0x004C))
#define TIMG_T0UPDATE       (*(volatile uint32_t *)(TIMG0_BASE + 0x0050))
#define TIMG_T0LOADLO       (*(volatile uint32_t *)(TIMG0_BASE + 0x0054))
#define TIMG_T0LOADHI       (*(volatile uint32_t *)(TIMG0_BASE + 0x0058))
#define TIMG_T0LOAD         (*(volatile uint32_t *)(TIMG0_BASE + 0x005C))

#define APB_CLK             80000000 // 80 MHz default

void timer_hw_init() {
    // Enable timer and set to autoreload
    TIMG_T0CONFIG |= (1 << 31); // Enable
    TIMG_T0CONFIG |= (1 << 30); // Autoreload
    TIMG_T0CONFIG &= ~(0xFFFF); // Divider = 1 (prescaler = 1)

    // Reset counter to 0
    TIMG_T0LOADLO = 0;
    TIMG_T0LOADHI = 0;
    TIMG_T0LOAD = 1;
}

uint64_t timer_hw_get_count() {
    TIMG_T0UPDATE = 1; // Trigger update
    uint32_t lo = TIMG_T0LO;
    uint32_t hi = TIMG_T0HI;
    return ((uint64_t)hi << 32) | lo;
}

void delay_us_hw(uint32_t us) {
    uint64_t start = timer_hw_get_count();
    uint64_t ticks = (APB_CLK / 1000000) * us;
    while ((timer_hw_get_count() - start) < ticks) yield();
}

void delay_ms_hw(uint32_t ms) {
    while (ms--) delay_us_hw(1000);
}
