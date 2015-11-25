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


//SCL 8
//SDA 9

int main(void) {
    uint16_t per = 0;
    uint8_t led = 18;
    uint8_t led2 = 19;
    // Initialization
    led_init(led);
    led_off(led);
    led_init(led2);
    led_off(led2);
    //nrf_gpio_pin_dir_set(11, NRF_GPIO_PIN_DIR_OUTPUT);
    twi_master_sw_init();
    int count = 0;
    //nrf_delay_ms(500);
    while(1){
        // set_SDA();
        // set_SCL();
        // nrf_delay_ms(1);
        // i2c_start_cond();
        // i2c_write_bit(1);
        // i2c_stop_cond();
       //nrf_gpio_pin_set(11);


        //nrf_delay_ms(1);
        //led_on(led2);
        per = getSOC();
        nrf_delay_ms(2);

        count++;

        if (count % 500 == 0)
            nrf_gpio_pin_toggle(led);
        //uint8_t led = 18;
        //led_on(led);
        //nrf_delay_ms(500);
        //led_off(led);
        //nrf_delay_ms(500);
        //led_on(led);
        //nrf_delay_ms(10);
        // if(per > 50 && per < 60){
          // while (1) {
            // led_on(led);
            // nrf_delay_ms(750);
            // led_off(led);
            // nrf_delay_ms(250);
            //nrf_gpio_pin_clear(11);
           // nrf_delay_ms(1);
        // }

    }
}









