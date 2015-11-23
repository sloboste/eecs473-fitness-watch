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

void lcd_builder_build_gps()
{
    lcd_builder_build_top_bar(true);
    clearLines(13,96);
    setCursor(0, 17);

    Cursor.row++;
    transferString("gps");
    Cursor.row++;

    transferString("coords");

    drawLine(32);
    setCursor(0, 36);
    transferSmallNumInt((int)lcd_builder_gps_data.longitude[0]-'0');
    transferSmallNumInt((int)lcd_builder_gps_data.longitude[1]-'0');
    transferSmallNumInt((int)lcd_builder_gps_data.longitude[2]-'0');
    Cursor.row++;
    transferSmallNumInt((int)lcd_builder_gps_data.longitude[4]-'0');
    transferSmallNumInt((int)lcd_builder_gps_data.longitude[5]-'0');
    transferSpecialChar(lcd_builder_gps_data.longitude[6]);
    transferSmallNumInt((int)lcd_builder_gps_data.longitude[7]-'0');
    transferSmallNumInt((int)lcd_builder_gps_data.longitude[8]-'0');
    transferSmallNumInt((int)lcd_builder_gps_data.longitude[9]-'0');
    transferSmallNumInt((int)lcd_builder_gps_data.longitude[10]-'0');
    transferChar((char)((int)lcd_builder_gps_data.longitude[12]+32));

    setCursor(0, 51);
    transferSmallNumInt((int)lcd_builder_gps_data.latitude[0]-'0');
    transferSmallNumInt((int)lcd_builder_gps_data.latitude[1]-'0');
    Cursor.row++;
    transferSmallNumInt((int)lcd_builder_gps_data.latitude[3]-'0');
    transferSmallNumInt((int)lcd_builder_gps_data.latitude[4]-'0');
    transferSpecialChar(lcd_builder_gps_data.latitude[5]-'0');
    transferSmallNumInt((int)lcd_builder_gps_data.latitude[6]-'0');
    transferSmallNumInt((int)lcd_builder_gps_data.latitude[7]-'0');
    transferSmallNumInt((int)lcd_builder_gps_data.latitude[8]-'0');
    transferSmallNumInt((int)lcd_builder_gps_data.latitude[9]-'0');
    Cursor.row++;
    transferChar((char)((int)lcd_builder_gps_data.latitude[11]+32));

    drawLine(65);

    setCursor(0, 69);
    transferString("alt");
    transferSpecialChar(':');
    Cursor.row++;
    transferSmallNumInt(lcd_builder_gps_data.altitude);

    setCursor(0, 83);
    transferString("speed");
    transferSpecialChar(':');
    Cursor.row++;
    transferSmallNumInt(lcd_builder_gps_data.ground_speed);
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
    clearLines(13,96);
    setCursor(0, 14);

    transferString("stop");
    Cursor.row++;
    transferString("watch");

    setCursor(0, 28);
    if (lcd_builder_stopwatch_data.timer_minutes < 10) {
      transferBigNumInt(0);
    }
    transferBigNumInt(lcd_builder_stopwatch_data.timer_minutes);
    transferSpecialBigChar(':');
    if (lcd_builder_stopwatch_data.timer_seconds < 10) {
      transferBigNumInt(0);
    }
    transferBigNumInt(lcd_builder_stopwatch_data.timer_seconds);
    setCursor(9, 38);
    transferSpecialChar('.');
    transferSmallNumInt(lcd_builder_stopwatch_data.timer_tenths);
    transferSmallNumInt(0);

    drawLine(54);

    if (lcd_builder_stopwatch_data.lapCounter > 0)
    {
        setCursor(0, 56);
        transferChar('l');
        transferSmallNumInt(lcd_builder_stopwatch_data.lapCounter);
        if (lcd_builder_stopwatch_data.lapCounter < 10) {
            transferSpecialChar(':');
        }
        Cursor.row++;
        if (lcd_builder_stopwatch_data.lapTimesMin[0] < 10) {
            transferSmallNumInt(0);
        }
        transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesMin[0]);
        transferSpecialChar(':');
        if (lcd_builder_stopwatch_data.lapTimesSec[0] < 10) {
            transferSmallNumInt(0);
        }
        transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesSec[0]);
        transferSpecialChar('.');
        transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesTenths[0]);
        transferSmallNumInt(0);
        drawLine(68);
    }

    if (lcd_builder_stopwatch_data.lapCounter > 1)
    {
        setCursor(0, 70);
        transferChar('l');
        transferSmallNumInt(lcd_builder_stopwatch_data.lapCounter-1);
        if (lcd_builder_stopwatch_data.lapCounter-1 < 10) {
            transferSpecialChar(':');
        }
        Cursor.row++;
        if (lcd_builder_stopwatch_data.lapTimesMin[1] < 10) {
            transferSmallNumInt(0);
        }
        transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesMin[1]);
        transferSpecialChar(':');
        if (lcd_builder_stopwatch_data.lapTimesSec[1] < 10) {
            transferSmallNumInt(0);
        }
        transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesSec[1]);
        transferSpecialChar('.');
        transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesTenths[1]);
        transferSmallNumInt(0);
        drawLine(82);
    }

    if (lcd_builder_stopwatch_data.lapCounter > 2)
    {
        setCursor(0, 84);
        transferChar('l');
        transferSmallNumInt(lcd_builder_stopwatch_data.lapCounter-2);
        if (lcd_builder_stopwatch_data.lapCounter-2 < 10) {
            transferSpecialChar(':');
        }
        Cursor.row++;

        if (lcd_builder_stopwatch_data.lapTimesMin[2] < 10) {
            transferSmallNumInt(0);
        }
        transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesMin[2]);
        transferSpecialChar(':');
        if (lcd_builder_stopwatch_data.lapTimesSec[2] < 10) {
            transferSmallNumInt(0);
        }
        transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesSec[2]);
        transferSpecialChar('.');
        transferSmallNumInt(lcd_builder_stopwatch_data.lapTimesTenths[2]);
        transferSmallNumInt(0);
    }
}

void lcd_builder_build_steps()
{
    int i;
    lcd_builder_build_top_bar(true);
    clearLines(13,96);
    setCursor(0, 20);

    transferString("steps");

    setCursor(1, 38);
    if (lcd_builder_step_data.steps < 10000) {
        transferBigNumInt(0);
    }
    if (lcd_builder_step_data.steps < 1000) {
        transferBigNumInt(0);
    }
    if (lcd_builder_step_data.steps < 100) {
        transferBigNumInt(0);
    }
    if (lcd_builder_step_data.steps < 10) {
        transferBigNumInt(0);
    }
    transferBigNumInt(lcd_builder_step_data.steps);

    setCursor(4, 66);
    transferString("of");
    Cursor.row++;
    for (i = 0; i < 5; i++)
    {
        transferSmallNumInt(lcd_builder_step_data.goal[i]);
        if (lcd_builder_step_data.goal_digit == i)
        {
            invertBitMap(7+i, 66, 9);
        }
    }

    drawLine(82);

    setCursor(0, 84);
    transferString("last");
    transferSpecialChar(':');
    Cursor.row++;
    if (lcd_builder_step_data.yesterday_steps < 10000) {
        transferSmallNumInt(0);
    }
    if (lcd_builder_step_data.yesterday_steps < 1000) {
        transferSmallNumInt(0);
    }
    if (lcd_builder_step_data.yesterday_steps < 100) {
        transferSmallNumInt(0);
    }
    if (lcd_builder_step_data.yesterday_steps < 10) {
        transferSmallNumInt(0);
    }
    transferSmallNumInt(lcd_builder_step_data.yesterday_steps);
}

void lcd_builder_build_watch_face()
{
    lcd_builder_build_top_bar(false);
    setCursor(1, 37);

    if (date_time.hours < 10) {
        transferBigNumInt(0);
    }
    transferBigNumInt(date_time.hours);
    transferBigNumInt(99999); // Big colon FIXME use a constant for this
    if (date_time.minutes < 10) {
        transferBigNumInt(0);
    }
    transferBigNumInt(date_time.minutes);

    clearLines(82, 96);
    setCursor(1, 82);
    transferString(date_time.day_str);
    Cursor.row++;
    transferString(date_time.month_str);
    Cursor.row++;
    transferSmallNumInt(date_time.day_num);
}

void lcd_builder_run_timer_reset()
{
    memset(&lcd_builder_run_data, 0, sizeof(lcd_builder_run_data));
}

void lcd_builder_build_run()
{
    lcd_builder_build_top_bar(true);

    clearLines(13,96);
    setCursor(0, 14);

    transferString("run");
    Cursor.row++;
    transferString("time");

    setCursor(0, 28);
    if (lcd_builder_run_data.timer_hours < 10) {
        transferBigNumInt(0);
    }
    transferBigNumInt(lcd_builder_run_data.timer_hours);
    transferSpecialBigChar(':');
    if (lcd_builder_run_data.timer_minutes < 10) {
        transferBigNumInt(0);
    }
    transferBigNumInt(lcd_builder_run_data.timer_minutes);
    setCursor(9, 38);
    transferSpecialChar(':');
    if (lcd_builder_run_data.timer_seconds < 10) {
        transferSmallNumInt(0);
    }
    transferSmallNumInt(lcd_builder_run_data.timer_seconds);

    if (lcd_builder_run_data.timer_running == false)
    {
        setCursor(5,52);
      	transferString("stopped");
    }

    drawLine(68);

    setCursor(0, 70);
    transferString("dist");
    transferSpecialChar(':');
    Cursor.row+=2;
    
    // TODO Is there a better way to do this?
    transferSmallNumInt(lcd_builder_run_data.meters/1000);
    transferSpecialChar('.');
    transferSmallNumInt((lcd_builder_run_data.meters%1000)/100);
    transferSmallNumInt((lcd_builder_run_data.meters%100)/10);

    drawLine(82);

    setCursor(0, 84);
    transferString("pace");
    transferSpecialChar(':');
    Cursor.row+=2;
    transferSmallNumInt(lcd_builder_run_data.pace_minutes);
    transferSpecialChar(':');
    if (lcd_builder_run_data.pace_seconds < 10)
    {
        transferSmallNumInt(0);
    }
    transferSmallNumInt(lcd_builder_run_data.pace_seconds);
}

void lcd_builder_build_top_bar(bool time)
{
    clearLines(1,11);
    if (time) {
        drawLine(12);
    }
    setCursor(0,1);
    transferBatteryLevel(3);
    if (lcd_builder_bluetooth_state == BLE_STATE_ADVERTISING) {
        transferSpecialChar('&'); // Bluetooth symbol
        transferChar('a');
    } else if (lcd_builder_bluetooth_state == BLE_STATE_CONNECTED) {
        transferSpecialChar('&'); // Bluetooth symbol
        transferChar('c');
    } else {
        Cursor.row += 2;
    }
    Cursor.row += 3;

    if (time) {
        if (date_time.hours < 10) {
            transferSmallNumInt(0);
        }
        transferSmallNumInt(date_time.hours);
        transferSpecialChar(':');
        if (date_time.minutes < 10) {
            transferSmallNumInt(0);
        }
        transferSmallNumInt(date_time.minutes);
    } 
    else {
        Cursor.row += 2;
        transferString("eir");
    }
}
