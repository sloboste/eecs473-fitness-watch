/* Test application for bluetooth functionality.
 * 
 * This application will continuously advertise (if not connected). When
 * connected, it will offer the same BLE service as the watch app, but with
 * fake data (see lib/ble/packets.h and lib/ble/watch_service.h for a
 * description of how it works). One of the LED's on the board should blink at
 * 1 Hz and the other will flash for 100ms whenever a request is handled.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "app_error.h"
#include "nrf.h"
#include "softdevice_handler.h"

#include "nrf_gpio.h"

#include "timer_config.h"
#include "scheduler_config.h"

#include "ble_config.h"
#include "watch_service.h"
#include "packets.h"

#include "pcb.h"
//#include "blue_dev_board.h"

#include "nrf_delay.h"

#include "mpu.h"

static uint8_t battery_level = 0;
static uint16_t heart_rate_bpm = 0;
static uint32_t step_count = 0;

static void on_adv_con_change(uint8_t ble_state)
{

}

void pcb_gpio_init(void)
{
    nrf_gpio_cfg_output(PIN_LED_1);
    nrf_gpio_cfg_output(PIN_LED_2);

    nrf_gpio_pin_set(PIN_LED_1);
    nrf_gpio_pin_set(PIN_LED_2);
}

void app_error_handler(uint32_t error_code, uint32_t line_num,
                       const uint8_t * p_file_name)
{
    // On assert, the system can only recover on reset.
    NVIC_SystemReset();
}

void task_1hz_1(void * arg_ptr)
{
    // Do nothing
}

void task_10hz(void * arg_ptr)
{
    // Do nothing
}

void task_1hz_0(void * arg_ptr)
{
    --battery_level;
    ++heart_rate_bpm;
    step_count = get_steps();
    if (step_count > 10) {
        nrf_gpio_pin_toggle(PIN_LED_1);
    } else {
        nrf_gpio_pin_toggle(PIN_LED_2);
    }
}

static ble_watch_request_handler_t request_handler(uint8_t * data, uint16_t len)
{
    static uint8_t packet_buf[PACKET_BUF_LEN];
    uint8_t packet_type;
    packet_type = packets_decode_request_packet(data, (uint8_t) len);

    char * fake_latitude = "42 17.5400 N";
    char * fake_longitude = "083 42.8600 W"; 
    uint32_t fake_speed = 12345; 

    // TODO finish
    switch (packet_type) {
        case PACKET_TYPE_REQUEST_PED_STEP_COUNT:
            // Send step count
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            uint32_t step_count_rev = __REV(step_count);
            packets_build_reply_packet(
                packet_buf,
                PACKET_TYPE_REPLY_PED_STEP_COUNT,
                (void *) &step_count_rev,
                sizeof(step_count),
                true); 
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;

        case PACKET_TYPE_REQUEST_GPS_DATA: 
            // Send latitude
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf,
                PACKET_TYPE_REPLY_GPS_LATITUDE,
                (void *) fake_latitude,
                sizeof(fake_latitude),
                true); 
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            // Send longitude
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_GPS_LONGITUDE,
                (void *) fake_longitude,
                sizeof(fake_longitude),
                true);
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            // Send speed
            uint32_t speed_rev = __REV(fake_speed); 
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_GPS_SPEED,
                (void *) &speed_rev,
                sizeof(fake_speed),
                true);
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;

        case PACKET_TYPE_REQUEST_GPS_LOG: 
            // Send GPS logs
            // TODO
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            break;

        case PACKET_TYPE_REQUEST_BATTERY_LEVEL:
            // Send battery level
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
            // Send heart rate
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            uint32_t heart_rate_bpm_rev = __REV16(heart_rate_bpm);
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_HEART_RATE,
                (void *) &heart_rate_bpm_rev,
                sizeof(heart_rate_bpm_rev),
                true);
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;

        default:
            // Unrecognized request. Send all 0's
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;

    }

    //nrf_gpio_pin_toggle(PIN_LED_2);

    //return 0; // FIXME what is the return value supposed to be? It's not used.
}

int main(void)
{
    pcb_gpio_init();
    timers_init(true);
    scheduler_init();

    mympu_open(200);

    ble_init(request_handler, on_adv_con_change);
    timer_start_1hz_periodic_0();
    ble_advertising_start();

    while (1) {
        app_sched_execute();
    }
}
