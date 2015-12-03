/*
 *
 */

#ifndef FLASH_H
#define FLASH_H

#include <stdbool.h>
#include <stdint.h>

#include "date_time.h"


/**
 *
 */
extern void flash_init();

/**
 *
 */
extern void flash_load_step_count(uint32_t * step_count_ptr);

/**
 *
 */
extern void flash_store_step_count(uint32_t * step_count_ptr);

/**
 *
 */
extern void flash_load_step_yesterday(uint32_t * step_yesterday);

/**
 *
 */
extern void flash_store_step_yesterday(uint32_t * step_yesterday);

/**
 *
 */
extern void flash_store_load_goal(char * step_goal_ptr);

/**
 *
 */
extern void flash_store_step_goal(char * step_goal_ptr);

/**
 *
 */
extern void flash_load_date_time(date_time_t * date_time_ptr);

/**
 *
 */
extern void flash_store_date_time(date_time_t * date_time_ptr);

#endif
