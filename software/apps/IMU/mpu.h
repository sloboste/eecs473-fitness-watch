#ifndef MPU_H
#define MPU_H

//extern unsigned long steps;

int mympu_open(unsigned int rate);
int mympu_update();
unsigned long get_steps();
void tap_detect(unsigned char direction, unsigned char count);
#endif

