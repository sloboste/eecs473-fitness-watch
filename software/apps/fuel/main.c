/*
 * LED Blink App
 */

// Standard Libraries
#include <stdint.h>
#include <stdbool.h>

// Nordic Libraries
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "app_timer.h"

// Platform, Peripherals, Devices, Services
//#include "blees.h"
#include "led.h"

#include "i2c.h"

/*******************************************************************************
 *   DEFINES
 ******************************************************************************/
 #include "nrf_drv_config.h"

// NOTE: added this
#define LED_PIN 18 // led 0 on the green board

static app_timer_id_t test_timer;

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {

    NVIC_SystemReset();
}

/**@brief Function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num     Line number of the failing ASSERT call.
 * @param[in] p_file_name  File name of the failing ASSERT call.
 */
 void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name) {
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}


int main(void) {
    uint16_t per = 0;;
    uint8_t led = 18;
    // Initialization
    led_init(led);
    led_on(led);
    set_SDA();
    set_SCL();
    while(1){
        // set_SDA();
        // set_SCL();
        // nrf_delay_ms(1);
        // i2c_start_cond();
        // i2c_write_bit(1);
        // i2c_stop_cond();
        nrf_delay_ms(3);
        per = getSOC();
        // if(per > 50 && per < 60){
          // while (1) {
          //   led_on(led);
          //   nrf_delay_ms(750);
          //   led_off(led);
          //   nrf_delay_ms(250);
        // }

    }
}









