/* This file is part of eecs473-fitness-watch.
 *   
 * The code / board schematics created by the authors of eecs473-fitness-watch
 * are free software/hardware: you can redistribute them and/or modify them
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * The code / board schematics are distributed in the hope that they will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the code / board schematics.  If not, see <http://www.gnu.org/licenses/>.
 *
 * All code / schematics not created by the authors of this repository fall
 * under their original licenses.
 */

#include <stdbool.h>

#include "app_timer.h"
#include "app_timer_appsh.h"
#include "app_error.h"

#include "timer_config.h"


static app_timer_id_t timer_id_1hz_0;
static app_timer_id_t timer_id_1hz_1;
static app_timer_id_t timer_id_10hz;

void timers_init(bool use_scheduler)
{
    uint32_t error_code;

    APP_TIMER_INIT(APP_TIMER_PRESCALER,
                   APP_TIMER_MAX_TIMERS,
                   APP_TIMER_OP_QUEUE_SIZE,
                   use_scheduler ? app_timer_evt_schedule : NULL);

    error_code = app_timer_create(
        &timer_id_1hz_0, APP_TIMER_MODE_REPEATED, task_1hz_0);         
    APP_ERROR_CHECK(error_code);

    error_code = app_timer_create(
        &timer_id_1hz_1, APP_TIMER_MODE_REPEATED, task_1hz_1);         
    APP_ERROR_CHECK(error_code);

    error_code = app_timer_create(
        &timer_id_10hz, APP_TIMER_MODE_REPEATED, task_10hz);       
    APP_ERROR_CHECK(error_code);
}


void timer_start_1hz_periodic_0()
{
    uint32_t error_code;
    error_code = app_timer_start(
        timer_id_1hz_0, APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER),   
        app_timer_evt_schedule);        
    APP_ERROR_CHECK(error_code);
}

void timer_stop_1hz_periodic_0()
{
    uint32_t error_code;
    error_code = app_timer_stop(timer_id_1hz_0); 
    APP_ERROR_CHECK(error_code);
}

void timer_start_1hz_periodic_1()
{
    uint32_t error_code;
    error_code = app_timer_start(
        timer_id_1hz_1, APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER),   
        app_timer_evt_schedule);        
    APP_ERROR_CHECK(error_code);
}

void timer_stop_1hz_periodic_1()
{
    uint32_t error_code;
    error_code = app_timer_stop(timer_id_1hz_1); 
    APP_ERROR_CHECK(error_code);
}

void timer_start_10hz_periodic()
{
    uint32_t error_code;
    error_code = app_timer_start(
        timer_id_10hz, APP_TIMER_TICKS(100, APP_TIMER_PRESCALER),   
        app_timer_evt_schedule);        
    APP_ERROR_CHECK(error_code);
}

void timer_stop_10hz_periodic()
{
    uint32_t error_code;
    error_code = app_timer_stop(timer_id_10hz); 
    APP_ERROR_CHECK(error_code);
}
