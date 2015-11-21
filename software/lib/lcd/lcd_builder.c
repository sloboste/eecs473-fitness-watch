#include <stdint.h>
#include <stdbool.h>

#include "nrf_gpio.h"

#include "lcd_driver.h"
#include "lcd_builder.h"
#include "spi_driver.h"
#include "charData.h"


/**************************************************************************/
/*!
    @brief    Initializes the data structures for the peripherals. The 
    @         current value and data types are not necessarily correct
*/
/**************************************************************************/  

void initStructs(){
  // GPS
  GPS_DATA.longitude = 0;
  GPS_DATA.latitude = 0;
  GPS_DATA.altitude = 0;
  GPS_DATA.velocity =0;

  // RUN
	RUN_DATA.meters = 1260;

	RUN_DATA.pace_minutes = 3;
	RUN_DATA.pace_seconds = 12;

	RUN_DATA.timer_hours = 14;
	RUN_DATA.timer_minutes = 69;
	RUN_DATA.timer_seconds = 59;

	RUN_DATA.startFlag = false;

  // TIMER
  TIMER_DATA.lapTimes[0] = 0;
  TIMER_DATA.lapTimes[1] = 0;
  TIMER_DATA.lapTimes[2] = 0;
  TIMER_DATA.lapTimes[3] = 0;
  TIMER_DATA.timer = 0;
  TIMER_DATA.numLaps = 0;

    // TIME FIXME remove
    TIME.hours = 23;
    TIME.minutes = 38;
    TIME.seconds = 3;

  // RUN
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

  void buildGPS_LCD(){

  }

/**************************************************************************/
/*!
    @brief    Macro function which builds the "TIMER" screen on the bitmap.
    @         Currently empty.  Will implement between 11/11/15 - 11/13/15.

    @size     Uses all 12 rows on the bottom 83 lines  
*/
/**************************************************************************/  

void buildTimer_LCD();

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
  buildTopBar_LCD();
  clearLines(13,96);
  setCursor(0, 20);

  transferChar('s');
  transferChar('t');
  transferChar('e');
  transferChar('p');
  transferChar('s');

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
  transferChar('o');
  transferChar('f');
  Cursor.row++;
  for (i = 0; i < 5; i++)
  {
    transferSmallNumInt(STEPS_DATA.goal[i]);
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
  transferChar('l');
  transferChar('a');
  transferChar('s');
  transferChar('t');
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
  //clearDisplay();
  setCursor(0,1);
  transferBatteryLevel(3);
  transferSpecialChar('&');
  Cursor.row+=6;
  transferChar('e');
  transferChar('i');
  transferChar('r');

  setCursor(1, 37);

  if(TIME.hours < 10){
    transferBigNumInt(0);
  }
  transferBigNumInt(TIME.hours);
  transferBigNumInt(99999); // Big colon
  //transferSpecialBigChar(':');
  if(TIME.minutes < 10){
    transferBigNumInt(0);
  }
  transferBigNumInt(TIME.minutes);

  // setCursor(9, 52);
  // transferSpecialChar(':');
  // if(TIME.seconds < 10){
  //   transferSmallNumInt(0);
  // }
  // transferSmallNumInt(TIME.seconds);

    // TODO Make date dynamic
  setCursor(1, 82);
  transferChar('s');
  transferChar('a');
  transferChar('t');
  Cursor.row++;
  transferChar('d');
  transferChar('e');
  transferChar('c');
  Cursor.row++;
  transferSmallNumInt(12);


  // NOTE: added this for demo to show step count
  // drawLine(82);
  // setCursor(0, 84);
  // transferChar('s');
  // transferChar('t');
  // transferChar('e');
  // transferChar('p');
  // transferChar('s');
  // transferSpecialChar(':');
  // Cursor.row++;
  // transferSmallNumInt(0);
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
	buildTopBar_LCD();
  clearLines(13,96);
  setCursor(0, 14);

  transferChar('t');
  transferChar('i');
  transferChar('m');
  transferChar('e');
  transferChar('r');

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
  	transferChar('r');
	  transferChar('u');
	  transferChar('n');
	  transferChar('n');
	  transferChar('i');
    transferChar('n');
    transferChar('g');
  }
  else
  {
  	transferChar('s');
	  transferChar('t');
	  transferChar('o');
	  transferChar('p');
    transferChar('p');
    transferChar('e');
    transferChar('d');
  }

  drawLine(68);

  setCursor(0, 70);
  transferChar('d');
  transferChar('i');
  transferChar('s');
  transferChar('t');
  transferSpecialChar(':');
  Cursor.row+=2;
  
  // THERE IS A BETTER WAY TO DO THIS
  transferSmallNumInt(RUN_DATA.meters/1000);
  transferSpecialChar('.');
  transferSmallNumInt((RUN_DATA.meters%1000)/100);
  transferSmallNumInt((RUN_DATA.meters%100)/10);

  drawLine(82);

  setCursor(0, 84);
  transferChar('p');
  transferChar('a');
  transferChar('c');
  transferChar('e');
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

void buildTopBar_LCD(){
  clearLines(1,11);
  drawLine(12);
  setCursor(0,1);
  transferBatteryLevel(3);
  Cursor.row++;
  transferChar('e');
  transferChar('i');
  transferChar('r');
  Cursor.row++;

  if(TIME.hours < 10){
    transferSmallNumInt(0);
  }
  transferSmallNumInt(TIME.hours);
  transferSpecialChar(':');
  if(TIME.minutes < 10){
    transferSmallNumInt(0);
  }
  transferSmallNumInt(TIME.minutes);
}
