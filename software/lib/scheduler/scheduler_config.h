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
 * Configuration for the Nordic scheduler module.
 */

#ifndef __SCHEDULER_CONFIG_H
#define __SCHEDULER_CONFIG_H

#include "app_timer_appsh.h"
#include "app_scheduler.h"                                                      

#define SCHED_MAX_EVENT_DATA_SIZE   APP_TIMER_SCHED_EVT_SIZE
#define SCHED_QUEUE_SIZE            6  // FIXME Arbitrary number 

/**
 * Setup the scheduler module.
 */
extern void scheduler_init();

#endif
