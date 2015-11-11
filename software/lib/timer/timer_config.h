/*  Configuration file for timers.
 */

#ifndef __TIMER_CONFIG_H
#define __TIMER_CONFIG_H

// TODO explain these parameters
#define APP_TIMER_PRESCALER         0
#define APP_TIMER_MAX_TIMERS        6
#define APP_TIMER_OP_QUEUE_SIZE     4
// ---

extern void timers_init(void);

#endif
