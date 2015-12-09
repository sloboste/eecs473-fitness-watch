/* This file is part of eecs473-fitness-watch.
 *   
 * The code / board schematics created by the authors of eecs473-fitness-watch
 * are free software/hardware: you can redistribute them and/or modify them
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * The code / board schematics are distributed in the hope that they will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the code / board schematics.  If not, see <http://www.gnu.org/licenses/>.
 *
 * All code / schematics not created by the authors of this repository fall
 * under their original licenses.
 */

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

