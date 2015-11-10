#ifndef MPU_H
#define MPU_H

//DONT FORGET TO CHOOSE PINS IN nrf_drv_Config

//Run this at the start. Rate should be 200
int mympu_open(unsigned int rate);

//returns number of steps
uint32_t get_steps();

//will most likely set steps to 0
void reset_steps();

//returns walk time in ms
uint32_t get_walktime();

//will most likely set walk time to 0
void reset_walktime();

//void tap_detect(unsigned char direction, unsigned char count);
#endif

