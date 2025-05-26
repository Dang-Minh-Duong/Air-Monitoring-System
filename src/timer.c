#include "timer_module.h"
#include "esp_intr_alloc.h"
#include "esp_attr.h"         /* For IRAM_ATTR */
#include "soc/interrupts.h"   /* For ETS_* macros; if not defined, we define them below */

/** If ETS_TG*_LEVEL_INTR_SOURCE macros are not already defined, define them here **/
#ifndef ETS_TG0_T0_LEVEL_INTR_SOURCE
#define ETS_TG0_T0_LEVEL_INTR_SOURCE  14
#endif

#ifndef ETS_TG0_T1_LEVEL_INTR_SOURCE
#define ETS_TG0_T1_LEVEL_INTR_SOURCE  15
#endif

#ifndef ETS_TG1_T0_LEVEL_INTR_SOURCE
#define ETS_TG1_T0_LEVEL_INTR_SOURCE  18
#endif

#ifndef ETS_TG1_T1_LEVEL_INTR_SOURCE
#define ETS_TG1_T1_LEVEL_INTR_SOURCE  19
#endif

/** Base addresses for Timer Groups (ESP32) **/
#define TG0_BASE_ADDR 0x3FF5F000
#define TG1_BASE_ADDR 0x3FF60000

/** Register offsets for a given timer (each timer occupies 0x20 bytes) **/
#define TIMER_CONFIG_OFFSET    0x0000
#define TIMER_LO_OFFSET        0x0004
#define TIMER_HI_OFFSET        0x0008
#define TIMER_UPDATE_OFFSET    0x000C
#define TIMER_ALARM_LO_OFFSET  0x0010
#define TIMER_ALARM_HI_OFFSET  0x0014
#define TIMER_LOAD_LO_OFFSET   0x0018
#define TIMER_LOAD_HI_OFFSET   0x001C
#define TIMER_LOAD_OFFSET      0x0020
#define TIMER_INT_EN_OFFSET    0x0098
#define TIMER_INT_CL_OFFSET    0x00A4

/** Timer register bit definitions **/
#define TIMER_ENABLE_BIT       (1UL << 31)  /* Enable the timer counter */
#define TIMER_AUTO_RELOAD_BIT  (1UL << 29)  /* Auto-reload: reset counter when alarm is reached */
#define TIMER_ALARM_EN         (1UL << 10)  /* Enable the alarm */

/** Global array of interrupt flags for four timers **/
volatile bool timer_isr_triggered[4] = { false, false, false, false };

/** Static array to hold timer parameters {group, timer} for each timer **/
static int s_timer_info[4][2] = {
    { 0, 0 },   /* Timer Group 0, Timer 0 */
    { 0, 1 },   /* Timer Group 0, Timer 1 */
    { 1, 0 },   /* Timer Group 1, Timer 0 */
    { 1, 1 }    /* Timer Group 1, Timer 1 */
};

/** 
 * Initialize a hardware timer by directly accessing its registers.
 * Note: The 'arr' parameter (auto-reload value) is of type uint32_t for consistency.
 *
 * @param group            Timer group number (e.g. 0 or 1)
 * @param timer            Timer number within the group (e.g. 0 or 1)
 * @param prescaler        Prescaler value to divide the timer clock
 * @param arr              Auto-reload value (alarm value in ticks)
 * @param enableInterrupt  Interrupt mode flag (e.g., TIMER_INT_LEVEL or TIMER_INT_EDGE)
 **/
void Timer_Init(int group, int timer, uint32_t prescaler, uint32_t arr, int enableInterrupt) {
    uint32_t base_address = (group == 0) ? TG0_BASE_ADDR : TG1_BASE_ADDR;
    uint32_t timer_offset = timer * 0x24;
    
    /** Obtain pointers to the timer registers **/
    volatile uint32_t *tconfig   = (volatile uint32_t *)(base_address + timer_offset + TIMER_CONFIG_OFFSET);
    volatile uint32_t *talarmlo  = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_LO_OFFSET);
    volatile uint32_t *talarmhi  = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_HI_OFFSET);
    volatile uint32_t *tloadlo   = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_LO_OFFSET);
    volatile uint32_t *tloadhi   = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_HI_OFFSET);
    volatile uint32_t *tload     = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_OFFSET);
    volatile uint32_t *tinte     = (volatile uint32_t *)(base_address + timer_offset + TIMER_INT_EN_OFFSET);
    volatile uint32_t *tintc     = (volatile uint32_t *)(base_address + timer_offset + TIMER_INT_CL_OFFSET);
   
    /** Disable the timer before reconfiguring it **/
    *tconfig &= ~TIMER_ENABLE_BIT;
    
    /** Set the prescaler
     *  Clear bits [13-28] first, then set the new prescaler value shifted by 13 bits.
     **/
    *tconfig &= ~(((uint32_t)0xFFFF) << 13);
    *tconfig |= (prescaler << 13);
    
    /** Enable auto-reload mode **/
    *tconfig |= TIMER_AUTO_RELOAD_BIT;
    
    /** Set the alarm (auto-reload) value **/
    *talarmlo = arr;
    *talarmhi = 0;
    
    /** Reset the counter to 0 **/
    *tloadlo = 0;
    *tloadhi = 0;
    *tload   = 1;
    
    /** Configure interrupt settings if interrupts are enabled **/
    if (enableInterrupt == TIMER_INT_LEVEL) {
        *tconfig |= TIMER_ALARM_EN;
        *tinte |= 1 << group;
        *tconfig |= 1 << 11;  /* Level interrupt configuration bit */
    }
    else if (enableInterrupt == TIMER_INT_EDGE) {
        *tconfig |= TIMER_ALARM_EN;
        *tinte |= 1 << group;
        *tconfig |= 1 << 12;  /* Edge interrupt configuration bit */
    } 
    else {
        *tconfig &= ~TIMER_ALARM_EN;
    }
    
    /** Finally, enable the timer **/
    *tconfig |= TIMER_ENABLE_BIT;
}

/** 
 * Use a hardware timer to create a blocking delay.
 * This function assumes each tick equals 1 µs (for example, with prescaler = 80 on an 80MHz clock).
 *
 * @param group  Timer group number (0 or 1)
 * @param timer  Timer number within the group (0 or 1)
 * @param ms     Delay duration in milliseconds
 **/
void Timer_Delay(int group, int timer, uint32_t ms) {
    uint32_t base_address = (group == 0) ? TG0_BASE_ADDR : TG1_BASE_ADDR;
    uint32_t timer_offset = timer * 0x24;
    
    /** Obtain pointers to the required timer registers **/
    volatile uint32_t *tconfig   = (volatile uint32_t *)(base_address + timer_offset + TIMER_CONFIG_OFFSET);
    volatile uint32_t *tupdate   = (volatile uint32_t *)(base_address + timer_offset + TIMER_UPDATE_OFFSET);
    volatile uint32_t *talarmlo  = (volatile uint32_t *)(base_address + timer_offset + TIMER_ALARM_LO_OFFSET);
    volatile uint32_t *tloadlo   = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_LO_OFFSET);
    volatile uint32_t *tloadhi   = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_HI_OFFSET);
    volatile uint32_t *tload     = (volatile uint32_t *)(base_address + timer_offset + TIMER_LOAD_OFFSET);
    
    /** Convert milliseconds to ticks (assuming 1 tick = 1 µs) **/
    uint32_t delay_ticks = ms * 1000;
    
    /** Reset the timer counter to 0 **/
    *tloadlo = 0;
    *tloadhi = 0;
    *tload   = 1;
    
    /** Set a new alarm value based on the desired delay **/
    *talarmlo = delay_ticks;
    
    /** Busy-wait loop: Poll the timer configuration register until the alarm bit is cleared by hardware **/
    while (*tconfig & TIMER_ALARM_EN) {
        *tupdate = 1;
    }
    
    /** Re-enable the alarm after the specified delay has elapsed **/
    *tconfig |= TIMER_ALARM_EN;
}

/** 
 * Timer Interrupt Service Routine (ISR)
 * This function is executed when the timer reaches the alarm value.
 * It is marked with IRAM_ATTR for fast execution from the IRAM.
 *
 * @param arg  Pointer to a timer information array (int[2]) containing {group, timer}
 **/
void IRAM_ATTR timerInterruptHandler(void *arg) {
    int *timer_info = (int *) arg;
    int group = timer_info[0];
    int timer = timer_info[1];
    int index = group * 2 + timer;
    
    /** Calculate the base address and timer offset for the corresponding timer **/
    uint32_t base_address = (group == 0) ? TG0_BASE_ADDR : TG1_BASE_ADDR;
    uint32_t timer_offset = timer * 0x24;
    
    /** Obtain pointers to the timer configuration and interrupt clear registers **/
    volatile uint32_t *tconfig = (volatile uint32_t *)(base_address + timer_offset + TIMER_CONFIG_OFFSET);
    volatile uint32_t *tintc   = (volatile uint32_t *)(base_address + timer_offset + TIMER_INT_CL_OFFSET);

    /** Enter a critical section to safely update the shared interrupt flag **/
    portENTER_CRITICAL_ISR(&timerMux);
      timer_isr_triggered[index] = true;
    portEXIT_CRITICAL_ISR(&timerMux);

    /** Re-enable the alarm to allow subsequent interrupts **/
    *tconfig |= TIMER_ALARM_EN;
    
    /** Clear the timer interrupt flag to indicate that the interrupt has been handled **/
    *tintc |= (1 << group);
}

/** 
 * Register the timer ISR using esp_intr_alloc().
 * The appropriate interrupt source is chosen based on the timer group and timer number.
 *
 * @param group  Timer group number (0 or 1)
 * @param timer  Timer number within the group (0 or 1)
 **/
void Timer_Isr_Register(int group, int timer) {
    intr_handle_t isr_handle = NULL;
    int index = group * 2 + timer;
    int intr_source = 0;
    
    /** Determine the correct interrupt source based on timer group and number **/
    if (group == 0 && timer == 0) {
        intr_source = ETS_TG0_T0_LEVEL_INTR_SOURCE;
    } else if (group == 0 && timer == 1) {
        intr_source = ETS_TG0_T1_LEVEL_INTR_SOURCE;
    } else if (group == 1 && timer == 0) {
        intr_source = ETS_TG1_T0_LEVEL_INTR_SOURCE;
    } else if (group == 1 && timer == 1) {
        intr_source = ETS_TG1_T1_LEVEL_INTR_SOURCE;
    }
    
    /** Allocate the interrupt and register the ISR
     *  ESP_INTR_FLAG_IRAM ensures that the ISR is placed in the IRAM for fast execution.
     **/
    esp_err_t ret = esp_intr_alloc(intr_source, ESP_INTR_FLAG_IRAM, timerInterruptHandler,
                                   (void *)s_timer_info[index], &isr_handle);
}
