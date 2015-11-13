#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "softdevice_handler.h"

#include "nrf_delay.h"

#include "timer_config.h"
#include "ble_config.h"
#include "heart_rate_service.h"
#include "battery_service.h"
#include "gps_service.h"
#include "ped_service.h"

//#include "blue_dev_board.h"
#include "green_dev_board.h"
#include "gps.h"

/*
#define ADVERTISING_LED                 PIN_LED_0   // On when advertising 
#define CONNECTED_LED                   PIN_LED_1   // On when connected
#define ADVERTISING_BUTTON              PIN_BUTTON_0  // Activate advertising
*/


/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    // This call can be used for debug purposes during application development.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Un-comment the line below to use.
    // ble_debug_assert_handler(error_code, line_num, p_file_name);

    // On assert, the system can only recover on reset.
    NVIC_SystemReset();
}


/**@brief Function for doing power management.
 */
static void power_manage(void)
{
    // TODO: maybe move this to BLE config because of the sd_app_event_wait
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}


void gpio_init(void)
{
    /*
    nrf_gpio_cfg_input(ADVERTISING_BUTTON, GPIO_PIN_CNF_PULL_Pullup);
    nrf_gpio_cfg_output(ADVERTISING_LED);
    nrf_gpio_cfg_output(CONNECTED_LED);
    nrf_gpio_pin_clear(ADVERTISING_LED);
    nrf_gpio_pin_clear(CONNECTED_LED);
    */

    //nrf_gpio_cfg_output(PIN_LED_0);
    //nrf_gpio_cfg_output(PIN_LED_1);
    //nrf_gpio_cfg_output(PIN_LED_2);
    //nrf_gpio_pin_clear(PIN_LED_0);
    //nrf_gpio_pin_set(PIN_LED_1);
    //nrf_gpio_pin_set(PIN_LED_2);
}



/**
 * @brief Function for application main entry.
 */
int main(void)
{
    gpio_init();
    timers_init(false);
    ble_init();

    // Wait for button press
    //while (nrf_gpio_pin_read(ADVERTISING_BUTTON)) {}; 

    //gps_init(PIN_LED_1);
    gps_init();
    nrf_delay_ms(1000);
    gps_enable();
    gps_config();

    gps_info_t gps_info;

    advertising_start();

    uint32_t gval = 0; // FIXME remove later
    uint8_t pstatus = 0x1; // FIXME remove later
    uint8_t gstatus = 0x1; // FIXME remove later
    uint32_t pval = 0; // FIXME remove later
    uint8_t battery_level = 100;
    uint16_t heart_rate_bpm = 1000;
    
    char * lat1 = "12 34.5678 N";
    char * lat2 = "90 87.1234 S";
    
    char * long1 = "012 34.5678 E";
    char * long2 = "180 87.1234 W";
    
    //char * info = NULL;
    //uint32_t len;

    int x = 0;
    while (1) {
        x ^= 1;
        //power_manage();
        
        // FIXME remove later
        nrf_delay_ms(500);
        bas_update(battery_level--);
        hrs_update(heart_rate_bpm++);
        
        gps_get_info(&gps_info, GPS_TYPE_GPRMC); 
        ble_gps_update_latitude(gps_info.latitude, 16);
        //ble_gps_update_speed((uint32_t) (gps_info.speed * 1000 + 0.5 ));
        //ble_gps_update_location(gps_info.longitude, 16);

        /*
        if (x) {
            ble_gps_update_latitude(lat1, strlen(lat1));
            ble_gps_update_longitude(long1, strlen(long1));
        } else {
            ble_gps_update_latitude(lat2, strlen(lat2));
            ble_gps_update_longitude(long2, strlen(long2));
        }
        ble_gps_update_speed(gval++);
        gstatus ^= 0x1;
        ble_gps_update_status(gstatus);

        ble_ped_update_step_count(pval--);
        pstatus ^= 0x1;
        ble_ped_update_status(pstatus);
        */
        // ---
    }
}
