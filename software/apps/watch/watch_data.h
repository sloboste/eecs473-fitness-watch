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

#ifndef WATCH_DATA_H
#define WATCH_DATA_H

#include "stdbool.h"
#include "stdint.h"

// Holds GPS data in an LCD friendly format.
typedef struct gps_data_struct {
   uint32_t ground_speed;
   uint32_t altitude;
   char latitude[14];
   char longitude[14];
} watch_data_gps_t;

// Holds run data in an LCD friendly format.
typedef struct run_data_struct {
    uint16_t meters;
    uint8_t pace_minutes;
    uint8_t pace_seconds;
    uint8_t timer_hours;
    uint8_t timer_minutes;
    uint8_t timer_seconds;
    bool timer_running;
} watch_data_run_t;

// Holds step data in an LCD friendly format.
typedef struct step_data_struct {
    uint32_t steps;
    uint32_t steps_offset;
    uint32_t yesterday_steps;
    char goal[6];
} watch_data_step_t;

// Holds stopwatch data in an LCD friendly format.
typedef struct stopwatch_data_struct {
    uint8_t lapTimesMin[3];
    uint8_t lapTimesSec[3];
    uint8_t lapTimesTenths[3];
    uint8_t lapCounter;
    uint8_t timer_minutes;
    uint8_t timer_seconds;
    uint8_t timer_tenths;
} watch_data_stopwatch_t;


// The current battery level of the watch. 
uint8_t watch_data_battery_level;

// The current bluetooth state (one of ble_config's BLE_STATE_X).
uint8_t watch_data_bluetooth_state;

// The current heart rate in BPM from the heart rate monitor.
uint32_t watch_data_heart_rate;

// The current GPS data.
watch_data_gps_t watch_data_gps;

// The current run data.
watch_data_run_t watch_data_run;

// The current step data.
watch_data_step_t watch_data_step;

// The current step data.
watch_data_stopwatch_t watch_data_stopwatch;


/**
 * Record a lap time in the stopwatch data struct.
 *
 * Note: this only changes the struct and doesn't make the changes visible.
 */
extern void watch_data_stopwatch_timer_lap();

/**
 * Zero out the values in the stopwatch data struct.
 *
 * Note: this only changes the struct and doesn't make the changes visible.
 */
extern void watch_data_stopwatch_timer_reset();

/**
 * Zero out the timer values in the run data struct.
 *
 * Note: this only changes the struct and doesn't make the changes visible.
 */
extern void watch_data_run_timer_reset();

/**
 * Initialize the watch data structures.
 */
extern void watch_data_init();

#endif
