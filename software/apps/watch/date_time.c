#include <string.h>
#include <stdbool.h>

#include "date_time.h"
#include "scheduler_config.h"

#include "app_util_platform.h"


static date_time_t current_date_time;
static void (*on_min_hr_change)(void);

/**
 * http://stackoverflow.com/questions/6385190/
 *                  correctness-of-sakamotos-algorithm-to-find-the-day-of-week
 */
static void update_day_month_str()
{
    uint8_t day_of_week_num;
    char * day_of_week_str;
    char * month_str;
    uint8_t day;
    uint8_t month;
    uint16_t year;   
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4}; 

    CRITICAL_REGION_ENTER();                                                    
    day = current_date_time.day_num;
    month = current_date_time.month_num;
    year = current_date_time.year_2digit;   
    CRITICAL_REGION_EXIT();                                                    

    year += 2000 - (month < DATE_MAR_NUM);
    day_of_week_num = (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
    switch (day_of_week_num) {
        case DATE_SUN_NUM:    
            day_of_week_str = DATE_SUN_STR;
            break; 
        case DATE_MON_NUM:    
            day_of_week_str = DATE_MON_STR;
            break; 
        case DATE_TUE_NUM:    
            day_of_week_str = DATE_TUE_STR;
            break; 
        case DATE_WED_NUM:    
            day_of_week_str = DATE_WED_STR;
            break; 
        case DATE_THU_NUM:    
            day_of_week_str = DATE_THU_STR;
            break; 
        case DATE_FRI_NUM:    
            day_of_week_str = DATE_FRI_STR;
            break; 
        case DATE_SAT_NUM:    
            day_of_week_str = DATE_SAT_STR;
            break; 
    }
   
    switch (month) {
        case DATE_JAN_NUM:    
            month_str = DATE_JAN_STR;
            break; 
        case DATE_FEB_NUM:    
            month_str = DATE_FEB_STR;
            break; 
        case DATE_MAR_NUM:    
            month_str = DATE_MAR_STR;
            break; 
        case DATE_APR_NUM:    
            month_str = DATE_APR_STR;
            break; 
        case DATE_MAY_NUM:    
            month_str = DATE_MAY_STR;
            break; 
        case DATE_JUN_NUM:    
            month_str = DATE_JUN_STR;
            break; 
        case DATE_JUL_NUM:    
            month_str = DATE_JUL_STR;
            break; 
        case DATE_AUG_NUM:    
            month_str = DATE_AUG_STR;
            break; 
        case DATE_SEP_NUM:    
            month_str = DATE_SEP_STR;
            break; 
        case DATE_OCT_NUM:    
            month_str = DATE_OCT_STR;
            break; 
        case DATE_NOV_NUM:    
            month_str = DATE_NOV_STR;
            break; 
        case DATE_DEC_NUM:    
            month_str = DATE_DEC_STR;
            break; 
    }
     
    
    CRITICAL_REGION_ENTER();                                                    
    current_date_time.day_str = day_of_week_str; 
    current_date_time.month_str = month_str; 
    CRITICAL_REGION_EXIT();                                                    
}

void date_time_init(void (*on_minute_hour_change)(void))
{
    on_min_hr_change = on_minute_hour_change;
    memset(&current_date_time, 0, sizeof(current_date_time));
    // FIXME remove
    current_date_time.hours = 23; 
    current_date_time.minutes = 59; 
    current_date_time.seconds = 55; 
    current_date_time.day_num = 10;
    current_date_time.month_num = DATE_DEC_NUM;
    current_date_time.year_2digit = 15;
    update_day_month_str();
    //
}

void date_time_increment_second()
{
    CRITICAL_REGION_ENTER();                                                    
    if (++current_date_time.seconds > 59) {                                                  
    current_date_time.seconds -= 60;                                                     
        if (++current_date_time.minutes > 59) {                                              
            current_date_time.minutes -= 60;                                                 
            if (++current_date_time.hours > 23) {                                            
                current_date_time.hours -= 24;                                               
            }                                                                   
        }                                                                       
    }                                                                           
    CRITICAL_REGION_EXIT(); 
    on_min_hr_change();
}

void date_time_get_current_date_time(date_time_t * dt_ptr)
{
    CRITICAL_REGION_ENTER();                                                    
    memcpy(dt_ptr, &current_date_time, sizeof(current_date_time));
    CRITICAL_REGION_EXIT(); 
}

void date_time_set_current_date_time(date_time_t * dt_ptr)
{
    CRITICAL_REGION_ENTER();                                                    
    memcpy(&current_date_time, dt_ptr, sizeof(current_date_time));
    CRITICAL_REGION_EXIT(); 
}
