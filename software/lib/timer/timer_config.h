/* This file is part of eecs473-fitness-watch.
 *   
 * The code / board schematics created by the authors of eecs473-fitness-watch
 * are free software/hardware: you can redistribute them and/or modify them
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * The code / board schematics are distributed in the hope that they will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the code / board schematics.  If not, see <http://www.gnu.org/licenses/>.
 *
 * All code / schematics not created by the authors of this repository fall
 * under their original licenses.
 *
 *
 * Configuration file for Noridc's app timer module. This module uses RTC1 and
 * the softdevice and the scheduler need this app timer to be setup before they
 * can be used.
 */

#ifndef TIMER_CONFIG_H
#define TIMER_CONFIG_H

#include <stdbool.h>


// The value to divide the low frequency clock by for RTC1
#define APP_TIMER_PRESCALER         0   // TODO may need to tune 

// The maximum number of times that will be used in the application
#define APP_TIMER_MAX_TIMERS        6   // TODO may need to tune

// Size of the timer operations queue
#define APP_TIMER_OP_QUEUE_SIZE     6   // TODO may need to tune


/**
 * Set up the app timer module for the watch application.
 */
extern void timers_init(bool use_scheduler);

/**
 * Start the periodic 1 Hz timer 0.
 */
extern void timer_start_1hz_periodic_0();

/**
 * Stop the periodic 1 Hz timer 0.
 */
extern void timer_stop_1hz_periodic_0();

/**
 * Start the periodic 1 Hz timer 1.
 */
extern void timer_start_1hz_periodic_1();

/**
 * Stop the periodic 1 Hz timer 1.
 */
extern void timer_stop_1hz_periodic_1();

/**
 * Start the periodic 10 Hz timer 0.
 */
extern void timer_start_10hz_periodic();

/**
 * Stop the periodic 10 Hz timer 1.
 */
extern void timer_stop_10hz_periodic();

/**
 * THIS MUST BE IMPLEMENTED IN THE MAIN FILE
 * Executes in the thread context.
 */
extern void task_1hz_0(void * arg_ptr);

/**
 * THIS MUST BE IMPLEMENTED IN THE MAIN FILE
 * Executes in the thread context.
 */
extern void task_1hz_1(void * arg_ptr);

/**
 * THIS MUST BE IMPLEMENTED IN THE MAIN FILE
 * Executes in the thread context.
 */
extern void task_10hz(void * arg_ptr);

#endif
