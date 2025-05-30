#include "esp_intr_alloc.h"
#include "esp_attr.h"
#include "soc/interrupts.h"
#include "timer.h"

/** Level interrupt source definitions */
#ifndef ETS_TG0_T0_LEVEL_INTR_SOURCE
#define ETS_TG0_T0_LEVEL_INTR_SOURCE 14
#endif

#ifndef ETS_TG0_T1_LEVEL_INTR_SOURCE
#define ETS_TG0_T1_LEVEL_INTR_SOURCE 15
#endif

#ifndef ETS_TG1_T0_LEVEL_INTR_SOURCE
#define ETS_TG1_T0_LEVEL_INTR_SOURCE 18
#endif

#ifndef ETS_TG1_T1_LEVEL_INTR_SOURCE
#define ETS_TG1_T1_LEVEL_INTR_SOURCE 19
#endif

/** Base addresses for Timer Groups (ESP32) */
#define TG0_BASE_ADDR 0x3FF5F000
#define TG1_BASE_ADDR 0x3FF60000

/** Register offsets for each timer (each timer occupies 0x24 bytes) */
#define TIMER_CONFIG_OFFSET 0x0000
#define TIMER_LO_OFFSET 0x0004
#define TIMER_HI_OFFSET 0x0008
#define TIMER_UPDATE_OFFSET 0x000C
#define TIMER_ALARM_LO_OFFSET 0x0010
#define TIMER_ALARM_HI_OFFSET 0x0014
#define TIMER_LOAD_LO_OFFSET 0x0018
#define TIMER_LOAD_HI_OFFSET 0x001C
#define TIMER_LOAD_OFFSET 0x0020
#define TIMER_INT_EN_OFFSET 0x0098
#define TIMER_INT_CL_OFFSET 0x00A4

/** Bit definitions for timer registers */
#define TIMER_ENABLE_BIT (1UL << 31)
#define TIMER_AUTO_RELOAD_BIT (1UL << 29)
#define TIMER_ALARM_EN (1UL << 10)

/** Global flag array for timer ISR triggers (up to 4 timers) */
volatile bool timer_isr_triggered[4] = {false, false, false, false};

/** Array holding timer info for each timer as {group, timer} */
static int s_timer_info[4][2] = {
    {0, 0}, /**< Timer Group 0, Timer 0 */
    {0, 1}, /**< Timer Group 0, Timer 1 */
    {1, 0}, /**< Timer Group 1, Timer 0 */
    {1, 1}  /**< Timer Group 1, Timer 1 */
};

/**
 * @brief Initialize a hardware timer.
 * @param group Timer group (0 or 1)
 * @param timer Timer number within the group (0 or 1)
 * @param prescaler Prescaler value for dividing the timer clock
 * @param arr 64-bit auto-reload (alarm) value
 * @param enableInterrupt Interrupt mode: TIMER_INT_LEVEL or TIMER_INT_DISABLE
 */
void Timer_Init(int group, int timer, uint32_t prescaler, uint64_t arr, int enableInterrupt)
{
    uint32_t base_address = (group == 0) ? TG0_BASE_ADDR : TG1_BASE_ADDR;
    /* Each timer block is separated by 0x24 bytes */
    uint32_t timer_offset = timer * 0x24;

    volatile uint32_t *tconfig = (volatile uint32_t *)(base_address + timer_offset + TIMER_CONFIG_OFFSET);
    volatile uint32_t *talarmlo = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_LO_OFFSET);
    volatile uint32_t *talarmhi = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_HI_OFFSET);
    volatile uint32_t *tloadlo = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_LO_OFFSET);
    volatile uint32_t *tloadhi = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_HI_OFFSET);
    volatile uint32_t *tload = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_OFFSET);
    volatile uint32_t *tinte = (volatile uint32_t *)(base_address + TIMER_INT_EN_OFFSET);

    /* Disable timer */
    *tconfig &= ~TIMER_ENABLE_BIT;

    /* Set prescaler (clear bits [13-28] and set new value) */
    *tconfig = (*tconfig & ~(((uint32_t)0xFFFF) << 13)) | (prescaler << 13);

    /* Enable auto-reload */
    *tconfig |= TIMER_AUTO_RELOAD_BIT;

    /* Set alarm value (64-bit) */
    *talarmlo = (uint32_t)(arr & 0xFFFFFFFF);
    *talarmhi = (uint32_t)(arr >> 32);

    /* Reset timer counter */
    *tloadlo = 0;
    *tloadhi = 0;
    *tload = 1;

    /* Configure interrupt mode if enabled */
    if (enableInterrupt == TIMER_INT_LEVEL)
    {
        *tconfig |= TIMER_ALARM_EN;
        *tinte |= (1 << timer);
        *tconfig |= (1 << 11); /**< Level interrupt configuration bit */
    }
    else
    {
        /* Disable alarm if interrupts are not used */
        *tconfig &= ~TIMER_ALARM_EN;
    }

    /* Enable timer */
    *tconfig |= TIMER_ENABLE_BIT;
}

/**
 * @brief Generate a blocking delay using the hardware timer.
 * @param group Timer group (0 or 1)
 * @param timer Timer number within the group (0 or 1)
 * @param ms Delay time in milliseconds
 */
void Timer_Delay(int group, int timer, uint32_t ms)
{
    uint32_t base_address = (group == 0) ? TG0_BASE_ADDR : TG1_BASE_ADDR;
    uint32_t timer_offset = timer * 0x24;

    volatile uint32_t *tconfig = (volatile uint32_t *)(base_address + timer_offset + TIMER_CONFIG_OFFSET);
    volatile uint32_t *talarmlo = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_LO_OFFSET);
    volatile uint32_t *talarmhi = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_HI_OFFSET);
    volatile uint32_t *tloadlo = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_LO_OFFSET);
    volatile uint32_t *tloadhi = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_HI_OFFSET);
    volatile uint32_t *tload = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_OFFSET);

    /* Convert delay time from milliseconds to microsecond ticks */
    uint64_t delay_ticks = ms * 1000;

    /* Reset counter */
    *tloadlo = 0;
    *tloadhi = 0;
    *tload = 1;

    /* Set alarm value */
    *talarmlo = (uint32_t)(delay_ticks & 0xFFFFFFFF);
    *talarmhi = (uint32_t)(delay_ticks >> 32);

    /* Ensure alarm is enabled */
    *tconfig |= TIMER_ALARM_EN;

    /* Busy-wait (with yielding) until the alarm triggers */
    while ((*tconfig & TIMER_ALARM_EN) == TIMER_ALARM_EN)
    {
    }

    /* Re-enable alarm after delay */
    *tconfig |= TIMER_ALARM_EN;
}

/**
 * @brief Timer interrupt service routine.
 * @param arg Pointer to an array containing {group, timer}
 */
static void IRAM_ATTR timerInterruptHandler(void *arg)
{
    int *timerInfo = (int *)arg;
    int group = timerInfo[0];
    int timer = timerInfo[1];

    uint32_t base_address = (group == 0) ? TG0_BASE_ADDR : TG1_BASE_ADDR;
    uint32_t timer_offset = timer * 0x24;
    volatile uint32_t *tconfig = (volatile uint32_t *)(base_address + timer_offset + TIMER_CONFIG_OFFSET);
    volatile uint32_t *tintc = (volatile uint32_t *)(base_address + TIMER_INT_CL_OFFSET);

    /* Mark that the interrupt occurred */
    int index = group * 2 + timer;
    timer_isr_triggered[index] = true;

    /* Re-enable alarm and clear the interrupt */
    *tconfig |= TIMER_ALARM_EN;
    *tintc |= (1 << timer);
}

/**
 * @brief Register the timer interrupt service routine.
 * @param group Timer group (0 or 1)
 * @param timer Timer number within the group (0 or 1)
 * @param intType Interrupt type: TIMER_INT_LEVEL or TIMER_INT_EDGE
 */
void Timer_Isr_Register(int group, int timer, int intType)
{
    intr_handle_t isr_handle = NULL;
    int index = group * 2 + timer;
    int intr_source = 0;

    /* Select the interrupt source based on group, timer, and interrupt type */
    if (group == 0)
    {
        intr_source = (timer == 0) ? ETS_TG0_T0_LEVEL_INTR_SOURCE : ETS_TG0_T1_LEVEL_INTR_SOURCE;
    }
    else
    {
        intr_source = (timer == 0) ? ETS_TG1_T0_LEVEL_INTR_SOURCE : ETS_TG1_T1_LEVEL_INTR_SOURCE;
    }

    esp_err_t err = esp_intr_alloc(
        intr_source,
        ESP_INTR_FLAG_IRAM,
        timerInterruptHandler,
        (void *)&s_timer_info[index],
        &isr_handle);
}
