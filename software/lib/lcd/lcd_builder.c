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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "lcd_driver.h"
#include "lcd_builder.h"
#include "charData.h"
#include "ble_config.h"
#include "date_time.h"
#include "watch_data.h"


void lcd_builder_init()
{
    // Set goal digit index to one past the end (no highlight)
    lcd_builder_step_goal_digit = 5;
}

void lcd_builder_build_sleep_message()
{
    // Note: the screen must be cleared prior to this!
    lcd_setCursor(2, 37);
    lcd_transferString("shutting");
    lcd_setCursor(4, 57);
    lcd_transferString("down");
}

void lcd_builder_build_gps()
{
    lcd_builder_build_top_bar(true);
    lcd_clearLines(13, 96);
    lcd_setCursor(0, 17);

    lcd_Cursor.row++;
    lcd_transferString("gps");
    lcd_Cursor.row++;

    lcd_transferString("coords");

    lcd_drawLine(32);
    lcd_setCursor(0, 36);

    lcd_transferSmallNumInt((int)watch_data_gps.latitude[0]-'0');
    lcd_transferSmallNumInt((int)watch_data_gps.latitude[1]-'0');
    lcd_transferSmallNumInt((int)watch_data_gps.latitude[2]-'0');
    lcd_Cursor.row++;
    lcd_transferSmallNumInt((int)watch_data_gps.latitude[4]-'0');
    lcd_transferSmallNumInt((int)watch_data_gps.latitude[5]-'0');
    lcd_transferSpecialChar(watch_data_gps.latitude[6]);
    lcd_transferSmallNumInt((int)watch_data_gps.latitude[7]-'0');
    lcd_transferSmallNumInt((int)watch_data_gps.latitude[8]-'0');
    lcd_transferSmallNumInt((int)watch_data_gps.latitude[9]-'0');
    lcd_transferSmallNumInt((int)watch_data_gps.latitude[10]-'0');
    lcd_transferChar((char)((int)watch_data_gps.latitude[12]+32));

    lcd_setCursor(0, 51);

    lcd_transferSmallNumInt((int)watch_data_gps.longitude[0]-'0');
    lcd_transferSmallNumInt((int)watch_data_gps.longitude[1]-'0');
    lcd_Cursor.row++;
    lcd_transferSmallNumInt((int)watch_data_gps.longitude[3]-'0');
    lcd_transferSmallNumInt((int)watch_data_gps.longitude[4]-'0');
    lcd_transferSpecialChar(watch_data_gps.longitude[5]);
    lcd_transferSmallNumInt((int)watch_data_gps.longitude[6]-'0');
    lcd_transferSmallNumInt((int)watch_data_gps.longitude[7]-'0');
    lcd_transferSmallNumInt((int)watch_data_gps.longitude[8]-'0');
    lcd_transferSmallNumInt((int)watch_data_gps.longitude[9]-'0');
    lcd_Cursor.row++;
    lcd_transferChar((char)((int)watch_data_gps.longitude[11]+32));

    lcd_drawLine(65);

    lcd_setCursor(0, 69);
    lcd_transferString("alt");
    lcd_transferSpecialChar(':');
    lcd_Cursor.row++;
    lcd_transferSmallNumInt(watch_data_gps.altitude);
    lcd_Cursor.row++;
    lcd_transferChar('m');

    lcd_setCursor(0, 83);
    lcd_transferString("speed");
    lcd_transferSpecialChar(':');
    lcd_Cursor.row++;
    lcd_transferSmallNumInt(watch_data_gps.ground_speed);
    lcd_setCursor(9, 83);
    lcd_transferChar('m');
    lcd_transferSpecialChar('/');
    lcd_transferChar('s');
}

void lcd_builder_build_stopwatch()
{
    lcd_builder_build_top_bar(true);
    lcd_clearLines(13,96);
    lcd_setCursor(0, 14);

    lcd_transferString("stop");
    lcd_Cursor.row++;
    lcd_transferString("watch");

    lcd_setCursor(0, 28);
    if (watch_data_stopwatch.timer_minutes < 10) {
        lcd_transferBigNumInt(0);
    }
    lcd_transferBigNumInt(watch_data_stopwatch.timer_minutes);
    lcd_transferSpecialBigChar(':');
    if (watch_data_stopwatch.timer_seconds < 10) {
        lcd_transferBigNumInt(0);
    }
    lcd_transferBigNumInt(watch_data_stopwatch.timer_seconds);
    lcd_setCursor(9, 38);
    lcd_transferSpecialChar('.');
    lcd_transferSmallNumInt(watch_data_stopwatch.timer_tenths);
    lcd_transferSmallNumInt(0);

    lcd_drawLine(54);

    if (watch_data_stopwatch.lapCounter > 0)
    {
        lcd_setCursor(0, 56);
        lcd_transferChar('l');
        lcd_transferSmallNumInt(watch_data_stopwatch.lapCounter);
        if (watch_data_stopwatch.lapCounter < 10) {
            lcd_transferSpecialChar(':');
        }
        lcd_Cursor.row++;
        if (watch_data_stopwatch.lapTimesMin[0] < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(watch_data_stopwatch.lapTimesMin[0]);
        lcd_transferSpecialChar(':');
        if (watch_data_stopwatch.lapTimesSec[0] < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(watch_data_stopwatch.lapTimesSec[0]);
        lcd_transferSpecialChar('.');
        lcd_transferSmallNumInt(watch_data_stopwatch.lapTimesTenths[0]);
        lcd_transferSmallNumInt(0);
        lcd_drawLine(68);
    }

    if (watch_data_stopwatch.lapCounter > 1)
    {
        lcd_setCursor(0, 70);
        lcd_transferChar('l');
        lcd_transferSmallNumInt(watch_data_stopwatch.lapCounter-1);
        if (watch_data_stopwatch.lapCounter-1 < 10) {
            lcd_transferSpecialChar(':');
        }
        lcd_Cursor.row++;
        if (watch_data_stopwatch.lapTimesMin[1] < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(watch_data_stopwatch.lapTimesMin[1]);
        lcd_transferSpecialChar(':');
        if (watch_data_stopwatch.lapTimesSec[1] < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(watch_data_stopwatch.lapTimesSec[1]);
        lcd_transferSpecialChar('.');
        lcd_transferSmallNumInt(watch_data_stopwatch.lapTimesTenths[1]);
        lcd_transferSmallNumInt(0);
        lcd_drawLine(82);
    }

    if (watch_data_stopwatch.lapCounter > 2)
    {
        lcd_setCursor(0, 84);
        lcd_transferChar('l');
        lcd_transferSmallNumInt(watch_data_stopwatch.lapCounter-2);
        if (watch_data_stopwatch.lapCounter-2 < 10) {
            lcd_transferSpecialChar(':');
        }
        lcd_Cursor.row++;

        if (watch_data_stopwatch.lapTimesMin[2] < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(watch_data_stopwatch.lapTimesMin[2]);
        lcd_transferSpecialChar(':');
        if (watch_data_stopwatch.lapTimesSec[2] < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(watch_data_stopwatch.lapTimesSec[2]);
        lcd_transferSpecialChar('.');
        lcd_transferSmallNumInt(watch_data_stopwatch.lapTimesTenths[2]);
        lcd_transferSmallNumInt(0);
    }
}

void lcd_builder_build_steps()
{
    int i;
    lcd_builder_build_top_bar(true);
    lcd_clearLines(13,96);
    lcd_setCursor(0, 20);

    lcd_transferString("steps");

    lcd_setCursor(1, 38);
    if (watch_data_step.steps < 10000) {
        lcd_transferBigNumInt(0);
    }
    if (watch_data_step.steps < 1000) {
       lcd_transferBigNumInt(0);
    }
    if (watch_data_step.steps < 100) {
        lcd_transferBigNumInt(0);
    }
    if (watch_data_step.steps < 10) {
        lcd_transferBigNumInt(0);
    }
    lcd_transferBigNumInt(watch_data_step.steps);

    lcd_setCursor(4, 66);
    lcd_transferString("of");
    lcd_Cursor.row++;
    for (i = 0; i < 5; i++)
    {
        lcd_transferSmallNumInt(watch_data_step.goal[i] - '0');
        if (lcd_builder_step_goal_digit == i)
        {
            lcd_invertBitMap(7+i, 66, 9);
        }
    }

    lcd_drawLine(82);

    lcd_setCursor(0, 84);
    lcd_transferString("last");
    lcd_transferSpecialChar(':');
    lcd_Cursor.row++;
    if (watch_data_step.yesterday_steps < 10000) {
        lcd_transferSmallNumInt(0);
    }
    if (watch_data_step.yesterday_steps < 1000) {
        lcd_transferSmallNumInt(0);
    }
    if (watch_data_step.yesterday_steps < 100) {
        lcd_transferSmallNumInt(0);
    }
    if (watch_data_step.yesterday_steps < 10) {
        lcd_transferSmallNumInt(0);
    }
    lcd_transferSmallNumInt(watch_data_step.yesterday_steps);
}

void lcd_builder_build_watch_face()
{
    lcd_builder_build_top_bar(false);
    lcd_setCursor(1, 37);

    if (date_time.hours < 10) {
        lcd_transferBigNumInt(0);
    }
    lcd_transferBigNumInt(date_time.hours);
    lcd_transferBigNumInt(99999); // Big colon FIXME use a constant for this
    if (date_time.minutes < 10) {
        lcd_transferBigNumInt(0);
    }
    lcd_transferBigNumInt(date_time.minutes);

    lcd_clearLines(82, 96);
    lcd_setCursor(1, 82);
    lcd_transferString(date_time.day_str);
    lcd_Cursor.row++;
    lcd_transferString(date_time.month_str);
    lcd_Cursor.row++;
    lcd_transferSmallNumInt(date_time.day_num);
}

void lcd_builder_build_run()
{
    lcd_builder_build_top_bar(true);

    lcd_clearLines(13,96);
    lcd_setCursor(0, 14);

    lcd_transferString("run");
    lcd_Cursor.row++;
    lcd_transferString("time");

    lcd_setCursor(0, 28);
    if (watch_data_run.timer_hours < 10) {
        lcd_transferBigNumInt(0);
    }
    lcd_transferBigNumInt(watch_data_run.timer_hours);
    lcd_transferSpecialBigChar(':');
    if (watch_data_run.timer_minutes < 10) {
        lcd_transferBigNumInt(0);
    }
    lcd_transferBigNumInt(watch_data_run.timer_minutes);
    lcd_setCursor(9, 38);
    lcd_transferSpecialChar(':');
    if (watch_data_run.timer_seconds < 10) {
        lcd_transferSmallNumInt(0);
    }
    lcd_transferSmallNumInt(watch_data_run.timer_seconds);

    if (watch_data_run.timer_running == false)
    {
        lcd_setCursor(5,52);
      	lcd_transferString("stopped");
    }

    lcd_drawLine(68);

    lcd_setCursor(0, 70);
    lcd_transferString("dist");
    lcd_transferSpecialChar(':');
    lcd_Cursor.row+=2;
    
    // TODO Is there a better way to do this?
    lcd_transferSmallNumInt(watch_data_run.meters/1000);
    lcd_transferSpecialChar('.');
    lcd_transferSmallNumInt((watch_data_run.meters%1000)/100);
    lcd_transferSmallNumInt((watch_data_run.meters%100)/10);

    lcd_drawLine(82);

    lcd_setCursor(0, 84);
    lcd_transferString("pace");
    lcd_transferSpecialChar(':');
    lcd_Cursor.row+=2;
    lcd_transferSmallNumInt(watch_data_run.pace_minutes);
    lcd_transferSpecialChar(':');
    if (watch_data_run.pace_seconds < 10)
    {
        lcd_transferSmallNumInt(0);
    }
    lcd_transferSmallNumInt(watch_data_run.pace_seconds);
}

void lcd_builder_build_top_bar(bool time)
{
    lcd_clearLines(1,11);
    if (time) {
        lcd_drawLine(12);
    }
    lcd_setCursor(0,1);

    // FIXME change back to battery icon when fuel gauge works on PCB
    //lcd_transferBatteryLevel(watch_data_battery_level);
    lcd_transferBatteryLevel(70);
    //lcd_transferSmallNumInt(watch_data_battery_level);
    //------

    if (watch_data_bluetooth_state == BLE_STATE_ADVERTISING) {
        lcd_transferSpecialChar('&'); // Bluetooth symbol FIXME use constant
        lcd_transferChar('a');
    } else if (watch_data_bluetooth_state == BLE_STATE_CONNECTED) {
        lcd_transferSpecialChar('&'); // Bluetooth symbol
        lcd_transferChar('c');
    } else {
        lcd_Cursor.row += 2;
    }

    lcd_Cursor.row++;
    if (watch_data_gps_logging_on) {
        lcd_transferChar('l');
    } else {
        lcd_Cursor.row++;
    }
    lcd_Cursor.row++;

    if (time) {
        if (date_time.hours < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(date_time.hours);
        lcd_transferSpecialChar(':');
        if (date_time.minutes < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(date_time.minutes);
    } 
    else {
        lcd_Cursor.row += 2;
        lcd_transferString("eir");
    }
}
