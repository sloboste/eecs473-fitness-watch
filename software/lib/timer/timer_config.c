#include <stdbool.h>

#include "app_timer.h"
#include "app_timer_appsh.h"

#include "timer_config.h"


void timers_init(bool use_scheduler)
{
    APP_TIMER_INIT(APP_TIMER_PRESCALER,
                   APP_TIMER_MAX_TIMERS,
                   APP_TIMER_OP_QUEUE_SIZE,
                   use_scheduler ? app_timer_evt_schedule : NULL);
}
