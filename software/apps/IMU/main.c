
// // void GPIOTE_IRQHandler(void)
// // {
// //     // This handler will be run after wakeup from system ON (GPIO wakeup)
// //     if(NRF_GPIOTE->EVENTS_PORT)
// //     {
// //         NRF_GPIOTE->EVENTS_PORT = 0;
        
// //         //nrf_gpio_pin_toggle(LED_PIN);

// //     }
// // }




// #include "nordic_common.h"
// #include "softdevice_handler.h"
// #include "led.h"
// #include "nrf_delay.h"
// #include "SEGGER_RTT.h"
// #include "twi_master.h"
// //#include "mpu.h"
// #include "inv_mpu.h"
// #include "inv_mpu_dmp_motion_driver.h"


// #define LED_PIN 18 // led 0 on the green board

// void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
//     NVIC_SystemReset();
// }

// static void power_manage (void) { 
//     uint32_t err_code = sd_app_evt_wait();
//     APP_ERROR_CHECK(err_code);
// }

// // bool i2c_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t const *data){
// // 	uint8_t w2_data[2];

// //     w2_data[0] = reg_addr;
// //     w2_data[1] = *data;
// //     return twi_master_transfer(slave_addr, w2_data, length, TWI_ISSUE_STOP);
// // }

// int main(void) {
//     led_init(LED_PIN);
//     led_on(LED_PIN);
    
//     twi_master_init();

// 	//uint8_t addr = (uint8_t)(0x68U << 1);
// 	//uint8_t w2_data[2];
//     //w2_data[0] = 0x68U;
//     //w2_data[1] = 0x07U;
//     //twi_master_transfer(addr, w2_data, 2, TWI_ISSUE_STOP);
//  //    bool aks = false;
// 	// aks = mpu6050_init(0x68U);
// 	// SEGGER_RTT_printf(0, "%d\r\n", aks);

//     SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
//     SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n\r\n");


//     uint8_t data[6];
//     data[0] = (1 >> 8) & 0xff;
//     data[1] = (1) & 0xff;
//     data[2] = (1 >> 8) & 0xff;
//     data[3] = (1) & 0xff;
//     data[4] = (1 >> 8) & 0xff;
//     data[5] = (1) & 0xff;

//     i2c_write(0x68, 0x06, 5, data);
    
//  //    mpu_select_device(0);
//  //    mpu_init_structures();
    
//  //    int ret = mpu_init(NULL);
// 	// SEGGER_RTT_printf(0, "A%d\r\n", ret);


// 	// ret = mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);
// 	// //SEGGER_RTT_printf(0, "B%d\r\n", ret);
//  //    ret = mpu_set_gyro_fsr(2000);
//  //    //SEGGER_RTT_printf(0, "C%d\r\n", ret);
//  //    ret = mpu_set_accel_fsr(2);
// 	// //SEGGER_RTT_printf(0, "D%d\r\n", ret);
//  //    mpu_get_power_state((unsigned char *)&ret);
// 	// //SEGGER_RTT_printf(0, "E%d\r\n", ret);
//  //    ret = mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL);
// 	// //SEGGER_RTT_printf(0, "F%d\r\n", ret);
	
// 	// dmp_select_device(0);
// 	// dmp_init_structures();

// 	// ret = dmp_load_motion_driver_firmware();
// 	// SEGGER_RTT_printf(0, "G%d\r\n", ret);
// 	// ret = dmp_set_fifo_rate(200);
// 	// SEGGER_RTT_printf(0, "H%d\r\n", ret);
// 	// ret = mpu_set_dmp_state(1);
// 	// SEGGER_RTT_printf(0, "I%d\r\n", ret);
// 	// ret = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_SEND_CAL_GYRO|DMP_FEATURE_GYRO_CAL|DMP_FEATURE_PEDOMETER|DMP_FEATURE_TAP );
// 	// SEGGER_RTT_printf(0, "J%d\r\n", ret);

//  //    int success = mpu_reg_dump();
//  //    SEGGER_RTT_printf(0, "K%d\r\n", success);

//     int i = 0;
//     while (1) {
//         led_toggle(LED_PIN);
//         nrf_delay_ms(3000); //wait 3 seconds
//         ++i;
//      }
// }

#include "nordic_common.h"
#include "softdevice_handler.h"
#include "led.h"
#include "nrf_delay.h"
#include "SEGGER_RTT.h"
#include "twi_master.h"
#include "mpu.h"
//#include "nrf_drv_config.h"
//#include "nrf_drv_twi.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


#define LED_PIN 18 // led 0 on the green board

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    NVIC_SystemReset();
}

static void power_manage (void) { 
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

// bool i2c_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t const *data){
// 	uint8_t w2_data[2];

//     w2_data[0] = reg_addr;
//     w2_data[1] = *data;
//     return twi_master_transfer(slave_addr, w2_data, length, TWI_ISSUE_STOP);
// }

int main(void) {
    led_init(LED_PIN);
    led_on(LED_PIN);

    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
    SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n\r\n");

    //i2c_init();

    //
	// mpu_select_device(0);
	// mpu_init_structures();

	// int ret = mpu_init(NULL);
	// SEGGER_RTT_printf(0, "A%d\r\n", ret);


	// ret = mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);
	// SEGGER_RTT_printf(0, "B%d\r\n", ret);
	// ret = mpu_set_gyro_fsr(2000);
	// SEGGER_RTT_printf(0, "C%d\r\n", ret);
	// ret = mpu_set_accel_fsr(2);
	// SEGGER_RTT_printf(0, "D%d\r\n", ret);
	// mpu_get_power_state((unsigned char *)&ret);
	// SEGGER_RTT_printf(0, "E%d\r\n", ret);
	// ret = mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL);
	// SEGGER_RTT_printf(0, "F%d\r\n", ret);

	// dmp_select_device(0);
	// dmp_init_structures();

	// ret = dmp_load_motion_driver_firmware();
	// SEGGER_RTT_printf(0, "G%d\r\n", ret);
	// ret = dmp_set_fifo_rate(200);
	// SEGGER_RTT_printf(0, "H%d\r\n", ret);
	// ret = mpu_set_dmp_state(1);
	// SEGGER_RTT_printf(0, "I%d\r\n", ret);
	// ret = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_SEND_CAL_GYRO|DMP_FEATURE_GYRO_CAL|DMP_FEATURE_PEDOMETER|DMP_FEATURE_TAP );
	// SEGGER_RTT_printf(0, "J%d\r\n", ret);

	//

	mympu_open(200);
	//int success = mpu_reg_dump();
	//SEGGER_RTT_printf(0, "K%d\r\n", success);

    int i = 0;
    while (1) {
    	mympu_update();
    	//SEGGER_RTT_printf(0, ":%d\r\n", mympu.steps);
        //led_toggle(LED_PIN);
        //nrf_delay_ms(1000); //wait 3 seconds
        ++i;
     }
}