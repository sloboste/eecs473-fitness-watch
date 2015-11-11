/**
 * Configuration file for Noridc's app timer module. This module uses RTC1 and
 * the softdevice and the scheduler need this app timer to be setup before they
 * can be used.
 */

#ifndef __TIMER_CONFIG_H
#define __TIMER_CONFIG_H

// The value to divide the low frequency clock by for RTC1
#define APP_TIMER_PRESCALER         0   // FIXME sched used 16??

// The maximum number of times that will be used in the application
#define APP_TIMER_MAX_TIMERS        6   // FIXME this is arbitrary right now

// Size of the timer operations queue
#define APP_TIMER_OP_QUEUE_SIZE     4   // FIXME this is arbitrary right now

/**
 * Set up the app timer module for the watch application.
 */
extern void timers_init(void);

#endif
