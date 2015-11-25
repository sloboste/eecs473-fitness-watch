#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "lcd_driver.h"
#include "lcd_builder.h"
#include "charData.h"
#include "ble_config.h"
#include "date_time.h"


void lcd_builder_init_structs()
{
    lcd_builder_bluetooth_state = BLE_STATE_IDLE;
    lcd_builder_battery_level = 0;

    //TIMER
    lcd_builder_stopwatch_timer_reset();

    // GPS
    memset(&lcd_builder_gps_data, 0, sizeof(lcd_builder_gps_data));
    lcd_builder_gps_data.longitude = "012 23.5678 S";
    lcd_builder_gps_data.latitude = "98 87.5432 W";
    lcd_builder_gps_data.altitude = 5898;
    lcd_builder_gps_data.ground_speed = 23;

    // Run
    lcd_builder_run_timer_reset(); 
  	lcd_builder_run_data.meters = 1260;
  	lcd_builder_run_data.pace_minutes = 3;
  	lcd_builder_run_data.pace_seconds = 12;

    // Steps
    memset(&lcd_builder_step_data, 0, sizeof(lcd_builder_step_data));
    lcd_builder_step_data.yesterday_steps = 12345;
    // Set goal digit index to one past the end (no highlight)
    lcd_builder_step_data.goal_digit = 5;
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
    lcd_clearLines(13,96);
    lcd_setCursor(0, 17);

    lcd_Cursor.row++;
    lcd_transferString("gps");
    lcd_Cursor.row++;

    lcd_transferString("coords");

    lcd_drawLine(32);
    lcd_setCursor(0, 36);
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.longitude[0]-'0');
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.longitude[1]-'0');
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.longitude[2]-'0');
    lcd_Cursor.row++;
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.longitude[4]-'0');
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.longitude[5]-'0');
    lcd_transferSpecialChar(lcd_builder_gps_data.longitude[6]);
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.longitude[7]-'0');
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.longitude[8]-'0');
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.longitude[9]-'0');
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.longitude[10]-'0');
    lcd_transferChar((char)((int)lcd_builder_gps_data.longitude[12]+32));

    lcd_setCursor(0, 51);
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.latitude[0]-'0');
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.latitude[1]-'0');
    lcd_Cursor.row++;
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.latitude[3]-'0');
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.latitude[4]-'0');
    lcd_transferSpecialChar(lcd_builder_gps_data.latitude[5]-'0');
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.latitude[6]-'0');
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.latitude[7]-'0');
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.latitude[8]-'0');
    lcd_transferSmallNumInt((int)lcd_builder_gps_data.latitude[9]-'0');
    lcd_Cursor.row++;
    lcd_transferChar((char)((int)lcd_builder_gps_data.latitude[11]+32));

    lcd_drawLine(65);

    lcd_setCursor(0, 69);
    lcd_transferString("alt");
    lcd_transferSpecialChar(':');
    lcd_Cursor.row++;
    lcd_transferSmallNumInt(lcd_builder_gps_data.altitude);

    lcd_setCursor(0, 83);
    lcd_transferString("speed");
    lcd_transferSpecialChar(':');
    lcd_Cursor.row++;
    lcd_transferSmallNumInt(lcd_builder_gps_data.ground_speed);
}

void lcd_builder_stopwatch_timer_reset()
{
    memset(&lcd_builder_stopwatch_data, 0, sizeof(lcd_builder_stopwatch_data));
}

void lcd_builder_stopwatch_timer_lap()
{
    lcd_builder_stopwatch_data.lapTimesMin[2] =
        lcd_builder_stopwatch_data.lapTimesMin[1];
    lcd_builder_stopwatch_data.lapTimesSec[2] =
        lcd_builder_stopwatch_data.lapTimesSec[1];
    lcd_builder_stopwatch_data.lapTimesTenths[2] =
        lcd_builder_stopwatch_data.lapTimesTenths[1];
    lcd_builder_stopwatch_data.lapTimesMin[1] =
        lcd_builder_stopwatch_data.lapTimesMin[0];
    lcd_builder_stopwatch_data.lapTimesSec[1] =
        lcd_builder_stopwatch_data.lapTimesSec[0];
    lcd_builder_stopwatch_data.lapTimesTenths[1] =
        lcd_builder_stopwatch_data.lapTimesTenths[0];
    lcd_builder_stopwatch_data.lapTimesMin[0] =
        lcd_builder_stopwatch_data.timer_minutes;
    lcd_builder_stopwatch_data.lapTimesSec[0] =
        lcd_builder_stopwatch_data.timer_seconds;
    lcd_builder_stopwatch_data.lapTimesTenths[0] =
        lcd_builder_stopwatch_data.timer_tenths;
    lcd_builder_stopwatch_data.lapCounter++;
    if (lcd_builder_stopwatch_data.lapCounter >= 100)
    {
        lcd_builder_stopwatch_data.lapCounter = 0;
    }
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
    if (lcd_builder_stopwatch_data.timer_minutes < 10) {
        lcd_transferBigNumInt(0);
    }
    lcd_transferBigNumInt(lcd_builder_stopwatch_data.timer_minutes);
    lcd_transferSpecialBigChar(':');
    if (lcd_builder_stopwatch_data.timer_seconds < 10) {
        lcd_transferBigNumInt(0);
    }
    lcd_transferBigNumInt(lcd_builder_stopwatch_data.timer_seconds);
    lcd_setCursor(9, 38);
    lcd_transferSpecialChar('.');
    lcd_transferSmallNumInt(lcd_builder_stopwatch_data.timer_tenths);
    lcd_transferSmallNumInt(0);

    lcd_drawLine(54);

    if (lcd_builder_stopwatch_data.lapCounter > 0)
    {
        lcd_setCursor(0, 56);
        lcd_transferChar('l');
        lcd_transferSmallNumInt(lcd_builder_stopwatch_data.lapCounter);
        if (lcd_builder_stopwatch_data.lapCounter < 10) {
            lcd_transferSpecialChar(':');
        }
        lcd_Cursor.row++;
        if (lcd_builder_stopwatch_data.lapTimesMin[0] < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesMin[0]);
        lcd_transferSpecialChar(':');
        if (lcd_builder_stopwatch_data.lapTimesSec[0] < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesSec[0]);
        lcd_transferSpecialChar('.');
        lcd_transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesTenths[0]);
        lcd_transferSmallNumInt(0);
        lcd_drawLine(68);
    }

    if (lcd_builder_stopwatch_data.lapCounter > 1)
    {
        lcd_setCursor(0, 70);
        lcd_transferChar('l');
        lcd_transferSmallNumInt(lcd_builder_stopwatch_data.lapCounter-1);
        if (lcd_builder_stopwatch_data.lapCounter-1 < 10) {
            lcd_transferSpecialChar(':');
        }
        lcd_Cursor.row++;
        if (lcd_builder_stopwatch_data.lapTimesMin[1] < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesMin[1]);
        lcd_transferSpecialChar(':');
        if (lcd_builder_stopwatch_data.lapTimesSec[1] < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesSec[1]);
        lcd_transferSpecialChar('.');
        lcd_transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesTenths[1]);
        lcd_transferSmallNumInt(0);
        lcd_drawLine(82);
    }

    if (lcd_builder_stopwatch_data.lapCounter > 2)
    {
        lcd_setCursor(0, 84);
        lcd_transferChar('l');
        lcd_transferSmallNumInt(lcd_builder_stopwatch_data.lapCounter-2);
        if (lcd_builder_stopwatch_data.lapCounter-2 < 10) {
            lcd_transferSpecialChar(':');
        }
        lcd_Cursor.row++;

        if (lcd_builder_stopwatch_data.lapTimesMin[2] < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesMin[2]);
        lcd_transferSpecialChar(':');
        if (lcd_builder_stopwatch_data.lapTimesSec[2] < 10) {
            lcd_transferSmallNumInt(0);
        }
        lcd_transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesSec[2]);
        lcd_transferSpecialChar('.');
        lcd_transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesTenths[2]);
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
    if (lcd_builder_step_data.steps < 10000) {
        lcd_transferBigNumInt(0);
    }
    if (lcd_builder_step_data.steps < 1000) {
       lcd_transferBigNumInt(0);
    }
    if (lcd_builder_step_data.steps < 100) {
        lcd_transferBigNumInt(0);
    }
    if (lcd_builder_step_data.steps < 10) {
        lcd_transferBigNumInt(0);
    }
    lcd_transferBigNumInt(lcd_builder_step_data.steps);

    lcd_setCursor(4, 66);
    lcd_transferString("of");
    lcd_Cursor.row++;
    for (i = 0; i < 5; i++)
    {
        lcd_transferSmallNumInt(lcd_builder_step_data.goal[i]);
        if (lcd_builder_step_data.goal_digit == i)
        {
            lcd_invertBitMap(7+i, 66, 9);
        }
    }

    lcd_drawLine(82);

    lcd_setCursor(0, 84);
    lcd_transferString("last");
    lcd_transferSpecialChar(':');
    lcd_Cursor.row++;
    if (lcd_builder_step_data.yesterday_steps < 10000) {
        lcd_transferSmallNumInt(0);
    }
    if (lcd_builder_step_data.yesterday_steps < 1000) {
        lcd_transferSmallNumInt(0);
    }
    if (lcd_builder_step_data.yesterday_steps < 100) {
        lcd_transferSmallNumInt(0);
    }
    if (lcd_builder_step_data.yesterday_steps < 10) {
        lcd_transferSmallNumInt(0);
    }
    lcd_transferSmallNumInt(lcd_builder_step_data.yesterday_steps);
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

void lcd_builder_run_timer_reset()
{
    memset(&lcd_builder_run_data, 0, sizeof(lcd_builder_run_data));
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
    if (lcd_builder_run_data.timer_hours < 10) {
        lcd_transferBigNumInt(0);
    }
    lcd_transferBigNumInt(lcd_builder_run_data.timer_hours);
    lcd_transferSpecialBigChar(':');
    if (lcd_builder_run_data.timer_minutes < 10) {
        lcd_transferBigNumInt(0);
    }
    lcd_transferBigNumInt(lcd_builder_run_data.timer_minutes);
    lcd_setCursor(9, 38);
    lcd_transferSpecialChar(':');
    if (lcd_builder_run_data.timer_seconds < 10) {
        lcd_transferSmallNumInt(0);
    }
    lcd_transferSmallNumInt(lcd_builder_run_data.timer_seconds);

    if (lcd_builder_run_data.timer_running == false)
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
    lcd_transferSmallNumInt(lcd_builder_run_data.meters/1000);
    lcd_transferSpecialChar('.');
    lcd_transferSmallNumInt((lcd_builder_run_data.meters%1000)/100);
    lcd_transferSmallNumInt((lcd_builder_run_data.meters%100)/10);

    lcd_drawLine(82);

    lcd_setCursor(0, 84);
    lcd_transferString("pace");
    lcd_transferSpecialChar(':');
    lcd_Cursor.row+=2;
    lcd_transferSmallNumInt(lcd_builder_run_data.pace_minutes);
    lcd_transferSpecialChar(':');
    if (lcd_builder_run_data.pace_seconds < 10)
    {
        lcd_transferSmallNumInt(0);
    }
    lcd_transferSmallNumInt(lcd_builder_run_data.pace_seconds);
}

void lcd_builder_build_top_bar(bool time)
{
    lcd_clearLines(1,11);
    if (time) {
        lcd_drawLine(12);
    }
    lcd_setCursor(0,1);
    lcd_transferBatteryLevel(3);
    if (lcd_builder_bluetooth_state == BLE_STATE_ADVERTISING) {
        lcd_transferSpecialChar('&'); // Bluetooth symbol
        lcd_transferChar('a');
    } else if (lcd_builder_bluetooth_state == BLE_STATE_CONNECTED) {
        lcd_transferSpecialChar('&'); // Bluetooth symbol
        lcd_transferChar('c');
    } else {
        lcd_Cursor.row += 2;
    }
    lcd_Cursor.row += 3;

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
