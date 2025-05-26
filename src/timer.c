#include "timer_module.h"
#include "esp_intr_alloc.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Base addresses for Timer Groups (for ESP32) */
#define TG0_BASE_ADDR 0x3FF5F000
#define TG1_BASE_ADDR 0x3FF60000

/* Register offsets for a given timer (each timer occupies 0x20 bytes) */
#define TIMER_CONFIG_OFFSET    0x0000
#define TIMER_LO_OFFSET        0x0004
#define TIMER_HI_OFFSET        0x0008
#define TIMER_UPDATE_OFFSET    0x000C
#define TIMER_ALARM_LO_OFFSET  0x0010
#define TIMER_ALARM_HI_OFFSET  0x0014
#define TIMER_LOAD_LO_OFFSET   0x0018
#define TIMER_LOAD_HI_OFFSET   0x001C
#define TIMER_LOAD_OFFSET      0x0020

/* Timer register bit definitions */
#define TIMER_ENABLE_BIT       (1UL << 31)  /* Enable the timer counter */
#define TIMER_AUTO_RELOAD_BIT  (1UL << 29)  /* Auto-reload: reset counter when alarm reached */
/* When set, TIMER_ALARM_EN enables the alarm (hardware clears it upon event) */
#define TIMER_ALARM_EN         (1UL << 10)

/* 
 * Global array of interrupt flags for four timers.
 * Index mapping as described in timer_module.h.
 */
volatile bool timer_isr_triggered[4] = { false, false, false, false };

/* 
 * Static array to hold timer parameters (group and timer index) for each of the 4 timers.
 * Each row represents a pair: { group, timer }.
 * Mapping: index = group * 2 + timer.
 */
static int s_timer_info[4][2] = {
    { 0, 0 },   /* Timer Group 0, Timer 0 */
    { 0, 1 },   /* Timer Group 0, Timer 1 */
    { 1, 0 },   /* Timer Group 1, Timer 0 */
    { 1, 1 }    /* Timer Group 1, Timer 1 */
};

/* 
 * @brief Initialize a hardware timer by directly accessing its registers.
 */
void Timer_Init(int group, int timer, uint32_t prescaler, uint32_t arr, int enableInterrupt) {
    /* Determine the base address for the selected timer group */
    uint32_t base_address = (group == 0) ? TG0_BASE_ADDR : TG1_BASE_ADDR;
    /* Calculate the offset for the selected timer (each timer occupies 0x20 bytes) */
    uint32_t timer_offset = timer * 0x20;
    
    /* Create pointers to the timer registers */
    volatile uint32_t *tconfig   = (volatile uint32_t *)(base_address + timer_offset + TIMER_CONFIG_OFFSET);
    volatile uint32_t *tlo       = (volatile uint32_t *)(base_address + timer_offset + TIMER_LO_OFFSET);
    volatile uint32_t *thi       = (volatile uint32_t *)(base_address + timer_offset + TIMER_HI_OFFSET);
    volatile uint32_t *tupdate   = (volatile uint32_t *)(base_address + timer_offset + TIMER_UPDATE_OFFSET);
    volatile uint32_t *talarmlo  = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_LO_OFFSET);
    volatile uint32_t *talarmhi  = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_HI_OFFSET);
    volatile uint32_t *tloadlo   = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_LO_OFFSET);
    volatile uint32_t *tloadhi   = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_HI_OFFSET);
    volatile uint32_t *tload     = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_OFFSET);
    
    /* Disable the timer before modifying its configuration */
    *tconfig &= ~TIMER_ENABLE_BIT;
    
    /* Clear bits [13-28] to set the prescaler and then set it */
    *tconfig &= ~(((uint32_t)0xFFFF) << 13);
    *tconfig |= (prescaler << 13);
    
    /* Enable auto-reload feature */
    *tconfig |= TIMER_AUTO_RELOAD_BIT;
    
    /* Set the alarm (auto-reload) value */
    *talarmlo = arr;
    *talarmhi = 0;
    
    /* Reset the counter by loading 0 */
    *tloadlo = 0;
    *tloadhi = 0;
    *tload   = 1;
    
    /* If interrupts are enabled (either edge or level), turn on the alarm */
    if (enableInterrupt == TIMER_INT_EDGE || enableInterrupt == TIMER_INT_LEVEL) {
        *tconfig |= TIMER_ALARM_EN;
    } else {
        *tconfig &= ~TIMER_ALARM_EN;
    }
    
    /* Finally, enable the timer */
    *tconfig |= TIMER_ENABLE_BIT;
}

/* 
 * @brief Create a blocking delay using the hardware timer.
 *
 * Assumes that each tick equals 1µs (e.g., prescaler = 80 for an 80MHz clock).
 */
void Timer_Delay(int group, int timer, uint32_t ms) {
    /* Determine the base address for the selected group */
    uint32_t base_address = (group == 0) ? TG0_BASE_ADDR : TG1_BASE_ADDR;
    uint32_t timer_offset = timer * 0x20;
    
    volatile uint32_t *tconfig   = (volatile uint32_t *)(base_address + timer_offset + TIMER_CONFIG_OFFSET);
    volatile uint32_t *tupdate   = (volatile uint32_t *)(base_address + timer_offset + TIMER_UPDATE_OFFSET);
    volatile uint32_t *talarmlo  = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_LO_OFFSET);
    volatile uint32_t *tloadlo   = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_LO_OFFSET);
    volatile uint32_t *tloadhi   = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_HI_OFFSET);
    volatile uint32_t *tload     = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_OFFSET);
    
    /* Calculate delay ticks (assumes 1µs per tick) */
    uint32_t delay_ticks = ms * 1000;
    
    /* Reset the timer counter to 0 */
    *tloadlo = 0;
    *tloadhi = 0;
    *tload   = 1;
    
    /* Set the new alarm value */
    *talarmlo = delay_ticks;
    
    /* Busy-wait until the hardware clears the alarm enable bit */
    while (*tconfig & TIMER_ALARM_EN) {
        *tupdate = 1;  /* Continuously update the timer counter value */
    }
    
    /* Re-enable the alarm for subsequent delay calls */
    *tconfig |= TIMER_ALARM_EN;
}

/* 
 * @brief Interrupt Service Routine (ISR) for the timer.
 *
 * This function should be placed in IRAM for fast interrupt handling.
 * It clears the alarm flag in the timer register and sets the global interrupt flag
 * corresponding to the timer that triggered the ISR.
 *
 * @param arg Pointer to an integer array containing {group, timer}.
 */
void IRAM_ATTR timerInterruptHandler(void *arg) {
    int *timer_info = (int *) arg;
    int group = timer_info[0];
    int timer = timer_info[1];
    int index = group * 2 + timer;
    
    /* Determine the base address and offset for this timer */
    uint32_t base_address = (group == 0) ? TG0_BASE_ADDR : TG1_BASE_ADDR;
    uint32_t timer_offset = timer * 0x20;
    volatile uint32_t *tconfig = (volatile uint32_t *)(base_address + timer_offset + TIMER_CONFIG_OFFSET);
    
    /* Clear the alarm enable bit to acknowledge the interrupt */
    *tconfig &= ~TIMER_ALARM_EN;
    
    /* Set the global interrupt flag for this timer */
    timer_isr_triggered[index] = true;
    
    ets_printf("ISR: Timer Group %d Timer %d interrupt triggered\n", group, timer);
}

/* 
 * @brief Register the timer ISR using esp_intr_alloc().
 *
 * Based on the timer group and timer index, the proper interrupt source is selected.
 * The static s_timer_info array is used to pass the timer parameters to the ISR.
 *
 * @param group Timer group (0 or 1)
 * @param timer Timer index (0 or 1)
 */
void Timer_Isr_Register(int group, int timer) {
    intr_handle_t isr_handle = NULL;
    int index = group * 2 + timer;
    int intr_source = 0;
    
    /* Determine the appropriate interrupt source based on group and timer index */
    if (group == 0 && timer == 0) {
        intr_source = ETS_TG0_T0_LEVEL_INTR_SOURCE;
    } else if (group == 0 && timer == 1) {
        intr_source = ETS_TG0_T1_LEVEL_INTR_SOURCE;
    } else if (group == 1 && timer == 0) {
        intr_source = ETS_TG1_T0_LEVEL_INTR_SOURCE;
    } else if (group == 1 && timer == 1) {
        intr_source = ETS_TG1_T1_LEVEL_INTR_SOURCE;
    }
    
    /* Allocate the interrupt and register the timerInterruptHandler */
    esp_err_t ret = esp_intr_alloc(intr_source, ESP_INTR_FLAG_IRAM, timerInterruptHandler,
                                   (void *)s_timer_info[index], &isr_handle);
    if (ret != ESP_OK) {
       ets_printf("Failed to allocate timer ISR handler for Timer Group %d Timer %d\n", group, timer);
    } else {
       ets_printf("Timer ISR registered successfully for Timer Group %d Timer %d\n", group, timer);
    }
}
