/* Application for the Eir fitness watch.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "app_error.h"
#include "nrf.h"
#include "softdevice_handler.h"

#include "nrf_gpio.h"
#include "app_gpiote.h"

#include "timer_config.h"
#include "scheduler_config.h"

#include "ble_config.h"
#include "watch_service.h"
#include "packets.h"

#include "uart_adapter.h"
#include "gps.h"

#include "boards.h"

#include "nrf_delay.h"

// Buffer used when sending notifications over BLE. 
static uint8_t packet_buf[PACKET_BUF_LEN];


/**
 * Initialize the GPIO LED pins.
 */
static void init_leds(void)
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
    nrf_gpio_pin_toggle(PIN_LED_1); // FIXME remove
}

/**
 * Called in the LOG case
 */
static void GPS_log_helper()
{
    uint8_t BUF_LEN = 18;
    uint8_t buf[BUF_LEN];

    timer_stop_1hz_periodic_0();

    nrf_gpio_pin_set(PIN_LED_1); // FIXME remove

    gps_flash_dump();
    
    // Send log dump data
    uint8_t bytes_got;
    memset(buf, 0, BUF_LEN);
    memset(packet_buf, 0, PACKET_BUF_LEN);
    bytes_got = gps_get_log_dump_bytes(buf, BUF_LEN);
    while (bytes_got) {
        packets_build_reply_packet(
            packet_buf,
            PACKET_TYPE_REPLY_GPS_LOG,
            buf,
            bytes_got,
            false);
        ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
        nrf_delay_ms(10);

        memset(buf, 0, BUF_LEN);
        memset(packet_buf, 0, PACKET_BUF_LEN);
        bytes_got = gps_get_log_dump_bytes(buf, BUF_LEN);
    }

    // Send terminal packet with no data
    memset(buf, 0, BUF_LEN);
    memset(packet_buf, 0, PACKET_BUF_LEN);

    packets_build_reply_packet(
        packet_buf,
        PACKET_TYPE_REPLY_GPS_LOG,
        buf,
        0,
        true);
    ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
    nrf_delay_ms(10);

    timer_start_1hz_periodic_0();
}

static ble_watch_request_handler_t request_handler(uint8_t * data, uint16_t len)
{
    uint8_t packet_type;
    packet_type = packets_decode_request_packet(data, (uint8_t) len);

    switch (packet_type) {
        case PACKET_TYPE_REQUEST_GPS_LOG: 
            // Send GPS logs
            app_sched_event_put(NULL, 0, GPS_log_helper);  
            break;

        default:
            // Unrecognized request. Send all 0's
            memset(packet_buf, 0, PACKET_BUF_LEN);    
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;

    }
    //return 0; // FIXME what is the return value supposed to be? It's not used.
}

static void on_adv_con(uint8_t state)
{
    if (state == BLE_STATE_IDLE) ble_advertising_start();
}

int main(void)
{
    // NOTE: don't change the init order. It may break if you do.

    // Init LED pins
    init_leds();

    // Init application timer and scheduler
    timers_init(true);
    scheduler_init();

    // TODO
    // Init UART, GPS
    uart_adapter_init(PIN_RXD, PIN_TXD, PIN_RTS, PIN_CTS);
    //gps_init();
    //gps_config();
    gps_enable();
    //gps_get_info(&gps_info, GPS_TYPE_GPRMC); // TODO test

    // Init BLE
    ble_init(request_handler, on_adv_con);

    ble_advertising_start();

    // TODO decide if this will be started immediately
    // Start the timer for seconds time keeping and sensor polling
    // This must be the last thing before the main loop
    timer_start_1hz_periodic_0();

    // Main loop
    while (1) {
        // Run an event from the scheduler queue
        app_sched_execute();
        // Wait for an event in low power mode with softdevice enabled
        sd_app_evt_wait();
    }
}
