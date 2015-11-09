#include "mpu.h"
#include <stdint.h>
#include <inttypes.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "SEGGER_RTT.h"
#include <math.h>
#include "nrf_delay.h"

#define DMP_INT_GESTURE     (0x01)
#define DMP_INT_CONTINUOUS  (0x02)

unsigned int tap_count = 0;
unsigned short *rate;
unsigned long time = 0;
short *status;

static int ret;
static unsigned char fifoCount;


void tap_detect(unsigned char direction, unsigned char count){
	tap_count += count;
	SEGGER_RTT_printf(0, "TAP DETECTED%d\r\n", tap_count);
	return;
}

int mympu_open(unsigned int rate) {
  	mpu_select_device(0);
   	mpu_init_structures();
	ret = mpu_init(NULL);
	ret = mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL); 
    mpu_get_power_state((unsigned char *)&ret);
	ret = dmp_load_motion_driver_firmware();
	ret = dmp_set_fifo_rate(rate);
	ret = mpu_set_dmp_state(1);
	ret = dmp_enable_feature(DMP_FEATURE_PEDOMETER|DMP_FEATURE_TAP);
	ret = dmp_register_tap_cb(&tap_detect);
	ret = dmp_set_interrupt_mode(DMP_INT_GESTURE);

	return 0;
}

unsigned long get_steps(){
	unsigned long steps = dmp_get_pedometer_step_count();
	SEGGER_RTT_printf(0, "Steps: %d\r\n", steps);
	return steps;
}

int mympu_update() {
	//SEGGER_RTT_printf(0, ":%d\r\n", tap_count);

	//mpu_get_int_status(status);
	//SEGGER_RTT_printf(0, "RET: %d\r\n", *status);

	// do {
	// 	ret = dmp_read_fifo(&fifoCount);
	// 	 // will return:
	// 		// 0 - if ok
	// 		// 1 - no packet available
	// 		// 2 - if BIT_FIFO_OVERFLOWN is set
	// 		// 3 - if frame corrupted
	// 	 //       <0 - if error
	// 	//SEGGER_RTT_printf(0, "RET: %d\r\n", ret);
	// 	if (ret!=0) return ret; 
	// } while (fifoCount>1);
	dmp_get_pedometer_walk_time(&time);
	SEGGER_RTT_printf(0, "RET: %d\r\n", time);
	nrf_delay_ms(3000); //wait 3 seconds
	return 0;
}

