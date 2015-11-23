#include <stdbool.h>

#include "app_timer.h"
#include "app_timer_appsh.h"

#include "timer_config.h"


static app_timer_id_t timer_id_1hz_0;
static app_timer_id_t timer_id_1hz_1;
static app_timer_id_t timer_id_10hz;

void timers_init(bool use_scheduler)
{
    APP_TIMER_INIT(APP_TIMER_PRESCALER,
                   APP_TIMER_MAX_TIMERS,
                   APP_TIMER_OP_QUEUE_SIZE,
                   use_scheduler ? app_timer_evt_schedule : NULL);

    app_timer_create(&timer_id_1hz_0, APP_TIMER_MODE_REPEATED, task_1hz_0);         
    app_timer_create(&timer_id_1hz_1, APP_TIMER_MODE_REPEATED, task_1hz_1);         
    app_timer_create(&timer_id_10hz, APP_TIMER_MODE_REPEATED, task_10hz);       
}


void timer_start_1hz_periodic_0()
{
    app_timer_start(timer_id_1hz_0, APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER),   
                    app_timer_evt_schedule);        
}

void timer_stop_1hz_periodic_0()
{
    app_timer_stop(timer_id_1hz_0); 
}

void timer_start_1hz_periodic_1()
{
    app_timer_start(timer_id_1hz_1, APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER),   
                    app_timer_evt_schedule);        
}

void timer_stop_1hz_periodic_1()
{
    app_timer_stop(timer_id_1hz_1); 
}

void timer_start_10hz_periodic()
{
    app_timer_start(timer_id_10hz, APP_TIMER_TICKS(100, APP_TIMER_PRESCALER),   
                    app_timer_evt_schedule);        
}

void timer_stop_10hz_periodic()
{
    app_timer_stop(timer_id_10hz); 
}
