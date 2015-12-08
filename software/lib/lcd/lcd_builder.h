/* High level functions and data structures used to build different screens on
 * the LCD. This abstracts away the line drawing and character sending LCD
 * functions.
 */

#ifndef LCD_BUILDER_H
#define LCD_BUILDER_H

#include <stdbool.h>
#include <stdint.h>

// The current digit the step goal digit selecter is on. 5 means none selected.
uint8_t lcd_builder_step_goal_digit;

/**
 * Register the watch data structures with the lcd_builder. 
 */
extern void lcd_builder_init();

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
