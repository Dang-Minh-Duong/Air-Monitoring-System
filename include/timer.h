#ifndef TIMER_MODULE_H
#define TIMER_MODULE_H

#include <stdint.h>
#include <stdbool.h>

/* Interrupt mode definitions */
#define TIMER_INT_DISABLE 0    /* Disable interrupt */
#define TIMER_INT_EDGE    1    /* Edge-triggered interrupt */
#define TIMER_INT_LEVEL   2    /* Level-triggered interrupt */

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * Global array of interrupt flags for four timers.
 * Mapping:
 *   index 0: Timer Group 0, Timer 0
 *   index 1: Timer Group 0, Timer 1
 *   index 2: Timer Group 1, Timer 0
 *   index 3: Timer Group 1, Timer 1
 */
extern volatile bool timer_isr_triggered[4];

/* 
 * @brief Initialize a hardware timer by accessing its registers directly.
 *
 * @param group         Timer group number (0 for TG0, 1 for TG1)
 * @param timer         Timer index within the selected group (0 or 1)
 * @param prescaler     Prescaler value; for example, 80 will produce ~1µs ticks with an 80MHz clock.
 * @param arr           Alarm (auto-reload) value in ticks. For instance, 500000 ticks is ~500ms.
 * @param enableInterrupt Interrupt configuration mode (TIMER_INT_DISABLE, TIMER_INT_EDGE, or TIMER_INT_LEVEL).
 */
void timer_hw_init(int group, int timer, uint32_t prescaler, uint32_t arr, int enableInterrupt);

/* 
 * @brief Produce a blocking delay using the specified timer.
 *
 * @param group Timer group number (0 or 1)
 * @param timer Timer index within the group (0 or 1)
 * @param ms    Delay in milliseconds. Assumes each tick is 1µs.
 */
void timerDelay(int group, int timer, uint32_t ms);

/* 
 * @brief Register an interrupt service routine (ISR) for the specified timer.
 *
 * This function uses esp_intr_alloc() to register the ISR, and associates an internal data structure
 * holding the timer group and timer index with the ISR callback.
 *
 * @param group Timer group (0 or 1)
 * @param timer Timer index (0 or 1)
 */
void register_timer_isr(int group, int timer);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_MODULE_H */
