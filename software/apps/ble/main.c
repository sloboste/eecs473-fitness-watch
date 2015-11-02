#include <stdbool.h>
#include <stdint.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "softdevice_handler.h"

#include "green_dev_board.h"
#include "timer_config.h"
#include "ble_config.h"
#include "heart_rate_service.h"
#include "battery_service.h"
#include "gps_service.h"


#define ADVERTISING_LED                 PIN_LED_0   // On when advertising 
#define CONNECTED_LED                   PIN_LED_1   // On when connected
#define ADVERTISING_BUTTON              PIN_BUTTON_0  // Activate advertising


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
    nrf_gpio_cfg_input(ADVERTISING_BUTTON, GPIO_PIN_CNF_PULL_Pullup);
    nrf_gpio_cfg_output(ADVERTISING_LED);
    nrf_gpio_cfg_output(CONNECTED_LED);
    nrf_gpio_pin_clear(ADVERTISING_LED);
    nrf_gpio_pin_clear(CONNECTED_LED);
}


/**
 * @brief Function for application main entry.
 */
int main(void)
{
    gpio_init();
    timers_init();
    ble_init();

    // Wait for button press
    while (nrf_gpio_pin_read(ADVERTISING_BUTTON)) {}; 

    advertising_start();

    uint32_t gval = 0; // FIXME remove later
    uint8_t gstatus = 0x1; // FIXME remove later
    uint32_t pval = 0; // FIXME remove later
    uint8_t pstatus = 0x1; // FIXME remove later
    while (1) {
        power_manage();

        // FIXME remove later
        bas_update();
        hrs_update();

        ble_gps_update_location(gval++);
        ble_gps_update_speed(gval++);
        gstatus ^= 0x1;
        ble_gps_update_status(gstatus);

        ble_ped_update_step_count(pval--);
        pstatus ^= 0x1;
        ble_ped_update_status(pstatus);
        // ---
    }
}
