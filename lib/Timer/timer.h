#ifndef TIMER_MODULE_H
#define TIMER_MODULE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize a hardware timer by directly accessing its registers.
 *
 * This function configures the specified hardware timer by modifying its registers,
 * setting up the prescaler and the alarm (auto-reload) value.
 *
 * @param group     Timer group number (0 for TG0, 1 for TG1).
 * @param timer     Timer index within the selected group (0 or 1).
 * @param prescaler Prescaler value; e.g., using 80 produces ~1µs ticks with an 80MHz clock.
 * @param arr       Alarm (auto-reload) value in ticks. For example, 500000 corresponds to ~500ms.
 */
void Timer_Init(int group, int timer, uint16_t prescaler, uint64_t arr);

/**
 * @brief Generate a blocking delay using the hardware timer.
 *
 * This function produces a blocking delay by busy-waiting until the timer reaches
 * the specified delay duration. It assumes that one tick corresponds to 1µs.
 *
 * @param group Timer group number (0 or 1).
 * @param timer Timer index within the selected group (0 or 1).
 * @param ms    Delay in milliseconds.
 * 
 */
void Timer_Delay(int group, int timer, uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_MODULE_H */