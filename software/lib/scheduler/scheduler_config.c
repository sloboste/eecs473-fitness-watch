#include "scheduler_config.h"

#include "app_scheduler.h"                                                      
#include "app_timer_appsh.h"                                                    
#include "nordic_common.h"                                                      
#include "app_util_platform.h"


void scheduler_init()
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}
