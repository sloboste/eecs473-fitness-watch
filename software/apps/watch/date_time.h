/*
 */

#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <stdint.h>

#define DATE_JAN_NUM 1
#define DATE_FEB_NUM 2
#define DATE_MAR_NUM 3
#define DATE_APR_NUM 4
#define DATE_MAY_NUM 5
#define DATE_JUN_NUM 6
#define DATE_JUL_NUM 7
#define DATE_AUG_NUM 8
#define DATE_SEP_NUM 9
#define DATE_OCT_NUM 10
#define DATE_NOV_NUM 11
#define DATE_DEC_NUM 12

#define DATE_JAN_STR "jan"
#define DATE_FEB_STR "feb"
#define DATE_MAR_STR "mar"
#define DATE_APR_STR "apr"
#define DATE_MAY_STR "may"
#define DATE_JUN_STR "jun"
#define DATE_JUL_STR "jul"
#define DATE_AUG_STR "aug"
#define DATE_SEP_STR "sep"
#define DATE_OCT_STR "oct"
#define DATE_NOV_STR "nov"
#define DATE_DEC_STR "dec"

#define DATE_SUN_NUM 0
#define DATE_MON_NUM 1
#define DATE_TUE_NUM 2
#define DATE_WED_NUM 3
#define DATE_THU_NUM 4
#define DATE_FRI_NUM 5
#define DATE_SAT_NUM 6

#define DATE_SUN_STR "sun" 
#define DATE_MON_STR "mon"
#define DATE_TUE_STR "tue"
#define DATE_WED_STR "wed"
#define DATE_THU_STR "thu"
#define DATE_FRI_STR "fri"
#define DATE_SAT_STR "sat"

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

/**
 *
 */
extern void date_time_init(void (*on_minute_hour_change)(void));

/**
 *
 */
extern void date_time_increment_second();

/**
 *
 */
extern void date_time_get_current_date_time(date_time_t * dt_ptr);

/**
 *
 */
extern void date_time_set_current_date_time(date_time_t * dt_ptr);

#endif
