/**
 * @file timer.c
 * @brief Implementation of hardware timer functions for ESP32 in pure C.
 *
 * This file implements functions used to initialize and use the ESP32 hardware timers.
 * It provides functionality for setting up a timer with a given prescaler, alarm (ARR) value,
 * and interrupt mode (interrupt disabled, edge-triggered, or level-triggered). The timer is
 * configured in auto-reload mode so that it resets automatically when reaching the alarm.
 */

#include "timer.h"

/**
 * @brief Initializes a hardware timer with user-specified settings.
 *
 * This function configures a hardware timer in the specified timer group and timer index 
 * with given prescaler, alarm (ARR) value, and interrupt mode. The timer runs in auto-reload 
 * mode so that when the counter reaches the alarm value, it automatically resets.
 * For the interrupt parameter:
 *   - TIMER_INT_DISABLE (0): The alarm interrupt function is disabled.
 *   - TIMER_INT_EDGE (1): Alarm interrupt is enabled and configured as edge-triggered.
 *   - TIMER_INT_LEVEL (2): Alarm interrupt is enabled and configured as level-triggered.
 *
 * @param group Timer group number (0 for TG0, 1 for TG1).
 * @param timer Timer index (0 or 1) within the selected timer group.
 * @param prescaler Prescaler value for the timer; e.g., a value of 80 gives 1µs per tick with an 80MHz clock.
 * @param arr Alarm value in ticks; for instance, 500000 corresponds to ~500ms delay if tick = 1µs.
 * @param enableInterrupt Interrupt configuration mode (0, 1, or 2).
 *
 * @note To actually use interrupts, you must install an ISR and configure the interrupt controller accordingly.
 */
void timer_hw_init(int group, int timer, uint32_t prescaler, uint32_t arr, int enableInterrupt)
{
    /* Determine the base address for the selected timer group. */
    uint32_t base_address = (group == 0) ? TG0_BASE_ADDR : TG1_BASE_ADDR;
    /* Calculate the offset for the selected timer (each timer occupies 0x20 bytes). */
    uint32_t timer_offset = timer * 0x20;
    
    /* Create pointers to the timer registers. */
    volatile uint32_t *tconfig   = (volatile uint32_t *)(base_address + timer_offset + TIMER_CONFIG_OFFSET);
    volatile uint32_t *tlo       = (volatile uint32_t *)(base_address + timer_offset + TIMER_LO_OFFSET);
    volatile uint32_t *thi       = (volatile uint32_t *)(base_address + timer_offset + TIMER_HI_OFFSET);
    volatile uint32_t *tupdate   = (volatile uint32_t *)(base_address + timer_offset + TIMER_UPDATE_OFFSET);
    volatile uint32_t *talarmlo  = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_LO_OFFSET);
    volatile uint32_t *talarmhi  = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_HI_OFFSET);
    volatile uint32_t *tloadlo   = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_LO_OFFSET);
    volatile uint32_t *tloadhi   = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_HI_OFFSET);
    volatile uint32_t *tload     = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_OFFSET);
    
    /** Disable the timer before configuration. */
    *tconfig &= ~TIMER_ENABLE_BIT;
    
    /** Configure the prescaler.
     *  Clear bits [13,28] then set the prescaler value.
     */
    *tconfig &= ~(((uint32_t)0xFFFF) << 13);
    *tconfig |= (prescaler << 13);
    
    /** Enable auto-reload mode so that the timer counter resets automatically upon reaching the alarm. */
    *tconfig |= TIMER_AUTO_RELOAD_BIT;
    
    /** Set the alarm value (ARR). */
    *talarmlo = arr;
    *talarmhi = 0;
    
    /** Reset the timer counter by loading 0. */
    *tloadlo = 0;
    *tloadhi = 0;
    *tload   = 1;
    
    /** 
     * Configure the alarm interrupt settings according to enableInterrupt.
     * For TIMER_INT_EDGE and TIMER_INT_LEVEL, the alarm function is enabled.
     * For TIMER_INT_DISABLE, the alarm interrupt function is disabled.
     */
    if (enableInterrupt == TIMER_INT_EDGE || enableInterrupt == TIMER_INT_LEVEL) {
        *tconfig |= TIMER_ALARM_EN;
        if (enableInterrupt == TIMER_INT_EDGE) {
            /* Additional configuration for edge-triggered interrupt can be added here. */
        } else if (enableInterrupt == TIMER_INT_LEVEL) {
            /* Additional configuration for level-triggered interrupt can be added here. */
        }
    } else {
        *tconfig &= ~TIMER_ALARM_EN;
    }
    
    /** Finally, enable the timer. */
    *tconfig |= TIMER_ENABLE_BIT;
}

/**
 * @brief Creates a blocking delay using the hardware timer.
 *
 * This function utilizes the specified timer to implement a delay of a given duration (in milliseconds).
 * It sets the delay (by adjusting the alarm value based on the tick time of 1µs) and then waits 
 * until the hardware automatically clears the TIMER_ALARM_EN bit (indicating the alarm event).
 * Once the alarm triggers, the alarm function is re-enabled for subsequent delays.
 *
 * @param group Timer group number (0 for TG0, 1 for TG1).
 * @param timer Timer index (0 or 1) within the selected timer group.
 * @param ms Delay duration in milliseconds.
 *
 * @note This function assumes that the timer has been configured for 1µs tick (e.g., prescaler = 80 for an 80MHz clock).  
 *       For interrupt-based operation, further modifications are required.
 */
void timerDelay(int group, int timer, uint32_t ms)
{
    /* Determine the base address based on the timer group. */
    uint32_t base_address = (group == 0) ? TG0_BASE_ADDR : TG1_BASE_ADDR;
    /* Compute the timer register block offset. */
    uint32_t timer_offset = timer * 0x20;
    
    /* Create pointers to relevant timer registers. */
    volatile uint32_t *tconfig   = (volatile uint32_t *)(base_address + timer_offset + TIMER_CONFIG_OFFSET);
    volatile uint32_t *tupdate   = (volatile uint32_t *)(base_address + timer_offset + TIMER_UPDATE_OFFSET);
    volatile uint32_t *talarmlo  = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_LO_OFFSET);
    volatile uint32_t *tloadlo   = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_LO_OFFSET);
    volatile uint32_t *tloadhi   = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_HI_OFFSET);
    volatile uint32_t *tload     = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_OFFSET);
    
    /** Calculate delay ticks (assuming 1µs per tick). */
    uint32_t delay_ticks = ms * 1000;
    
    /** Reset the timer counter to zero. */
    *tloadlo = 0;
    *tloadhi = 0;
    *tload   = 1;
    
    /** Set the new alarm value to correspond to the delay duration. */
    *talarmlo = delay_ticks;
    
    /** Ensure the timer is enabled. */
    *tconfig |= TIMER_ENABLE_BIT;
    
    /** Busy-wait until the timer alarm triggers.
     *  The hardware automatically clears the TIMER_ALARM_EN bit when the alarm occurs.
     */
    while (*tconfig & TIMER_ALARM_EN) {
        *tupdate = 1;  /* Continuously update the timer latch with the current counter value. */
    }
    
    /** Re-enable the alarm for subsequent delay operations. */
    *tconfig |= TIMER_ALARM_EN;
}
