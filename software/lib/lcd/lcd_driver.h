#ifndef LCD_DRIVER_H

#define LCD_DRIVER_H

#include "globals.h"
/* ************** */
/* LCD Dimensions */
/* ************** */

#define LCD_WIDTH       (96)
#define LCD_HEIGHT      (96) 

/* *********************** */
/* Potential Struct Setups */
/* *********************** */

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


/* ************** */
/* Init Functions */
/* ************** */

void initCursor();
void initStructs();

/* *********************************** */
/* Transfer, Draw, and Erase Functions */
/* *********************************** */

void drawLine(uint8_t line);
void transferToBitmap(uint8_t data);
void clearLines(uint8_t start, uint8_t end);

void transferChar(char c);
void transferSmallNumInt(int num);
void transferBigNumInt(int num);
void transferSpecialChar(char c);
void transferSpecialBigChar(char c);

void transferBatteryLevel(int num);

/* ***************************** */
/* LCD Transfer Helper Functions */
/* ***************************** */

uint8_t reverseBitOrder(uint8_t MSB);

/* ******************************** */
/* Macro LCD Manipulation Functions */
/* ******************************** */

void refresh(void);
void clearDisplay();

/* ********************** */
/* LCD Mangment Functions */
/* ********************** */

void setCursor(int x, int y);

/* ******************* */
/* Macro EIR Functions */
/* ******************* */

void buildGPS_LCD();
void buildTimer_LCD();
void buildRun_LCD();
void buildTopBar_LCD();
void buildWatchFace_LCD();

/* ************** */
/* Test Functions */
/* ************** */

void charTest();

#endif