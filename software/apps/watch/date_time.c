#include <string.h>
#include <stdbool.h>

#include "date_time.h"
#include "scheduler_config.h"

#include "app_util_platform.h"


static date_time_t current_date_time;
static void (*on_min_hr_change)(void);

void date_time_init(void (*on_minute_hour_change)(void))
{
    on_min_hr_change = on_minute_hour_change;
    memset(&current_date_time, 0, sizeof(current_date_time));
    // FIXME remove
    current_date_time.hours = 23; 
    current_date_time.minutes = 59; 
    current_date_time.seconds = 55; 
    //
}

void date_time_increment_second()
{
    bool minute_hour_change = false;
    CRITICAL_REGION_ENTER();                                                    
    if (++current_date_time.seconds > 59) {                                                  
    current_date_time.seconds -= 60;                                                     
        if (++current_date_time.minutes > 59) {                                              
            current_date_time.minutes -= 60;                                                 
            minute_hour_change = true;
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
