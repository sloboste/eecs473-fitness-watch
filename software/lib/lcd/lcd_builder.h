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
 *
 *
 * High level functions and data structures used to build different screens on
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
