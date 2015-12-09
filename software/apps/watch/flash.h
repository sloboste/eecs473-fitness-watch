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
 * Wrapper for the Nordic persistent storage module that is specific to the
 * use case for the watch application. Allows for easy loading and storing of
 * the watch data.
 */

#ifndef FLASH_H
#define FLASH_H

#include <stdbool.h>
#include <stdint.h>

#include "date_time.h"


/**
 * Set up the flash memory module.
 */
extern void flash_init();

/**
 * Retrieve the step count stored in flash.
 *
 * step_count_ptr -- pointer to the variable in which to load the step count.
 */
extern void flash_load_step_count(uint32_t * step_count_ptr);

/**
 * Store the current step count into flash.
 *
 * step_count_ptr -- pointer to the variable that holds the step count to store.
 */
extern void flash_store_step_count(uint32_t * step_count_ptr);

/**
 * Retrieve the yesterday step count stored in flash.
 *
 * step_yesterday_ptr -- pointer to the variable in which to load the
 *                       yesterday'sstep count.
 */
extern void flash_load_step_yesterday(uint32_t * step_yesterday_ptr);

/**
 * Store the current yesterday's step count into flash.
 *
 * step_yesterday_ptr -- pointer to the variable that holds the yesterday's
 *                       step count to store.
 */
extern void flash_store_step_yesterday(uint32_t * step_yesterday_ptr);

/**
 * Retrieve the step goal stored in flash.
 *
 * step_goal_str -- string in which to load the step goal.
 */
extern void flash_store_load_goal(char * step_goal_str);

/**
 * Store the current step goal into flash.
 *
 * step_count_str -- the string that holds the step goal to store.
 */
extern void flash_store_step_goal(char * step_goal_str);

/**
 * Retrieve the date_time struct stored in flash.
 *
 * date_time_ptr -- pointer to the struct in which to load the date/time.
 */
extern void flash_load_date_time(date_time_t * date_time_ptr);

/**
 * Store the date_time struct into flash.
 *
 * date_time_ptr -- pointer to the struct that holds the date/time to store.
 */
extern void flash_store_date_time(date_time_t * date_time_ptr);

#endif
