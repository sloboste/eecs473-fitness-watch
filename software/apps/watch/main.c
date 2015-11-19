#include <stdbool.h>
#include <stdint.h>
//#include <stdlib.h> // TODO memset include file

#include "nrf_drv_clock.h"// FIXME remove when using ble

#include "nrf.h"
#include "nrf_gpio.h"
#include "softdevice_handler.h"

//#include "nrf_delay.h"

#include "blue_dev_board.h"
//#include "green_dev_board.h"

#include "timer_config.h"
#include "scheduler_config.h"

#include "ble_config.h"
#include "watch_service.h"
#include "packets.h"

#include "mpu.h"
#include "gps.h"

#include "spi_driver.h"
#include "lcd_driver.h"

#include "app_util_platform.h" // CRITICAL SECTION
#include "time_keeper.h"


static rtc_time_t time;
static gps_info_t gps_info;
static uint32_t step_count = 0;
static uint8_t battery_level = 0;
static uint16_t heart_rate_bpm = 0;
static uint8_t packet_buf[PACKET_BUF_LEN];

// FIXME remove

//---

static app_timer_id_t timer_id_1hz;

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


void task_1hz(void * arg_ptr)
{
    nrf_gpio_pin_toggle(PIN_LED_1); // FIXME remove

    //gps_get_info(&gps_info, GPS_TYPE_GPRMC);
    //set_time(gps_info.hours, gps_info.minutes, gps_info.seconds);

    step_count = get_steps();
    --battery_level; // FIXME do real stuff
    ++heart_rate_bpm; // FIXME do real stuff

    increment_time(); // FIXME do real stuff

    buildWatchFace_LCD(&time, step_count);
    refresh();

}

/**
 * Handle request packets sent by the phone.
 */
static ble_watch_request_handler_t request_handler(uint8_t * data, uint16_t len)
{
    nrf_gpio_pin_toggle(PIN_LED_4); // FIXME remove

    uint8_t packet_type;
    packet_type = packets_decode_request_packet(data, (uint8_t) len);

    // TODO finish
    switch (packet_type) {
        case PACKET_TYPE_REQUEST_PED_STEP_COUNT:
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            uint32_t step_count_rev = __REV(step_count); // Flip endianness
            packets_build_reply_packet(
                packet_buf,
                PACKET_TYPE_REPLY_PED_STEP_COUNT,
                (void *) &step_count_rev,
                sizeof(step_count),
                true); 
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;
        case PACKET_TYPE_REQUEST_GPS_DATA: 
            // Latitude
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf,
                PACKET_TYPE_REPLY_GPS_LATITUDE,
                (void *) gps_info.latitude,
                sizeof(gps_info.latitude),
                true); 
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            // Longitude
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_GPS_LONGITUDE,
                (void *) gps_info.longitude,
                sizeof(gps_info.latitude),
                true);
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            // Speed
            uint32_t speed_rev = __REV(gps_info.speed); // Flip endianness
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_GPS_SPEED,
                (void *) &speed_rev,
                sizeof(gps_info.speed),
                true);
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;
        case PACKET_TYPE_REQUEST_GPS_LOG: 
            // TODO
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            break;
        case PACKET_TYPE_REQUEST_BATTERY_LEVEL:
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_BATTERY_LEVEL,
                (void *) &battery_level,
                sizeof(battery_level),
                true);
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;
        case PACKET_TYPE_REQUEST_HEART_RATE:
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            uint32_t heart_rate_bpm_rev = __REV16(heart_rate_bpm); // Flip endianness
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_HEART_RATE,
                (void *) &heart_rate_bpm_rev,
                sizeof(heart_rate_bpm_rev),
                true);
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;
        default: // Unrecognized request
            // TODO
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;
    }
    return 0; // FIXME
}

/**
 * Watch app.
 */
int main(void)
{
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
    ble_init(request_handler);

    // Init IMU
    mympu_open(200);

    // Init LCD
    spi_init();
    clearDisplay();
    
    // Init GPS
    //gps_init();
    //gps_config();
    //gps_enable();
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
