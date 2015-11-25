/* High level functions and data structures used to build different screens on
 * the LCD. This abstracts away the line drawing and character sending LCD
 * functions.
 */

#ifndef LCD_BUILDER_H
#define LCD_BUILDER_H

#include <stdbool.h>
#include <stdint.h>


//------------------------------------------------------------------------------
// TODO These probably belong in a data definition file since they will be used
// by multiple parts of the program that shouldn't know about the
// screen_building

// Holds GPS data in an LCD friendly format.
typedef struct lcd_builder_gps_data_struct {
   uint32_t ground_speed;
   uint32_t altitude;
   char * latitude;
   char * longitude;
} lcd_builder_gps_data_t;

// Holds run data in an LCD friendly format.
typedef struct lcd_builder_run_data_struct {
    uint16_t meters;
    uint8_t pace_minutes;
    uint8_t pace_seconds;
    uint8_t timer_hours;
    uint8_t timer_minutes;
    uint8_t timer_seconds;
    bool timer_running;
} lcd_builder_run_data_t;

// Holds step data in an LCD friendly format.
typedef struct lcd_builder_step_data_struct {
    uint32_t steps;
    uint32_t yesterday_steps;
    uint8_t goal_digit;
    uint8_t goal[5];
} lcd_builder_step_data_t;

// Holds stopwatch data in an LCD friendly format.
typedef struct lcd_builder_stopwatch_data_struct {
    uint8_t lapTimesMin[3];
    uint8_t lapTimesSec[3];
    uint8_t lapTimesTenths[3];
    uint8_t lapCounter;
    uint8_t timer_minutes;
    uint8_t timer_seconds;
    uint8_t timer_tenths;
} lcd_builder_stopwatch_data_t;


// The current battery level of the watch to show on the screen. 
uint8_t lcd_builder_battery_level;

// The current bluetooth state (one of ble_config's BLE_STATE_X).
uint8_t lcd_builder_bluetooth_state;

// The current GPS data that will be shown on the screen.
lcd_builder_gps_data_t lcd_builder_gps_data;

// The current run data that will be shown on the screen.
lcd_builder_run_data_t lcd_builder_run_data;

// The current step data that will be shown on the screen.
lcd_builder_step_data_t lcd_builder_step_data;

// The current step data that will be shown on the screen.
lcd_builder_stopwatch_data_t lcd_builder_stopwatch_data;


/**
 * Record a lap time in the stopwatch data struct.
 *
 * Note: this only changes the struct and doesn't make the changes visible.
 */
extern void lcd_builder_stopwatch_timer_lap();

/**
 * Zero out the values in the stopwatch data struct.
 *
 * Note: this only changes the struct and doesn't make the changes visible.
 */
extern void lcd_builder_stopwatch_timer_reset();

/**
 * Zero out the timer values in the run data struct.
 *
 * Note: this only changes the struct and doesn't make the changes visible.
 */
extern void lcd_builder_run_timer_reset();

// FIXME/TODO this function sucks
/**
 *
 */
extern void lcd_builder_init_structs();

//------------------------------------------------------------------------------

/**
 * Update the LCD bitmap for the pre-sleep mode message.
 *
 * Note: this only changes the bitmap and doesn't make the changes visible.
 */
extern void lcd_builder_build_sleep_message();

/**
 * Update the LCD bitmap for the top bar portion of the screen.
 *
 * time -- do or do not display the time on the top bar. 
 *
 * Note: this only changes the bitmap and doesn't make the changes visible.
 */
extern void lcd_builder_build_top_bar(bool time);

/**
 * Update the LCD bitmap to contain the watch face screen.
 *
 * Note: this only changes the bitmap and doesn't make the changes visible.
 */
extern void lcd_builder_build_watch_face();

/**
 * Update the LCD bitmap to contain the step count screen.
 *
 * Note: this only changes the bitmap and doesn't make the changes visible.
 */
extern void lcd_builder_build_steps();

/**
 * Update the LCD bitmap to contain the GPS screen.
 *
 * Note: this only changes the bitmap and doesn't make the changes visible.
 */
extern void lcd_builder_build_gps();

/**
 * Update the LCD bitmap to contain the stopwatch screen.
 *
 * Note: this only changes the bitmap and doesn't make the changes visible.
 */
extern void lcd_builder_build_stopwatch();

/**
 * Update the LCD bitmap to contain the run screen.
 *
 * Note: this only changes the bitmap and doesn't make the changes visible.
 */
extern void lcd_builder_build_run();

#endif
