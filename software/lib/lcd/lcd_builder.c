#include <stdint.h>
#include <stdbool.h>

#include "nrf_gpio.h" // FIXME needed?
#include "app_util_platform.h" // CRITICAL SECTION

#include "lcd_driver.h"
#include "lcd_builder.h"
#include "spi_driver.h"
#include "charData.h"
#include "ble_config.h"
#include "date_time.h"


/**************************************************************************/
/*!
    @brief    Initializes the data structures for the peripherals. The 
    @         current value and data types are not necessarily correct
*/
/**************************************************************************/  

void initStructs(){

  //TIMER
  TIMER_DATA.lapCounter = 0;
  
  lcd_builder_bluetooth_state = BLE_STATE_IDLE;

  // GPS
  GPS_DATA.longitude = "012 23.5678 S";
  GPS_DATA.latitude = "98 87.5432 W";
  GPS_DATA.altitude = 5898;
  GPS_DATA.ground_speed = 23;

  // RUN - DONE
	RUN_DATA.meters = 1260;

	RUN_DATA.pace_minutes = 3;
	RUN_DATA.pace_seconds = 12;

	RUN_DATA.timer_hours = 14;
	RUN_DATA.timer_minutes = 69;
	RUN_DATA.timer_seconds = 59;

	RUN_DATA.startFlag = false;

    // TIMER
    timerReset();

  // RUN - DONE
  STEPS_DATA.steps = 12345;
  STEPS_DATA.yesterdaySteps = 54321;
  memset(&STEPS_DATA.goal, 4, 5);
}

/* ******************* */
/* Macro EIR Functions */
/* ******************* */

/**************************************************************************/
/*!
    @brief    Macro function which builds the "GPS" screen on the bitmap.
    @         Currently empty.  Will implement between 11/11/15 - 11/13/15.

    @size     Uses all 12 rows on the bottom 83 lines 
*/
/**************************************************************************/  

// TODO make this look pretty
void buildGPS_LCD(){
    date_time_t date_time;
    date_time_get_current_date_time(&date_time);
    buildTopBar_LCD(&date_time, true);
    clearLines(13,96);
    setCursor(0, 17);

    Cursor.row++;
    transferString("gps");
    Cursor.row++;

    transferString("coords");

    drawLine(32);
    setCursor(0, 36);
    transferSmallNumInt((int)GPS_DATA.longitude[0]-'0');
    transferSmallNumInt((int)GPS_DATA.longitude[1]-'0');
    transferSmallNumInt((int)GPS_DATA.longitude[2]-'0');
    Cursor.row++;
    transferSmallNumInt((int)GPS_DATA.longitude[4]-'0');
    transferSmallNumInt((int)GPS_DATA.longitude[5]-'0');
    transferSpecialChar(GPS_DATA.longitude[6]);
    transferSmallNumInt((int)GPS_DATA.longitude[7]-'0');
    transferSmallNumInt((int)GPS_DATA.longitude[8]-'0');
    transferSmallNumInt((int)GPS_DATA.longitude[9]-'0');
    transferSmallNumInt((int)GPS_DATA.longitude[10]-'0');
    //Cursor.row++;
    //setCursor(11, 42);
    transferChar((char)((int)GPS_DATA.longitude[12]+32));

    setCursor(0, 51);
    transferSmallNumInt((int)GPS_DATA.latitude[0]-'0');
    transferSmallNumInt((int)GPS_DATA.latitude[1]-'0');
    Cursor.row++;
    transferSmallNumInt((int)GPS_DATA.latitude[3]-'0');
    transferSmallNumInt((int)GPS_DATA.latitude[4]-'0');
    transferSpecialChar(GPS_DATA.latitude[5]-'0');
    transferSmallNumInt((int)GPS_DATA.latitude[6]-'0');
    transferSmallNumInt((int)GPS_DATA.latitude[7]-'0');
    transferSmallNumInt((int)GPS_DATA.latitude[8]-'0');
    transferSmallNumInt((int)GPS_DATA.latitude[9]-'0');
    Cursor.row++;
    transferChar((char)((int)GPS_DATA.latitude[11]+32));


    drawLine(65);

    setCursor(0, 69);
    transferString("alt");
    transferSpecialChar(':');
    Cursor.row++;
    transferSmallNumInt(GPS_DATA.altitude);

    setCursor(0, 83);
    transferString("speed");
    transferSpecialChar(':');
    Cursor.row++;
    transferSmallNumInt(GPS_DATA.ground_speed);
}

/**************************************************************************/
/*!
    @brief    Macro function which builds the "TIMER" screen on the bitmap.
    @         Currently empty.  Will implement between 11/11/15 - 11/13/15.

    @size     Uses all 12 rows on the bottom 83 lines  
*/
/**************************************************************************/  

void timerReset()
{
    memset(&TIMER_DATA, 0, sizeof(TIMER_DATA));
}

void timerLap()
{
    CRITICAL_REGION_ENTER();
    TIMER_DATA.lapTimesMin[2] = TIMER_DATA.lapTimesMin[1];
    TIMER_DATA.lapTimesSec[2] = TIMER_DATA.lapTimesSec[1];
    TIMER_DATA.lapTimesTenths[2] = TIMER_DATA.lapTimesTenths[1];
    TIMER_DATA.lapTimesMin[1] = TIMER_DATA.lapTimesMin[0];
    TIMER_DATA.lapTimesSec[1] = TIMER_DATA.lapTimesSec[0];
    TIMER_DATA.lapTimesTenths[1] = TIMER_DATA.lapTimesTenths[0];
    TIMER_DATA.lapTimesMin[0] = TIMER_DATA.timer_minutes;
    TIMER_DATA.lapTimesSec[0] = TIMER_DATA.timer_seconds;
    TIMER_DATA.lapTimesTenths[0] = TIMER_DATA.timer_tenths;
    TIMER_DATA.lapCounter++;
    if (TIMER_DATA.lapCounter >= 100)
    {
        TIMER_DATA.lapCounter = 0;
    }
    CRITICAL_REGION_EXIT();
}

void buildTimer_LCD()
{
    date_time_t date_time;
    date_time_get_current_date_time(&date_time);
    buildTopBar_LCD(&date_time, true);
    clearLines(13,96);
    setCursor(0, 14);

    transferString("stop");
    Cursor.row++;
    transferString("watch");

    setCursor(0, 28);
    if(TIMER_DATA.timer_minutes < 10){
      transferBigNumInt(0);
    }
    transferBigNumInt(TIMER_DATA.timer_minutes);
    transferSpecialBigChar(':');
    if(TIMER_DATA.timer_seconds < 10){
      transferBigNumInt(0);
    }
    transferBigNumInt(TIMER_DATA.timer_seconds);
    setCursor(9, 38);
    transferSpecialChar('.');
    transferSmallNumInt(TIMER_DATA.timer_tenths);
    transferSmallNumInt(0);

    drawLine(54);

    if (TIMER_DATA.lapCounter > 0)
    {
        setCursor(0, 56);
        transferChar('l');
        transferSmallNumInt(TIMER_DATA.lapCounter);
        if (TIMER_DATA.lapCounter < 10){
            transferSpecialChar(':');
        }
        Cursor.row++;
        if(TIMER_DATA.lapTimesMin[0] < 10){
            transferSmallNumInt(0);
        }
        transferSmallNumInt(TIMER_DATA.lapTimesMin[0]);
        transferSpecialChar(':');
        if(TIMER_DATA.lapTimesSec[0] < 10){
            transferSmallNumInt(0);
        }
        transferSmallNumInt(TIMER_DATA.lapTimesSec[0]);
        transferSpecialChar('.');
        transferSmallNumInt(TIMER_DATA.lapTimesTenths[0]);
        transferSmallNumInt(0);
        drawLine(68);
    }

    if (TIMER_DATA.lapCounter > 1)
    {
        setCursor(0, 70);
        transferChar('l');
        transferSmallNumInt(TIMER_DATA.lapCounter-1);
        if (TIMER_DATA.lapCounter-1 < 10){
            transferSpecialChar(':');
        }
        Cursor.row++;
        if(TIMER_DATA.lapTimesMin[1] < 10){
            transferSmallNumInt(0);
        }
        transferSmallNumInt(TIMER_DATA.lapTimesMin[1]);
        transferSpecialChar(':');
        if(TIMER_DATA.lapTimesSec[1] < 10){
            transferSmallNumInt(0);
        }
        transferSmallNumInt(TIMER_DATA.lapTimesSec[1]);
        transferSpecialChar('.');
        transferSmallNumInt(TIMER_DATA.lapTimesTenths[1]);
        transferSmallNumInt(0);
        drawLine(82);
    }

    if (TIMER_DATA.lapCounter > 2)
    {
        setCursor(0, 84);
        transferChar('l');
        transferSmallNumInt(TIMER_DATA.lapCounter-2);
        if (TIMER_DATA.lapCounter-2 < 10){
            transferSpecialChar(':');
        }
        Cursor.row++;

        if(TIMER_DATA.lapTimesMin[2] < 10){
            transferSmallNumInt(0);
        }
        transferSmallNumInt(TIMER_DATA.lapTimesMin[2]);
        transferSpecialChar(':');
        if(TIMER_DATA.lapTimesSec[2] < 10){
            transferSmallNumInt(0);
        }
        transferSmallNumInt(TIMER_DATA.lapTimesSec[2]);
        transferSpecialChar('.');
        transferSmallNumInt(TIMER_DATA.lapTimesTenths[2]);
        transferSmallNumInt(0);
    }
}

/**************************************************************************/
/*!
    @brief    Macro function which builds the "STEPS" screen on the bitmap.
    @         Currently empty.  Will implement between 11/11/15 - 11/13/15.

    @size     Uses all 12 rows on the bottom 83 lines  
*/
/**************************************************************************/  

void buildSteps_LCD()
{
  int i;
    date_time_t date_time;
    date_time_get_current_date_time(&date_time);
    buildTopBar_LCD(&date_time, true);
  clearLines(13,96);
  setCursor(0, 20);

  transferString("steps");

  setCursor(1, 38);
  if(STEPS_DATA.steps < 10000){
    transferBigNumInt(0);
  }
  if(STEPS_DATA.steps < 1000){
    transferBigNumInt(0);
  }
  if(STEPS_DATA.steps < 100){
    transferBigNumInt(0);
  }
  if(STEPS_DATA.steps < 10){
    transferBigNumInt(0);
  }
  transferBigNumInt(STEPS_DATA.steps);

  setCursor(4, 66);
  transferString("of");
  Cursor.row++;
  for (i = 0; i < 5; i++)
  {
    transferSmallNumInt(STEPS_DATA.goal[i]);
    if (STEPS_DATA.oal_digit == i)
    {
      invertBitMap(7+i, 66, 9);
    }

  }

  // drawLine(68);

  // setCursor(0, 70);
  // transferChar('d');
  // transferChar('i');
  // transferChar('s');
  // transferChar('t');
  // transferSpecialChar(':');
  // Cursor.row+=2;
  
  // // THERE IS A BETTER WAY TO DO THIS
  // transferSmallNumInt(RUN_DATA.meters/1000);
  // transferSpecialChar('.');
  // transferSmallNumInt((RUN_DATA.meters%1000)/100);
  // transferSmallNumInt((RUN_DATA.meters%100)/10);

  drawLine(82);

  setCursor(0, 84);
  transferString("last");
  transferSpecialChar(':');
  Cursor.row++;
  if(STEPS_DATA.yesterdaySteps < 10000){
    transferSmallNumInt(0);
  }
  if(STEPS_DATA.yesterdaySteps < 1000){
    transferSmallNumInt(0);
  }
  if(STEPS_DATA.yesterdaySteps < 100){
    transferSmallNumInt(0);
  }
  if(STEPS_DATA.yesterdaySteps < 10){
    transferSmallNumInt(0);
  }
  transferSmallNumInt(STEPS_DATA.yesterdaySteps);
}

/**************************************************************************/
/*!
    @brief    Macro function which builds the "Watch Face" screen on the 
    @         bitmap.

    @size     Uses all 12 rows on all 96 lines  
*/
/**************************************************************************/  

void buildWatchFace_LCD() {
    date_time_t date_time;
    date_time_get_current_date_time(&date_time);
    buildTopBar_LCD(&date_time, false);
/*
    clearLines(1, 11); 

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
    Cursor.row += 5;
    transferString("eir");
*/

    setCursor(1, 37);

    if(date_time.hours < 10){
    transferBigNumInt(0);
    }
    transferBigNumInt(date_time.hours);
    transferBigNumInt(99999); // Big colon
    //transferSpecialBigChar(':');
    if(date_time.minutes < 10){
    transferBigNumInt(0);
    }
    transferBigNumInt(date_time.minutes);

    // setCursor(9, 52);
    // transferSpecialChar(':');
    // if(TIME.seconds < 10){
    //   transferSmallNumInt(0);
    // }
    // transferSmallNumInt(TIME.seconds);

    // TODO Make date dynamic
    setCursor(1, 82);
    transferString(date_time.day_str);
    Cursor.row++;
    transferString(date_time.month_str);
    Cursor.row++;
    transferSmallNumInt(date_time.day_num);
}

/**************************************************************************/
/*!
    @brief    Macro function which builds the "RUN" screen on the bitmap.
    @         Currently is not dynamic. Dynamics will be implemented
    @         between 11/11/15 - 11/13/15.

    @size     Uses all 12 rows on the bottom 83 lines 
*/
/**************************************************************************/

void buildRun_LCD(){
    date_time_t date_time;
    date_time_get_current_date_time(&date_time);
    buildTopBar_LCD(&date_time, true);

  clearLines(13,96);
  setCursor(0, 14);

  transferString("run");
  Cursor.row++;
  transferString("time");

  setCursor(0, 28);
  if(RUN_DATA.timer_hours < 10){
    transferBigNumInt(0);
  }
  transferBigNumInt(RUN_DATA.timer_hours);
  transferSpecialBigChar(':');
  if(RUN_DATA.timer_minutes < 10){
    transferBigNumInt(0);
  }
  transferBigNumInt(RUN_DATA.timer_minutes);
  setCursor(9, 38);
  transferSpecialChar(':');
  if(RUN_DATA.timer_seconds < 10){
    transferSmallNumInt(0);
  }
  transferSmallNumInt(RUN_DATA.timer_seconds);

  setCursor(5,52);
  if(RUN_DATA.startFlag)
  {
  	transferString("running");
  }
  else
  {
  	transferString("stopped");
  }

  drawLine(68);

  setCursor(0, 70);
  transferString("dist");
  transferSpecialChar(':');
  Cursor.row+=2;
  
  // THERE IS A BETTER WAY TO DO THIS
  transferSmallNumInt(RUN_DATA.meters/1000);
  transferSpecialChar('.');
  transferSmallNumInt((RUN_DATA.meters%1000)/100);
  transferSmallNumInt((RUN_DATA.meters%100)/10);

  drawLine(82);

  setCursor(0, 84);
  transferString("pace");
  transferSpecialChar(':');
  Cursor.row+=2;
  transferSmallNumInt(RUN_DATA.pace_minutes);
  transferSpecialChar(':');
  if (RUN_DATA.pace_seconds < 10)
  {
    transferSmallNumInt(0);
  }
  transferSmallNumInt(RUN_DATA.pace_seconds);
}

  /**************************************************************************/
/*!
    @brief    Macro function which builds the "TOP BAR" portion of the bitmap.
    @         Currently is not dynamic. Dynamics will be implemented
    @         between 11/11/15 - 11/13/15.

    @size     Uses all 12 rows on the top 13 lines 
*/
/**************************************************************************/

void buildTopBar_LCD(date_time_t * date_time_ptr, bool time){
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
        transferChar('c');    // Took this out for aesthetics
    } else {
        Cursor.row += 2;
    }
    Cursor.row += 3;

    if (time) {
        if (date_time_ptr->hours < 10){
            transferSmallNumInt(0);
        }
        transferSmallNumInt(date_time_ptr->hours);
        transferSpecialChar(':');
        if (date_time_ptr->minutes < 10){
            transferSmallNumInt(0);
        }
        transferSmallNumInt(date_time_ptr->minutes);
    } else {
        Cursor.row += 2;
        transferString("eir");
    }
}
