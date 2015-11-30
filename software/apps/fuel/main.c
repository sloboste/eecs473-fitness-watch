/* Application for the Eir fitness watch.
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

#include "fuel_gauge.h" 

#include "blue_dev_board.h"
//#include "pcb.h"


// TODO
static uint16_t heart_rate_bpm = 0;
static uint8_t lcd_builder_battery_level = 120;
//--

void gpio_init(void)
{
    nrf_gpio_cfg_output(PIN_LED_1);
    nrf_gpio_cfg_output(PIN_LED_2);
    nrf_gpio_cfg_output(PIN_LED_3);

    nrf_gpio_pin_set(PIN_LED_1);
    nrf_gpio_pin_set(PIN_LED_2);
    nrf_gpio_pin_set(PIN_LED_3);
}

void app_error_handler(uint32_t error_code, uint32_t line_num,
                       const uint8_t * p_file_name)
{
    // On assert, the system can only recover on reset.
    NVIC_SystemReset();
}

void task_10hz(void * arg_ptr)
{

}

void task_1hz_1(void * arg_ptr)
{
}

void task_1hz_0(void * arg_ptr)
{
    ++heart_rate_bpm;
    lcd_builder_battery_level = fuel_get_battery_level();
    nrf_gpio_pin_toggle(PIN_LED_1);
}

static ble_watch_request_handler_t request_handler(uint8_t * data, uint16_t len)
{
    static uint8_t packet_buf[PACKET_BUF_LEN];
    uint8_t packet_type;
    packet_type = packets_decode_request_packet(data, (uint8_t) len);

    // TODO finish
    switch (packet_type) {
        case PACKET_TYPE_REQUEST_PED_STEP_COUNT:
            break;

        case PACKET_TYPE_REQUEST_GPS_DATA: 
            break;

        case PACKET_TYPE_REQUEST_GPS_LOG: 
            break;

        case PACKET_TYPE_REQUEST_BATTERY_LEVEL:
            // Send battery level
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_BATTERY_LEVEL,
                (void *) &lcd_builder_battery_level,
                sizeof(lcd_builder_battery_level),
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
    //return 0; // FIXME what is the return value supposed to be? It's not used.
}

void dummy()
{

}

int main(void)
{
    gpio_init();
    timers_init(true);
    scheduler_init();
    fuel_init(PIN_SW_I2C_SDA, PIN_SW_I2C_SCL, PIN_FUEL_QST);
    fuel_quick_start(false);
    ble_init(request_handler, dummy);
    timer_start_1hz_periodic_0();
    ble_advertising_start();

    // Main loop
    while (1) {
        // Run an event from the scheduler queue
        app_sched_execute();
        // Wait for an event in low power mode with softdevice enabled
        sd_app_evt_wait();
    }
}
