#ifndef GLOBALS_H
#define GLOBALS_H
#include <stdint.h>

typedef struct current_time_struct{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t milli;
} current_time_t;

extern current_time_t curr_time;

#endif