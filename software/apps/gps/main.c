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
    nrf_gpio_pin_clear(PIN_LED_3); // FIXME remove
    nrf_delay_ms(5000);
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

// TODO clean up
/**
 * Called in the LOG case
 */
static void GPS_log_helper()
{
    nrf_gpio_pin_set(PIN_LED_1);
    nrf_gpio_pin_toggle(PIN_LED_2);

    timer_stop_1hz_periodic_0();
    // Used in Log Dump
    uint16_t loopNum;
    uint16_t i;
    uint8_t buf[18];
    memset(packet_buf, 0, PACKET_BUF_LEN); 
    memset(buf, 0, 18); 
    loopNum = gps_flash_dump_partial(buf); 
    loopNum = 500; // FIXME remove
    i = 0;

    // copy over contents to buffer
    uart_adapter_read(buf, 17);
    // build and send packet
    packets_build_reply_packet(
        packet_buf,
        PACKET_TYPE_REPLY_GPS_LOG,
        buf,
        17,
        false);
    ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN); //17);
    nrf_delay_ms(10);
    
    do
    {
       nrf_gpio_pin_toggle(PIN_LED_3);

        memset(packet_buf, 0, PACKET_BUF_LEN); 
        memset(buf, 0, 18); 
        // Check to see if its the end of the sentence

        // copy over contents to buffer
        uart_adapter_read(buf, 16);
        // build and send packet
        packets_build_reply_packet(
            packet_buf,
            PACKET_TYPE_REPLY_GPS_LOG,
            buf,
            16,
            (loopNum-1 == i));
        ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN); //16);
        nrf_delay_ms(10);
        // if end of sentence get next, else inc position
        i++;
        // gps_send_msg(buf);
    }
    while (i < loopNum);

    nrf_gpio_pin_toggle(PIN_LED_1);

    memset(packet_buf, 0, PACKET_BUF_LEN); 
    memset(buf, 0, 18); 
    // Check to see if its the end of the sentence

    // copy over contents to buffer
    uart_adapter_read(buf, 17);
    packets_build_reply_packet(
        packet_buf,
        PACKET_TYPE_REPLY_GPS_LOG,
        buf,
        17,
        (loopNum-1 == i));
    ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN); //17);
    nrf_delay_ms(10);

    timer_start_1hz_periodic_0();

    nrf_gpio_pin_toggle(PIN_LED_2);
}

static ble_watch_request_handler_t request_handler(uint8_t * data, uint16_t len)
{
    uint8_t packet_type;
    packet_type = packets_decode_request_packet(data, (uint8_t) len);

    uint8_t test_18_bytes[18];
    memset(test_18_bytes, 9, sizeof(test_18_bytes));    

    switch (packet_type) {
        case PACKET_TYPE_REQUEST_PED_STEP_COUNT: 
            memset(packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf,
                PACKET_TYPE_REPLY_PED_STEP_COUNT,
                test_18_bytes,
                18,
                true);
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;
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
