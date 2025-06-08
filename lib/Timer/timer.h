#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize the timer with a prescaler and auto-reload setting.
 * 
 * @param group Timer group (0 or 1)
 * @param timer Timer number (0 or 1)
 * @param prescaler Clock divider (min 2, max 65536)
 */
void Timer_Init(int group, int timer, uint16_t prescaler);

/**
 * @brief Create a blocking delay using a hardware timer.
 * 
 * @param group Timer group (0 or 1)
 * @param timer Timer number (0 or 1)
 * @param ms Delay time in milliseconds
 */
void Timer_Delay(int group, int timer, uint64_t ms);

#endif // TIMER_H
