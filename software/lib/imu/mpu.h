#ifndef MPU_H
#define MPU_H

//DONT FORGET TO CHOOSE PINS IN nrf_drv_Config

/**
 * Setup the pedometer. 
 */
extern void pedometer_init();

/**
 * Get the current number of steps recorded by the pedometer.
 *
 * Returns the number of steps as a uint32_t.
 */
extern uint32_t pedometer_get_steps();

/**
 * Reset the current number of steps to 0.
 */
extern void pedometer_reset_steps();

//returns walk time in ms
//extern uint32_t pedometer_get_walktime();

//will most likely set walk time to 0
//extern void pedometer_reset_walktime();

//void tap_detect(unsigned char direction, unsigned char count);
#endif

