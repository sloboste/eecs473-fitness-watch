#include "nordic_common.h"
#include "softdevice_handler.h"
#include "led.h"
#include "nrf_delay.h"
#include "SEGGER_RTT.h"
#include "twi_master.h"
#include "mpu.h"

#define LED_PIN 18 // led 0 on the green board

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    NVIC_SystemReset();
}

static void power_manage (void) { 
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

int main(void) {
    led_init(LED_PIN);
    led_on(LED_PIN);

    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
    SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n\r\n");

	mympu_open(200); //run this once

    while (1) {
    	mympu_update(); //schedule this
    	//SEGGER_RTT_printf(0, ":%d\r\n", mympu.steps);
        //led_toggle(LED_PIN);
        //nrf_delay_ms(3000); //wait 3 seconds
        //SEGGER_RTT_printf(0, "Steps: %d\r\n", steps);
        //SEGGER_RTT_printf(0, "Steps: %d\r\n", get_steps());
     }
}