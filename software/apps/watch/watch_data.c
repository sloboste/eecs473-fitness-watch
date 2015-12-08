#include "stdbool.h"
#include "stdint.h"
#include "string.h"

#include "watch_data.h"
#include "ble_config.h"


void watch_data_stopwatch_timer_lap()
{
    watch_data_stopwatch.lapTimesMin[2] =
        watch_data_stopwatch.lapTimesMin[1];
    watch_data_stopwatch.lapTimesSec[2] =
        watch_data_stopwatch.lapTimesSec[1];
    watch_data_stopwatch.lapTimesTenths[2] =
        watch_data_stopwatch.lapTimesTenths[1];
    watch_data_stopwatch.lapTimesMin[1] =
        watch_data_stopwatch.lapTimesMin[0];
    watch_data_stopwatch.lapTimesSec[1] =
        watch_data_stopwatch.lapTimesSec[0];
    watch_data_stopwatch.lapTimesTenths[1] =
        watch_data_stopwatch.lapTimesTenths[0];
    watch_data_stopwatch.lapTimesMin[0] =
        watch_data_stopwatch.timer_minutes;
    watch_data_stopwatch.lapTimesSec[0] =
        watch_data_stopwatch.timer_seconds;
    watch_data_stopwatch.lapTimesTenths[0] =
        watch_data_stopwatch.timer_tenths;
    watch_data_stopwatch.lapCounter++;
    if (watch_data_stopwatch.lapCounter > 99) {
        watch_data_stopwatch.lapCounter = 0;
    }
}

void watch_data_stopwatch_timer_reset()
{
    memset(&watch_data_stopwatch, 0, sizeof(watch_data_stopwatch));
}

void watch_data_run_timer_reset()
{
    memset(&watch_data_run, 0, sizeof(watch_data_run));
}

void watch_data_init()
{
    watch_data_bluetooth_state = BLE_STATE_IDLE;
    watch_data_battery_level = 0;
    watch_data_stopwatch_timer_reset();
    watch_data_run_timer_reset(); 

    char * default_long = "42 17.4683 N";
    char * default_lat = "83 42.9367 W";
    memset(&watch_data_gps, 0, sizeof(watch_data_gps));
    watch_data_gps.altitude = 267;
    watch_data_gps.ground_speed = 0;
    strcpy(watch_data_gps.longitude, default_long);
    strcpy(watch_data_gps.latitude, default_lat);

    memset(&watch_data_step, 0, sizeof(watch_data_step));
    uint8_t i;
    for (i = 0; i < 5; ++i) {
        watch_data_step.goal[i] = '0';
    }
}
