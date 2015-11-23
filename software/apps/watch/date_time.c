#include <string.h>
#include <stdbool.h>

#include "date_time.h"
#include "scheduler_config.h"

// Stores the callback
static void (*on_min_hr_change)(void);

// TODO use a table instead
/**
 * http://stackoverflow.com/questions/6385190/
 *                  correctness-of-sakamotos-algorithm-to-find-the-day-of-week
 */
static void update_day_month_str()
{
    uint8_t day_of_week_num;
    uint16_t year;   
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4}; 

    year = date_time.year_2digit;   
    year += 2000 - (date_time.month_num < DATE_MAR_NUM);
    day_of_week_num = (year + year/4 - year/100 + year/400 +
                       t[date_time.month_num-1] + date_time.day_num) % 7;
    switch (day_of_week_num) {
        case DATE_SUN_NUM:    
            date_time.day_str = DATE_SUN_STR;
            break; 
        case DATE_MON_NUM:    
            date_time.day_str = DATE_MON_STR;
            break; 
        case DATE_TUE_NUM:    
            date_time.day_str = DATE_TUE_STR;
            break; 
        case DATE_WED_NUM:    
            date_time.day_str = DATE_WED_STR;
            break; 
        case DATE_THU_NUM:    
            date_time.day_str = DATE_THU_STR;
            break; 
        case DATE_FRI_NUM:    
            date_time.day_str = DATE_FRI_STR;
            break; 
        case DATE_SAT_NUM:    
            date_time.day_str = DATE_SAT_STR;
            break; 
    }
   
    switch (date_time.month_num) {
        case DATE_JAN_NUM:    
            date_time.month_str = DATE_JAN_STR;
            break; 
        case DATE_FEB_NUM:    
            date_time.month_str = DATE_FEB_STR;
            break; 
        case DATE_MAR_NUM:    
            date_time.month_str = DATE_MAR_STR;
            break; 
        case DATE_APR_NUM:    
            date_time.month_str = DATE_APR_STR;
            break; 
        case DATE_MAY_NUM:    
            date_time.month_str = DATE_MAY_STR;
            break; 
        case DATE_JUN_NUM:    
            date_time.month_str = DATE_JUN_STR;
            break; 
        case DATE_JUL_NUM:    
            date_time.month_str = DATE_JUL_STR;
            break; 
        case DATE_AUG_NUM:    
            date_time.month_str = DATE_AUG_STR;
            break; 
        case DATE_SEP_NUM:    
            date_time.month_str = DATE_SEP_STR;
            break; 
        case DATE_OCT_NUM:    
            date_time.month_str = DATE_OCT_STR;
            break; 
        case DATE_NOV_NUM:    
            date_time.month_str = DATE_NOV_STR;
            break; 
        case DATE_DEC_NUM:    
            date_time.month_str = DATE_DEC_STR;
            break; 
    }
}

void date_time_init(void (*on_minute_hour_change)(void))
{
    on_min_hr_change = on_minute_hour_change;
    memset(&date_time, 0, sizeof(date_time));
    
    // Default date/time
    date_time.hours = 23; 
    date_time.minutes = 59; 
    date_time.seconds = 55; 
    date_time.day_num = 9;
    date_time.month_num = DATE_DEC_NUM;
    date_time.year_2digit = 15;
    update_day_month_str();
}

void date_time_increment_second()
{
    uint8_t max_days_feb = 29;
    bool day_update = false;
    if (++date_time.seconds > 59) {                                                  
    date_time.seconds = 0;                                                     
        if (++date_time.minutes > 59) {                                              
            date_time.minutes = 0;                                                 
            if (++date_time.hours > 23) {                                            
                date_time.hours = 0;                                               
                day_update = true;
            }                                                                   
        }                                                                       
    }                                                                           
    if (day_update) {
        switch (date_time.month_num) {
            case DATE_JAN_NUM:
            case DATE_MAR_NUM:
            case DATE_MAY_NUM:
            case DATE_JUL_NUM:
            case DATE_AUG_NUM:
            case DATE_OCT_NUM:
                if (++date_time.day_num > 31) {
                    date_time.day_num = 0;
                    ++date_time.month_num;
                }                
                break;
            case DATE_DEC_NUM:
                if (++date_time.day_num > 31) {
                    date_time.day_num = 0;
                    date_time.month_num = DATE_JAN_NUM;
                }                
                break;
            case DATE_APR_NUM:
            case DATE_JUN_NUM:
            case DATE_SEP_NUM:
            case DATE_NOV_NUM:
                if (++date_time.day_num > 30) {
                    date_time.day_num = 0;
                    ++date_time.month_num;
                }                
                break; 
            case DATE_FEB_NUM:
                if (date_time.year_2digit % 4) { // Leap year
                    max_days_feb = 28;
                }
                if (++date_time.day_num > max_days_feb) {
                    date_time.day_num = 0;
                    ++date_time.month_num;
                }                
                break; 
        }
        update_day_month_str();
    }
    on_min_hr_change();
}
