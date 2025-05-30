#ifndef TIMER_MODULE_H
#define TIMER_MODULE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @brief Interrupt mode definitions.
 *
 * TIMER_INT_DISABLE : Disable interrupt.
 * TIMER_INT_LEVEL   : Use a level-triggered interrupt.
 */
#define TIMER_INT_DISABLE 0
#define TIMER_INT_LEVEL   1

/** 
 * @brief Global array of interrupt flags for four timers.
 *
 * Mapping:
 *   - Index 0: Timer Group 0, Timer 0
 *   - Index 1: Timer Group 0, Timer 1
 *   - Index 2: Timer Group 1, Timer 0
 *   - Index 3: Timer Group 1, Timer 1
 */
extern volatile bool timer_isr_triggered[4];

/** 
 * @brief Initialize a hardware timer by directly accessing its registers.
 *
 * This function configures the specified hardware timer by modifying its registers,
 * setting up the prescaler, alarm value (auto-reload), and interrupt mode as required.
 *
 * @param group         Timer group number (0 for TG0, 1 for TG1).
 * @param timer         Timer index within the selected group (0 or 1).
 * @param prescaler     Prescaler value; for example, 80 produces ~1µs ticks with an 80MHz clock.
 * @param arr           Alarm (auto-reload) value in ticks. For instance, 500000 ticks corresponds to ~500ms.
 * @param enableInterrupt Interrupt configuration mode, choosing one of TIMER_INT_DISABLE,
 *                        TIMER_INT_EDGE, or TIMER_INT_LEVEL.
 */
void Timer_Init(int group, int timer, uint32_t prescaler, uint64_t arr, int enableInterrupt);

/** 
 * @brief Create a blocking delay using the specified timer.
 *
 * This function produces a blocking delay by busy-waiting until the timer
 * reaches the specified delay duration. It assumes that one tick corresponds to 1µs.
 *
 * @param group Timer group number (0 or 1).
 * @param timer Timer index within the group (0 or 1).
 * @param ms    Delay in milliseconds.
 */
void Timer_Delay(int group, int timer, uint32_t ms);

/** 
 * @brief Register an interrupt service routine (ISR) for the specified timer.
 *
 * This function registers the timer’s ISR using esp_intr_alloc() and associates an internal data structure,
 * which holds the timer group and timer index, with the ISR callback.
 *
 * @param group Timer group (0 or 1).
 * @param timer Timer index within the group (0 or 1).
 */
void Timer_Isr_Register(int group, int timer, int intType);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_MODULE_H */