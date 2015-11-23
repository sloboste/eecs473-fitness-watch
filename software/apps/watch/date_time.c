#include <string.h>
#include <stdbool.h>

#include "date_time.h"
#include "scheduler_config.h"

// Month names
static char * month_strings[12] = {
    "jan", "feb", "mar", "apr", "may", "jun",
    "jul", "aug", "sep", "oct", "nov", "dec"
 };

// Day names
static char * day_strings[7] = {
    "sun", "mon", "tue", "wed", "thu", "fri", "sat"    
};

// Stores the callback
static void (*on_min_hr_change)(void);


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
    year += 2000 - (date_time.month_num < 3);
    day_of_week_num = (year + year/4 - year/100 + year/400 +
                       t[date_time.month_num-1] + date_time.day_num) % 7;
    date_time.day_str = day_strings[day_of_week_num];
    date_time.month_str = month_strings[date_time.month_num-1];
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
    date_time.month_num = 12;
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
            case 1:  // Jan
            case 3:  // Mar
            case 5:  // May 
            case 7:  // July
            case 8:  // Aug
            case 10: // Oct 
                if (++date_time.day_num > 31) {
                    date_time.day_num = 0;
                    ++date_time.month_num;
                }                
                break;
            case 12: // Dec
                if (++date_time.day_num > 31) {
                    date_time.day_num = 0;
                    date_time.month_num = 1; // Jan
                }                
                break;
            case 4:  // Apr
            case 6:  // Jun
            case 9:  // Sep
            case 11: // Nov
                if (++date_time.day_num > 30) {
                    date_time.day_num = 0;
                    ++date_time.month_num;
                }                
                break; 
            case 2: // Feb
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
