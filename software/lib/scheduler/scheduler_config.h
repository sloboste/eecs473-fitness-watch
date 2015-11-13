/**
 * Configuration for the Nordic scheduler module.
 */

#ifndef __SCHEDULER_CONFIG_H
#define __SCHEDULER_CONFIG_H

#include "app_timer_appsh.h"
#include "app_scheduler.h"                                                      

#define SCHED_MAX_EVENT_DATA_SIZE   APP_TIMER_SCHED_EVT_SIZE
#define SCHED_QUEUE_SIZE            2  // FIXME Arbitrary number 

/**
 * Setup the scheduler module.
 */
extern void scheduler_init();

#endif
