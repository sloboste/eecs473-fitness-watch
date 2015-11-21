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
	RUN_DATA.meters = 0;

	RUN_DATA.pace_minutes = 0;
	RUN_DATA.pace_seconds = 0;

	RUN_DATA.timer_hours = 0;
	RUN_DATA.timer_minutes = 0;
	RUN_DATA.timer_seconds = 0;

	RUN_DATA.startFlag = false;

  // TIMER
  TIMER_DATA.lapTimes[0] = 0;
  TIMER_DATA.lapTimes[1] = 0;
  TIMER_DATA.lapTimes[2] = 0;
  TIMER_DATA.lapTimes[3] = 0;
  TIMER_DATA.timer = 0;
  TIMER_DATA.numLaps = 0;
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
    @brief    Macro function which builds the "Watch Face" screen on the 
    @         bitmap.

    @size     Uses all 12 rows on all 96 lines  
*/
/**************************************************************************/  

void buildWatchFace_LCD() {
  //clearDisplay();
  setCursor(0,1);
  transferBatteryLevel(3);
  Cursor.row+=2;

  transferSmallNumInt(11);
  transferSpecialChar('/');
  transferSmallNumInt(13);
  transferSpecialChar('/');
  transferSmallNumInt(15);

  setCursor(0, 42);
  if(TIME.hours < 10){
    transferBigNumInt(0);
  }
  transferBigNumInt(TIME.hours);
  transferSpecialBigChar(':');
  if(TIME.minutes < 10){
    transferBigNumInt(0);
  }
  transferBigNumInt(TIME.minutes);
  setCursor(9, 52);
  transferSpecialChar(':');
  if(TIME.seconds < 10){
    transferSmallNumInt(0);
  }
  transferSmallNumInt(TIME.seconds);

  // NOTE: added this for demo to show step count
  drawLine(82);
  setCursor(0, 84);
  transferChar('s');
  transferChar('t');
  transferChar('e');
  transferChar('p');
  transferChar('s');
  transferSpecialChar(':');
  Cursor.row++;
  transferSmallNumInt(0);
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
  setCursor(9, 52);
  transferSpecialChar(':');
  if(RUN_DATA.timer_seconds < 10){
    transferSmallNumInt(0);
  }
  transferSmallNumInt(RUN_DATA.timer_seconds);


  setCursor(0,54);
  if(RUN_DATA.startFlag)
  {
  	transferChar('s');
	  transferChar('t');
	  transferChar('a');
	  transferChar('r');
	  transferChar('t');
  }
  else
  {
  	transferChar('s');
	  transferChar('t');
	  transferChar('o');
	  transferChar('p');
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
  transferSmallNumInt(TIME.minutes);
  transferSpecialChar(':');
  if(TIME.minutes < 10){
    transferSmallNumInt(0);
  }
  transferSmallNumInt(TIME.seconds);
}
