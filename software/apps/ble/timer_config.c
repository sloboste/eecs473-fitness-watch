#include <stdbool.h>

#include "app_timer.h"

#include "timer_config.h"


void timers_init(void)
{
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS,
    APP_TIMER_OP_QUEUE_SIZE, false);
}
