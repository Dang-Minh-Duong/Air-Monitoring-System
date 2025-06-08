#include "timer.h"
#include "timer_regs.h"  

/**
 * @brief Structure holding pointers to the timer's registers.
 */
typedef struct {
  volatile uint32_t* tconfig;
  volatile uint32_t* tcountlo;
  volatile uint32_t* tcounthi;
  volatile uint32_t* talarmlo;
  volatile uint32_t* talarmhi;
  volatile uint32_t* tloadlo;
  volatile uint32_t* tloadhi;
  volatile uint32_t* tload;
} timer_ptrs_t;

/**
 * @brief Get register pointers for a specific timer.
 * 
 * @param group Timer group (0 or 1)
 * @param timer Timer number within the group (0 or 1)
 * @return timer_ptrs_t Struct with pointers to timer registers
 */
timer_ptrs_t getTimerPointers(int group, int timer) {
  timer_ptrs_t ptrs;
  uint32_t base_address = (group == 0) ? TG0_BASE_ADDR : TG1_BASE_ADDR;
  uint32_t timer_offset = timer * 0x24;

  ptrs.tconfig   = (volatile uint32_t*)(base_address + timer_offset + TIMER_CONFIG_OFFSET);
  ptrs.tcountlo  = (volatile uint32_t*)(base_address + timer_offset + TIMER_COUNT_LO_OFFSET);
  ptrs.tcounthi  = (volatile uint32_t*)(base_address + timer_offset + TIMER_COUNT_HI_OFFSET);
  ptrs.talarmlo  = (volatile uint32_t*)(base_address + timer_offset + TIMER_ALARM_LO_OFFSET);
  ptrs.talarmhi  = (volatile uint32_t*)(base_address + timer_offset + TIMER_ALARM_HI_OFFSET);
  ptrs.tloadlo   = (volatile uint32_t*)(base_address + timer_offset + TIMER_LOAD_LO_OFFSET);
  ptrs.tloadhi   = (volatile uint32_t*)(base_address + timer_offset + TIMER_LOAD_HI_OFFSET);
  ptrs.tload     = (volatile uint32_t*)(base_address + timer_offset + TIMER_LOAD_OFFSET);

  return ptrs;
}

/**
 * @brief Initialize the timer with a prescaler and auto-reload setting.
 * 
 * @param group Timer group (0 or 1)
 * @param timer Timer number (0 or 1)
 * @param prescaler Clock divider 
 */
void Timer_Init(int group, int timer, uint16_t prescaler) {
  timer_ptrs_t t = getTimerPointers(group, timer);

  /** Disable the timer before configuration */
  *t.tconfig &= ~TIMER_ENABLE_BIT;

  /** Set prescaler (bits 13–28) */
  *t.tconfig &= ~(TIMER_PRESCALER_MASK << TIMER_PRESCALER_SHIFT);
  *t.tconfig |= ((prescaler & TIMER_PRESCALER_MASK) << TIMER_PRESCALER_SHIFT);

  /** Configure auto-reload behavior */
  *t.tconfig |= TIMER_AUTO_RELOAD_BIT;

  /** Enable the timer */
  *t.tconfig |= TIMER_ENABLE_BIT;
}

/**
 * @brief Create a blocking delay using a hardware timer.
 * 
 * @param group Timer group (0 or 1)
 * @param timer Timer number (0 or 1)
 * @param ms Delay time in milliseconds
 */
void Timer_Delay(int group, int timer, uint64_t ms) {
  timer_ptrs_t t = getTimerPointers(group, timer);

  /** Read prescaler from config register */
  uint32_t prescaler = ((*t.tconfig >> TIMER_PRESCALER_SHIFT) & TIMER_PRESCALER_MASK);

  /** Calculate delay ticks from milliseconds */
  uint64_t delay_ticks = ms * (80000000UL / prescaler)) / 1000;

  /** Load counter with zero */
  *t.tloadlo = 0;
  *t.tloadhi = 0;
  *t.tload = 1;

  /** Set alarm value */
  *t.talarmlo = (uint32_t)(delay_ticks & 0xFFFFFFFF);
  *t.talarmhi = (uint32_t)(delay_ticks >> 32);

  /** Enable the alarm */
  *t.tconfig |= TIMER_ALARM_EN;

  /** Wait until the alarm triggers (polling TIMER_ALARM_EN bit) */
  while ((*t.tconfig & TIMER_ALARM_EN) == TIMER_ALARM_EN) {
  }
  
  /** Re-enable alarm for future use */
  *t.tconfig |= TIMER_ALARM_EN;
}
