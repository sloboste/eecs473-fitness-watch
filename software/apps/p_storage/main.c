/* Application for the Eir fitness watch.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "app_error.h"
#include "nrf.h"
#include "softdevice_handler.h"

#include "nrf_gpio.h"
#include "nrf_delay.h"

#include "timer_config.h"
#include "scheduler_config.h"

#include "ble_config.h"
#include "watch_service.h"
#include "packets.h"

#include "pstorage.h"
#include "pstorage_platform.h"

#include "green_dev_board.h"

  
static void sys_evt_dispatch(uint32_t sys_evt)
{
    pstorage_sys_event_handler(sys_evt);
}


static volatile bool flag = false;
static pstorage_ntf_cb_t callback(
    pstorage_handle_t * p_handle, uint8_t op_code, uint32_t result,
    uint8_t * p_data, uint32_t data_len)
{
    nrf_gpio_pin_toggle(PIN_LED_1);
    flag = true;
}

static uint32_t fake_step_count = 0;
#define BLOCK_SIZE 16
#define NUM_BLOCKS 1
pstorage_module_param_t module_param = {
    callback, BLOCK_SIZE, NUM_BLOCKS
};
pstorage_handle_t base_block_id;


void dev_board_gpio_init(void)
{
    nrf_gpio_cfg_output(PIN_LED_0);
    nrf_gpio_cfg_output(PIN_LED_1);

    nrf_gpio_pin_clear(PIN_LED_0);
    nrf_gpio_pin_clear(PIN_LED_1);

    uint8_t i;
    for (i = 0; i < 10; ++i) {
        nrf_gpio_pin_set(PIN_LED_1);
        nrf_delay_ms(100);
        nrf_gpio_pin_clear(PIN_LED_1);
        nrf_delay_ms(100);
    }
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
    uint32_t error_code;
    ++fake_step_count;

    error_code = pstorage_clear(&base_block_id, BLOCK_SIZE);
    APP_ERROR_CHECK(error_code);
    while (!flag) {}

    error_code = pstorage_store(&base_block_id, (uint8_t*) &fake_step_count, sizeof(fake_step_count), 0);
    APP_ERROR_CHECK(error_code);
    while (!flag) {}

    nrf_gpio_pin_toggle(PIN_LED_0);
}

static ble_watch_request_handler_t request_handler(uint8_t * data, uint16_t len)
{
    static uint8_t packet_buf[PACKET_BUF_LEN];
    uint8_t packet_type;
    packet_type = packets_decode_request_packet(data, (uint8_t) len);

    uint32_t error_code;

    // TODO finish
    switch (packet_type) {
        case PACKET_TYPE_REQUEST_PED_STEP_COUNT:
            // Send step count
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            uint32_t step_count_rev = __REV(fake_step_count);
            packets_build_reply_packet(
                packet_buf,
                PACKET_TYPE_REPLY_PED_STEP_COUNT,
                (void *) &step_count_rev,
                sizeof(step_count_rev),
                true); 
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;

        case PACKET_TYPE_REQUEST_GPS_DATA: 
            break;

        case PACKET_TYPE_REQUEST_GPS_LOG: 
            break;

        case PACKET_TYPE_REQUEST_BATTERY_LEVEL:
            break;

        case PACKET_TYPE_REQUEST_HEART_RATE:
            break;

        case 0x0F:
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            uint32_t stuff;
            error_code = pstorage_load(
                (uint8_t *) &stuff, &base_block_id, sizeof(stuff), 0);
            APP_ERROR_CHECK(error_code);
            while (!flag) {}
            uint32_t stuff_rev = __REV(stuff);
            packets_build_reply_packet(
                packet_buf,
                0x0F,
                (void *) &stuff_rev,
                sizeof(stuff_rev),
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

static void on_ble_adv_con(uint8_t state)
{

}

int main(void)
{
    uint32_t error_code;

    dev_board_gpio_init();
    timers_init(true);
    scheduler_init();


    softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(error_code);

    pstorage_init();
    APP_ERROR_CHECK(error_code);
    
    error_code = pstorage_register(&module_param, &base_block_id);
    APP_ERROR_CHECK(error_code);


    ble_init(request_handler, on_ble_adv_con);
    ble_advertising_start();

    timer_start_1hz_periodic_0();

    // Main loop
    while (1) {
        // Run an event from the scheduler queue
        app_sched_execute();
        // Wait for an event in low power mode with softdevice enabled
        sd_app_evt_wait();
    }
}
