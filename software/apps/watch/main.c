#include <stdbool.h>
#include <stdint.h>

#include "nrf_drv_clock.h"// FIXME remove when using ble

#include "nrf.h"
#include "nrf_gpio.h"
#include "softdevice_handler.h"

#include "nrf_delay.h"

#include "blue_dev_board.h"
//#include "green_dev_board.h"

#include "timer_config.h"
#include "scheduler_config.h"

/*
#include "ble_config.h"
#include "heart_rate_service.h"
#include "battery_service.h"
#include "gps_service.h"
#include "ped_service.h"
*/

#include "mpu.h"
//#include "inv_mpu.h" // FIXME remove
//#include "SEGGER_RTT.h" // FIXME remove

//#include "gps.h"

#include "spi_driver.h"
#include "lcd_driver.h"

#include "globals.h"
#include "time_keeper.h"


static rtc_time_t time;

/**
 * Handle when an error ocurrs during runtime.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num,
                       const uint8_t * p_file_name)
{
    // On assert, the system can only recover on reset.
    NVIC_SystemReset();
}


/**
 * Initialize the gpio pins based on the dev board we are using.
 */
void gpio_init(void)
{
    nrf_gpio_cfg_output(PIN_LED_1);
    nrf_gpio_cfg_output(PIN_LED_2);
    nrf_gpio_cfg_output(PIN_LED_3);
    nrf_gpio_cfg_output(PIN_LED_4);

    nrf_gpio_pin_set(PIN_LED_1);
    nrf_gpio_pin_set(PIN_LED_2);
    nrf_gpio_pin_set(PIN_LED_3);
    nrf_gpio_pin_set(PIN_LED_4);
}

static void demo_time_steps()
{
    time = rtc_get_time();
    buildWatchFace_LCD(&time, get_steps());
    refresh();
}


static app_timer_id_t timer_id_1hz;

void task_1hz(void * arg_ptr)
{
    static int i = 0;
    ++i;

    nrf_gpio_pin_toggle(PIN_LED_1);

    demo_time_steps();

    //time = rtc_get_time();
    //buildRun_LCD();
    //buildTopBar_LCD();
    //buildWatchFace_LCD(&time);
    //refresh();

    /*
    static uint8_t battery_level = 100;
    bas_update(battery_level--);

    static uint16_t heart_rate_bpm = 1000;
    hrs_update(heart_rate_bpm++);

    static uint32_t step_count = 0;
    //ble_ped_update_step_count(step_count++);
    step_count = get_steps();
    ble_ped_update_step_count(step_count);
    */

}


/**
 * Watch app.
 */
int main(void)
{
    // FIXME remove
    //SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
    //SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n\r\n"); 

    // FIXME won't need this when using ble
    uint32_t err_code = nrf_drv_clock_init(NULL);                               
    APP_ERROR_CHECK(err_code);                                                  
    nrf_drv_clock_lfclk_request();                                              
    //-----

    // Init real time counter
    rtc_config();
    rtc_start();

    // Init gpio pins
    gpio_init();

    // Init application timer and scheduler
    timers_init(true);
    scheduler_init();

    // Init BLE
    //ble_init();

    // Init IMU
    mympu_open(200);

    // Init LCD
    spi_init();
    clearDisplay();

    // TODO move this
    app_timer_create(&timer_id_1hz, APP_TIMER_MODE_REPEATED, task_1hz);
    app_timer_start(timer_id_1hz, APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER),
                    app_timer_evt_schedule);

    // Begin BLE advertisement
    //advertising_start();

    // Main loop
    while (1) {
        app_sched_execute();
        //sd_app_evt_wait();         
        __WFI(); // FIXME remove when using ble
    }
}
