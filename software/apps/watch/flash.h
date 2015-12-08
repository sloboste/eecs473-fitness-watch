/* Wrapper for the Nordic persistent storage module that is specific to the
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
