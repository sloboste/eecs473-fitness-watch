#ifndef LCD_BUILDER_H

#define LCD_BUILDER_H

#include <stdint.h>

#include "time_keeper.h"
#include "lcd_driver.h"

/* ************* */
/* Struct Setups */
/* ************* */

struct CURSOR {
	uint8_t line;
	uint8_t row;
}Cursor;

struct GPS_LCD_DATA {
	double longitude;
	double latitude;
	int altitude;
	int velocity;
}GPS_DATA;

struct RUN_LCD_DATA {
	double dist;
	double pace;
	int timer;
}RUN_DATA;

struct TIMER_LCD_DATA {
	double lapTimes[4];
	int timer;
	int numLaps;
}TIMER_DATA;

struct TOP_BAR_DATA {
	int time;
	uint8_t batLevel;
	bool flag;
}TIMER_BAR;

/* ******************* */
/* Macro EIR Functions */
/* ******************* */

void buildGPS_LCD();
void buildTimer_LCD();
void buildRun_LCD();
void buildTopBar_LCD();
void buildWatchFace_LCD(rtc_time_t * time_ptr, uint32_t steps);

#endif