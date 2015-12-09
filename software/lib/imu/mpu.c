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

#include <stdint.h>
#include <inttypes.h>
#include "mpu.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include <math.h>
#include "nrf_delay.h"

#define DMP_INT_GESTURE     (0x01)
#define DMP_INT_CONTINUOUS  (0x02)

//unsigned int tap_count = 0;
//unsigned long time = 0;

/*void tap_detect(unsigned char direction, unsigned char count){
	tap_count += count;
	SEGGER_RTT_printf(0, "TAP DETECTED%d\r\n", tap_count);
	return;
}*/

void pedometer_init()
{
  	mpu_select_device(0);
   	mpu_init_structures();
	mpu_init(NULL);
	mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL); 
    //mpu_get_power_state((unsigned char *)&ret);
	dmp_load_motion_driver_firmware();
	dmp_set_fifo_rate(200);
	mpu_set_dmp_state(1);
	dmp_enable_feature(DMP_FEATURE_PEDOMETER|DMP_FEATURE_TAP);
	//dmp_register_tap_cb(&tap_detect);
	dmp_set_interrupt_mode(DMP_INT_GESTURE);
	return 0;
}

uint32_t pedometer_get_steps()
{
	return dmp_get_pedometer_step_count();
}

void pedometer_reset_steps()
{
	//try a few times to reset even if race condition occurs
	dmp_set_pedometer_step_count(0);
	dmp_set_pedometer_step_count(0);
	dmp_set_pedometer_step_count(0);
	return;
}

/*
uint32_t get_walktime(){
	unsigned long time = 0;
	dmp_get_pedometer_walk_time(&time);
	return (uint32_t)time;	
}

void reset_walktime(){
	dmp_set_pedometer_walk_time(0);
	dmp_set_pedometer_walk_time(0);
	dmp_set_pedometer_walk_time(0);
	return;	
}
*/

// int mympu_update() {
// 	//SEGGER_RTT_printf(0, ":%d\r\n", tap_count);

// 	//mpu_get_int_status(status);
// 	//SEGGER_RTT_printf(0, "RET: %d\r\n", *status);

// 	// do {
// 	// 	ret = dmp_read_fifo(&fifoCount);
// 	// 	 // will return:
// 	// 		// 0 - if ok
// 	// 		// 1 - no packet available
// 	// 		// 2 - if BIT_FIFO_OVERFLOWN is set
// 	// 		// 3 - if frame corrupted
// 	// 	 //       <0 - if error
// 	// 	//SEGGER_RTT_printf(0, "RET: %d\r\n", ret);
// 	// 	if (ret!=0) return ret; 
// 	// } while (fifoCount>1);
// 	//dmp_get_pedometer_walk_time(&time);
// 	//nrf_delay_ms(3000); //wait 3 seconds
// 	return 0;
// }

