/*
 *
 */

#ifndef LCD_BUILDER_H
#define LCD_BUILDER_H

#include <stdint.h>
#include <stdint.h>

#include "lcd_driver.h"

/* ************* */
/* Struct Setups */
/* ************* */

uint8_t lcd_builder_bluetooth_state;

struct time_struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t milli;  // FIXME data type
} TIME;

struct GPS_LCD_DATA {
   uint32_t ground_speed;
   uint32_t altitude;
   char * latitude;
   char * longitude;
}GPS_DATA;

struct RUN_LCD_DATA {
    uint16_t meters;

    uint8_t pace_minutes;
    uint8_t pace_seconds;

    uint8_t timer_hours;
    uint8_t timer_minutes;
    uint8_t timer_seconds;

    bool startFlag;
}RUN_DATA;

struct STEPS_LCD_DATA {
    uint32_t steps;
    uint32_t yesterdaySteps;
    uint8_t goal[5];
}STEPS_DATA;

struct TIMER_LCD_DATA {
    uint8_t lapTimesMin[3];
    uint8_t lapTimesSec[3];
    uint8_t lapTimesTenths[3];

    uint8_t timer_minutes;
    uint8_t timer_seconds;
    uint8_t timer_tenths;
}TIMER_DATA;

struct TOP_BAR_DATA {
    int time;
    uint8_t batLevel;
    bool flag;
}TIMER_BAR;


/* ******************* */
/* Macro EIR Functions */
/* ******************* */

void buildTopBar_LCD();

void buildGPS_LCD();

void timerLap();
void timerReset();
void buildTimer_LCD();

void buildSteps_LCD();

void buildRun_LCD();

void buildWatchFace_LCD();

#endif
