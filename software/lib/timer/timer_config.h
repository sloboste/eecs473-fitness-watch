/**
 * Configuration file for Noridc's app timer module. This module uses RTC1 and
 * the softdevice and the scheduler need this app timer to be setup before they
 * can be used.
 */

#ifndef __TIMER_CONFIG_H
#define __TIMER_CONFIG_H

#include <stdbool.h>

// The value to divide the low frequency clock by for RTC1
#define APP_TIMER_PRESCALER         0   // TODO may need to tune 

// The maximum number of times that will be used in the application
#define APP_TIMER_MAX_TIMERS        6   // FIXME this is arbitrary right now

// Size of the timer operations queue
#define APP_TIMER_OP_QUEUE_SIZE     6   // FIXME this is arbitrary right now

/**
 * Set up the app timer module for the watch application.
 */
extern void timers_init(bool use_scheduler);

/**
 * Start the periodic 1 Hz timer.
 */
extern void timer_start_1hz_periodic();

/**
 * Stop the periodic 1 Hz timer.
 */
extern void timer_stop_1hz_periodic();

/**
 * Start the periodic 10 Hz timer.
 */
extern void timer_start_10hz_periodic();

/**
 * Stop the periodic 10 Hz timer.
 */
extern void timer_stop_10hz_periodic();

/**
 * THIS MUST BE IMPLEMENTED IN THE MAIN FILE
 * Executes in the thread context.
 */
extern void task_1hz(void * arg_ptr);

/**
 * THIS MUST BE IMPLEMENTED IN THE MAIN FILE
 * Executes in the thread context.
 */
extern void task_10hz(void * arg_ptr);

#endif
