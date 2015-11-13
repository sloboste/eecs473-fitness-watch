
#ifndef TIME_KEEPER_H
#define TIME_KEEPER_H

//#include "globals.h"

typedef struct time_struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t milli;
} rtc_time_t;

// Required RTC working clock RTC_FREQUENCY Hertz. Changable.
#define RTC_FREQUENCY             (800UL)
// Get Compare event COMPARE_TIME seconds after the counter starts from 0
#define COMPARE_COUNTERTIME       (1UL)
#define LFCLK_FREQUENCY           (32768UL)
// f = LFCLK/(prescaler + 1)
#define COUNTER_PRESCALER         ((LFCLK_FREQUENCY/RTC_FREQUENCY) - 1)

extern void rtc_config(void);

extern void rtc_start();

extern void rtc_stop();

extern void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t milli);

extern rtc_time_t rtc_get_time();

#endif
