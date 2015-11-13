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

#include "ble_config.h"
#include "ped_service.h"
/*
#include "heart_rate_service.h"
#include "battery_service.h"
#include "gps_service.h"
*/

#include "mpu.h"
//#include "inv_mpu.h" // FIXME remove
//#include "SEGGER_RTT.h" // FIXME remove

#include "gps.h"

#include "spi_driver.h"
#include "lcd_driver.h"

#include "app_util_platform.h" // CRITICAL SECTION
#include "time_keeper.h"


static rtc_time_t time;
static gps_info_t gps_info;

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

static void set_time(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    CRITICAL_REGION_ENTER();                                                    
    time.seconds = seconds;
    time.minutes = minutes;
    time.hours = hours;
    CRITICAL_REGION_EXIT();                                                     
}

static void increment_time()
{                                                 
    CRITICAL_REGION_ENTER();                                                    
    ++time.seconds;                                                          
    if (time.seconds == 60) {                                              
        ++time.minutes;                                                    
        time.seconds = 0;                                                  
    }                                                                           
    if (time.minutes == 60) {                                              
        ++time.hours;                                                      
        time.minutes = 0;                                                  
    }                                                                           
    if (time.hours == 24) {                                                
        time.seconds = 0;                                                  
        time.minutes = 0;                                                  
        time.hours = 0;                                                    
        time.milli = 0;                                                    
    }                                                                           
    CRITICAL_REGION_EXIT();                                                     
}

static app_timer_id_t timer_id_1hz;

void task_1hz(void * arg_ptr)
{
    static int i = 0;
    static uint32_t step_count = 0;
    //static uint8_t battery_level = 100;
    //static uint16_t heart_rate_bpm = 1000;

    nrf_gpio_pin_toggle(PIN_LED_1);

    //gps_get_info(&gps_info, GPS_TYPE_GPRMC);
    //set_time(gps_info.hours, gps_info.minutes, gps_info.seconds);

    increment_time();
    step_count = get_steps();
    buildWatchFace_LCD(&time, get_steps());
    refresh();

    ble_ped_update_step_count(step_count);
    /*
    bas_update(battery_level--);
    hrs_update(heart_rate_bpm++);
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

    // Init gpio pins
    gpio_init();

    // Init application timer and scheduler
    timers_init(true);
    scheduler_init();

    // Init BLE
    ble_init();

    // Init IMU
    mympu_open(200);

    // Init LCD
    spi_init();
    clearDisplay();
    
    // Init GPS
    gps_init();
    gps_config();
    gps_enable();
    //gps_get_info(&gps_info, GPS_TYPE_GPRMC);

    // TODO move this
    app_timer_create(&timer_id_1hz, APP_TIMER_MODE_REPEATED, task_1hz);
    app_timer_start(timer_id_1hz, APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER),
                    app_timer_evt_schedule);

    // Begin BLE advertisement
    advertising_start();

    // Main loop
    while (1) {
        app_sched_execute();
        sd_app_evt_wait();         
        //__WFI(); // FIXME remove when using ble
    }
}
