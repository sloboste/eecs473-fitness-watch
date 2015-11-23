#include <stdbool.h>
#include <stdint.h>
//#include <stdlib.h> // TODO memset include file

//#include "nrf_drv_clock.h"// FIXME remove when using ble

#include "nrf.h"
#include "nrf_gpio.h"
#include "softdevice_handler.h"

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
//#include "lcd_driver.h"
#include "lcd_builder.h"
#include "state_machine.h"

#include "app_util_platform.h" // CRITICAL SECTION

#include "nrf_delay.h"
#include "app_gpiote.h"


static gps_info_t gps_info;
//static uint32_t step_count = 0;
static uint8_t battery_level = 0;
static uint16_t heart_rate_bpm = 0;
static uint8_t packet_buf[PACKET_BUF_LEN];


static app_timer_id_t timer_id_1hz;
static app_timer_id_t timer_id_10hz;

/**
 *
 * Note: this fuction executes in the interrupt context so schedule any function
 *       calls in response to the button.
 */
void button_handler(uint32_t event_pins_low_to_high, uint32_t event_pins_high_to_low)
{
    // TODO finish
    uint32_t pin;
    if ((event_pins_high_to_low >> PIN_BUTTON_1) & 0x1) {
        pin = PIN_LED_2;
        // Cycle
        app_sched_event_put(NULL, 0, state_machine_on_button_0);
    } else if ((event_pins_high_to_low >> PIN_BUTTON_2) & 0x1) {
        pin = PIN_LED_3;
        // Select
        app_sched_event_put(NULL, 0, state_machine_on_button_1);
    } else if ((event_pins_high_to_low >> PIN_BUTTON_3) & 0x1) {
        pin = PIN_LED_4;
        // TODO
    } else {
        return;
    }
    nrf_gpio_pin_clear(pin);
    nrf_delay_ms(100);
    nrf_gpio_pin_set(pin);
}

/**
 *
 */
void buttons_init()
{
    uint32_t err_code;
    nrf_gpio_cfg_sense_input(PIN_BUTTON_1, BUTTON_PULL, NRF_GPIO_PIN_SENSE_LOW);
    nrf_gpio_cfg_sense_input(PIN_BUTTON_2, BUTTON_PULL, NRF_GPIO_PIN_SENSE_LOW);
    nrf_gpio_cfg_sense_input(PIN_BUTTON_3, BUTTON_PULL, NRF_GPIO_PIN_SENSE_LOW);
    uint32_t low_to_high_bitmask = 0x00000000;
    uint32_t high_to_low_bitmask =
        (1 << PIN_BUTTON_1) + (1 << PIN_BUTTON_2) + (1 << PIN_BUTTON_3);
    APP_GPIOTE_INIT(1);
    static app_gpiote_user_id_t gpiote_user_id;
    err_code = app_gpiote_user_register(
        &gpiote_user_id, low_to_high_bitmask, high_to_low_bitmask,
        button_handler);
    APP_ERROR_CHECK(err_code);
    err_code = app_gpiote_user_enable(gpiote_user_id);
    APP_ERROR_CHECK(err_code);
}


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


void task_10hz(void * arg_ptr)
{
    nrf_gpio_pin_toggle(PIN_LED_4); // FIXME remove

    CRITICAL_REGION_ENTER();                                                     
    TIMER_DATA.timer_tenths += 1;
    if (TIMER_DATA.timer_tenths > 9) {
        TIMER_DATA.timer_tenths -= 10;
        if (++TIMER_DATA.timer_seconds > 59) { 
            TIMER_DATA.timer_seconds -= 60;
            if (++TIMER_DATA.timer_minutes > 59) { 
                TIMER_DATA.timer_minutes -= 60;
            }
        }
    }
    CRITICAL_REGION_EXIT();                                                     
    state_machine_refresh_screen();
}

void task_1hz(void * arg_ptr)
{
    nrf_gpio_pin_toggle(PIN_LED_1); // FIXME remove

    //gps_get_info(&gps_info, GPS_TYPE_GPRMC);
    //set_time(gps_info.hours, gps_info.minutes, gps_info.seconds);

    STEPS_DATA.steps = get_steps();
    --battery_level; // FIXME do real stuff
    ++heart_rate_bpm; // FIXME do real stuff

    date_time_increment_second();
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
            uint32_t step_count_rev = __REV(STEPS_DATA.steps); // Flip endianness
            packets_build_reply_packet(
                packet_buf,
                PACKET_TYPE_REPLY_PED_STEP_COUNT,
                (void *) &step_count_rev,
                sizeof(STEPS_DATA.steps),
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
    //uint32_t err_code = nrf_drv_clock_init(NULL);                               
    //APP_ERROR_CHECK(err_code);                                                  
    //nrf_drv_clock_lfclk_request();                                              
    //-----

    // Init gpio pins
    gpio_init();

    // Init application timer and scheduler
    timers_init(true);
    scheduler_init();

    // Init BLE
    ble_init(request_handler, state_machine_on_ble_adv_con);

    // Init buttons
    buttons_init();
    //nrf_gpio_pin_clear(PIN_LED_2); // FIXME remove

    // Init IMU
    mympu_open(200);

    // Init LCD and state machine
    spi_init();
    state_machine_init();
    
    // Init GPS
    //gps_init();
    //gps_config();
    //gps_enable();
    //gps_get_info(&gps_info, GPS_TYPE_GPRMC);

    // Start the timer for seconds time keeping
    // This must be the last thing before the main loop
    timer_start_1hz_periodic();

    // Main loop
    while (1) {
        app_sched_execute();
        sd_app_evt_wait();         
        //__WFI(); // FIXME remove when using ble
    }
}
