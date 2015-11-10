// USE "make flashRTT"
// kill JLINKEXE PID after use

#include "nordic_common.h"
#include "softdevice_handler.h"
#include "led.h"
#include "nrf_delay.h"
#include "SEGGER_RTT.h"

#define LED_PIN 18 // led 0 on the green board

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    NVIC_SystemReset();
}
/*
The downside of not using a timer means that we cant call power_manage
to sleep the processor, To use the power_mange feature we must create an application
event like an interrupt. If we use the schedule, it should generate those events
*/
static void power_manage (void) { 
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

int main(void) {
    led_init(LED_PIN);
    led_on(LED_PIN);

    //more info about RTT: https://devzone.nordicsemi.com/tutorials/6/debugging-with-real-time-terminal/
    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
    SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n\r\n");
    int i = 0;
    while (1) {
        SEGGER_RTT_printf(0, "%d\r\n", i);
        led_toggle(LED_PIN);
        nrf_delay_ms(3000); //wait 3 seconds
        ++i;
     }
}