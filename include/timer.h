#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** 
 * @file timer.h
 * @brief Definitions and prototypes for configuring and using the ESP32 hardware timer.
 *
 * This header provides macros, register offsets, and function prototypes for initializing
 * a hardware timer and creating delays based on that timer. The timer can be configured with
 * a prescaler, an alarm (ARR) value, and interrupt mode (disabled, edge-triggered, or level-triggered).
 */

/** Base addresses for Timer Groups. */
#define TG0_BASE_ADDR 0x3FF5F000  /**< Base address for Timer Group 0 */
#define TG1_BASE_ADDR 0x3FF60000  /**< Base address for Timer Group 1 */

/** Timer register offsets for each timer block (each block is 0x20 bytes). */
#define TIMER_CONFIG_OFFSET    0x0000  /**< Offset for configuration register */
#define TIMER_LO_OFFSET        0x0004  /**< Offset for low 32 bits of counter */
#define TIMER_HI_OFFSET        0x0008  /**< Offset for high 32 bits of counter */
#define TIMER_UPDATE_OFFSET    0x000C  /**< Offset for updating the counter latch */
#define TIMER_ALARM_LO_OFFSET  0x0010  /**< Offset for low 32 bits of alarm register */
#define TIMER_ALARM_HI_OFFSET  0x0014  /**< Offset for high 32 bits of alarm register */
#define TIMER_LOAD_LO_OFFSET   0x0018  /**< Offset for low 32 bits of load register */
#define TIMER_LOAD_HI_OFFSET   0x001C  /**< Offset for high 32 bits of load register */
#define TIMER_LOAD_OFFSET      0x0020  /**< Offset for load command register */

/** Timer configuration bits. */
#define TIMER_ENABLE_BIT       (1UL << 31)  /**< Bit to enable the timer counter */
#define TIMER_AUTO_RELOAD_BIT  (1UL << 29)  /**< Bit to enable auto-reload mode;
                                               counter resets automatically upon reaching the alarm value */
/** Alarm enable bit.
 *
 * When set, the timer alarm is enabled and will generate an alarm event.
 * Note: This bit is automatically cleared by the hardware when the alarm triggers.
 */
#define TIMER_ALARM_EN         (1UL << 10)

/** Interrupt configuration modes for the timer alarm. */
#define TIMER_INT_DISABLE 0    /**< Disable interrupt: alarm function will not generate an interrupt */
#define TIMER_INT_EDGE    1    /**< Enable edge-triggered interrupt for the alarm */
#define TIMER_INT_LEVEL   2    /**< Enable level-triggered interrupt for the alarm */

/**
 * @brief Initializes a hardware timer with specified settings.
 *
 * This function configures a hardware timer in the specified timer group/index with
 * a given prescaler, alarm value (ARR), and interrupt mode. The timer is configured to
 * run in auto-reload mode so that the counter resets automatically upon an alarm event.
 *
 * @param group Timer group (0 for TG0, 1 for TG1).
 * @param timer Timer index within the group (usually 0 or 1).
 * @param prescaler The prescaler value used to adjust the timer tick duration.
 *                  For an 80MHz clock, a prescaler of 80 gives a tick period of 1µs.
 * @param arr Alarm value (in ticks). For example, 500000 corresponds to a ~500ms delay if each tick is 1µs.
 * @param enableInterrupt Interrupt configuration mode:
 *                        - TIMER_INT_DISABLE: interrupt is disabled.
 *                        - TIMER_INT_EDGE: edge-triggered interrupt is enabled.
 *                        - TIMER_INT_LEVEL: level-triggered interrupt is enabled.
 *
 * @note For interrupt-based operation, an ISR must be installed and the external interrupt controller
 *       must be configured accordingly.
 */
void timer_hw_init(int group, int timer, uint32_t prescaler, uint32_t arr, int enableInterrupt);

/**
 * @brief Generates a blocking delay using the hardware timer.
 *
 * This function sets the timer's alarm value to achieve the desired delay in milliseconds and then
 * busy-waits until the alarm event occurs (i.e. the TIMER_ALARM_EN bit is automatically cleared by hardware).
 * After the alarm event, the alarm bit is re-enabled for subsequent delay operations.
 *
 * @param group Timer group (0 for TG0, 1 for TG1).
 * @param timer Timer index (0 or 1) within the specified group.
 * @param ms Delay duration in milliseconds.
 *
 * @note It is assumed that the timer has been configured with a prescaler that produces a tick period of 1µs.
 */
void timerDelay(int group, int timer, uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif  // TIMER_H
