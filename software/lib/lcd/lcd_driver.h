#ifndef LCD_DRIVER_H

#define LCD_DRIVER_H

//#include "globals.h"
#include <stdint.h>

#include "time_keeper.h"


/* ************** */
/* LCD Dimensions */
/* ************** */

#define LCD_WIDTH       (96)
#define LCD_HEIGHT      (96) 

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

/* ************** */
/* Test Functions */
/* ************** */

void charTest();

#endif
