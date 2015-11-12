// Standard Libraries
#include <stdint.h>
#include <stdbool.h>

// Nordic Libraries
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "app_timer.h"

#include "led.h"

#include "spi_driver.h"
#include "time_keeper.h"
#include "lcd_driver.h"

#include "nrf_drv_config.h"



/*******************************************************************************
 *   DEFINES
 ******************************************************************************/

#define GPIO_TOGGLE_TICK_EVENT    (21)                       /*!< Pin number to toggle when there is a tick event in RTC */
#define GPIO_TOGGLE_COMPARE_EVENT (22)     

/*******************************************************************************
 *   HANDLERS AND CALLBACKS
 ******************************************************************************/

/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    // APPL_LOG("[APPL]: ASSERT: %s, %d, error 0x%08x\r\n", p_file_name, line_num, error_code);
    // nrf_gpio_pin_set(ASSERT_LED_PIN_NO);

    // This call can be used for debug purposes during development of an application.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Un-comment the line below to use.
    // ble_debug_assert_handler(error_code, line_num, p_file_name);

    // On assert, the system can only recover with a reset.
    NVIC_SystemReset();
}

/// TESTING

static void gpio_config(void)
{
  nrf_gpio_cfg_output(GPIO_TOGGLE_TICK_EVENT);
  nrf_gpio_cfg_output(GPIO_TOGGLE_COMPARE_EVENT);

  nrf_gpio_pin_write(GPIO_TOGGLE_TICK_EVENT, 0);
  nrf_gpio_pin_write(GPIO_TOGGLE_COMPARE_EVENT, 0);
}

/*******************************************************************************
 *   MAIN LOOP
 ******************************************************************************/

int main(void) {
    spi_init();
    lfclk_config();
    rtc_config();
    gpio_config();
    NRF_RTC0->TASKS_START = 1;
  set_time(2, 11, 00,  10);
    clearDisplay();

    nrf_delay_ms(500);

    buildWatchFace_LCD();

    //led_off(LED_PIN);

    while (1) {
    //buildRun_LCD();
    buildWatchFace_LCD();
    refresh();
    }
}
