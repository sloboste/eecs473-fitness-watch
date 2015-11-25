/* A container to store the current date and time with 1 second resolution.
 */

#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <stdint.h>


typedef struct date_time_struct {
    // Date
    char * day_str;   // 3 chars, null-teminated
    char * month_str; // 3 chars, null-teminated
    uint8_t day_num;
    uint8_t month_num;
    uint8_t year_2digit;
    // Time (1s resolution)
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} date_time_t;

// The current date/time
date_time_t date_time;

/**
 * Initialize the date_time container.
 * 
 * on_minute_hour_change -- the callback function to be called when a change in
 *                          the date/time by an interval of 1 minute or greater
 *                          occurs. Callback will be called by the scheduler. 
 */
extern void date_time_init(void (*on_minute_hour_change)(void));

/**
 * Increment the second field of the date_time struct. 
 */
extern void date_time_increment_second();

#endif