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

#include "mpu.h"
#include "gps.h"

#include "spi_driver.h"
#include "lcd_builder.h"
#include "state_machine.h"
#include "date_time.h"

#include "blue_dev_board.h"


// TODO change pin assignments for custom PCB
#define PIN_BUTTON_CYCLE        PIN_BUTTON_1
#define PIN_BUTTON_SELECT       PIN_BUTTON_2
#define PIN_BUTTON_SLEEP        PIN_BUTTON_3


// TODO
static gps_info_t gps_info;
static uint8_t battery_level = 0;
static uint16_t heart_rate_bpm = 0;
//--

// TODO remove when using custom PCB
void dev_board_gpio_init(void)
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

/**
 * Runtime error handler.
 *
 * Note: the parameters are for debugging purposes and are unused.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num,
                       const uint8_t * p_file_name)
{
    // On assert, the system can only recover on reset.
    NVIC_SystemReset();
}

/**
 * Button press/release interrupt handler.
 * 
 * event_pins_low_to_high -- bitmask for GPIO pins that experienced a low to
 *                           high transition.
 * event_pins_high_to_low -- bitmask for GPIO pins that experienced a high to
 *                           low transition.
 *
 * Note: this fuction executes in the interrupt context so schedule any
 *       function calls in response to the button.
 */
static void button_handler(uint32_t event_pins_low_to_high,
                           uint32_t event_pins_high_to_low)
{
    uint32_t error_code;
    if ((event_pins_high_to_low >> PIN_BUTTON_CYCLE) & 0x1) {
        // Cycle
        error_code = app_sched_event_put(NULL, 0, state_machine_on_button_0);
        APP_ERROR_CHECK(error_code);

    } else if ((event_pins_high_to_low >> PIN_BUTTON_SELECT) & 0x1) {
        // Select
        error_code = app_sched_event_put(NULL, 0, state_machine_on_button_1);
        APP_ERROR_CHECK(error_code);

    } else if ((event_pins_high_to_low >> PIN_BUTTON_SLEEP) & 0x1) {
        // Sleep
        timer_stop_1hz_periodic_0();
        error_code = app_sched_event_put(NULL, 0, state_machine_on_button_2);
        APP_ERROR_CHECK(error_code);
    }
}

/**
 * Initialize the button GPIO pins to generate interrupts on push.
 */
static void buttons_init()
{
    uint32_t error_code;
    nrf_gpio_cfg_sense_input(
        PIN_BUTTON_CYCLE, BUTTON_PULL, NRF_GPIO_PIN_SENSE_LOW);
    nrf_gpio_cfg_sense_input(
        PIN_BUTTON_SELECT, BUTTON_PULL, NRF_GPIO_PIN_SENSE_LOW);
    nrf_gpio_cfg_sense_input(
        PIN_BUTTON_SLEEP, BUTTON_PULL, NRF_GPIO_PIN_SENSE_LOW);
    uint32_t low_to_high_bitmask = 0x00000000;
    uint32_t high_to_low_bitmask = (1 << PIN_BUTTON_CYCLE) +
                                   (1 << PIN_BUTTON_SELECT) +
                                   (1 << PIN_BUTTON_SLEEP);
    APP_GPIOTE_INIT(1);
    static app_gpiote_user_id_t gpiote_user_id;
    error_code = app_gpiote_user_register(
        &gpiote_user_id, low_to_high_bitmask, high_to_low_bitmask,
        button_handler);
    APP_ERROR_CHECK(error_code);
    error_code = app_gpiote_user_enable(gpiote_user_id);
    APP_ERROR_CHECK(error_code);
}

// TODO this should probably be moved to state_machine.c
/**
 * The function to run when the 10 Hz timer from timer_config goes off. We are
 * using it to count tenths of seconds for the stopwatch.
 * 
 * arg_ptr -- pointer to event data (unused)
 */
void task_10hz(void * arg_ptr)
{
    if (++lcd_builder_stopwatch_data.timer_tenths > 9) {
        lcd_builder_stopwatch_data.timer_tenths = 0;
        if (++lcd_builder_stopwatch_data.timer_seconds > 59) { 
            lcd_builder_stopwatch_data.timer_seconds = 0;
            if (++lcd_builder_stopwatch_data.timer_minutes > 59) { 
                lcd_builder_stopwatch_data.timer_minutes = 0;
            }
        }
    }
    state_machine_refresh_screen();
}

// TODO this should probably be moved to state_machine.c
/**
 * The function to run when the 1 Hz timer number 1 from timer_config goes off.
 * We are using it to count seconds for the run timer.
 * 
 * arg_ptr -- pointer to event data (unused)
 */
void task_1hz_1(void * arg_ptr)
{
    if (++lcd_builder_run_data.timer_seconds > 59) {
        lcd_builder_run_data.timer_seconds = 0;
        if (++lcd_builder_run_data.timer_minutes > 59) { 
            lcd_builder_run_data.timer_minutes = 0;
            if (++lcd_builder_run_data.timer_hours > 23) { 
                lcd_builder_run_data.timer_hours = 0;
            }
        }
    }
    state_machine_refresh_screen();
}

// TODO this should probably be moved to state_machine.c
// TODO decide on functionality
/**
 * The function to run when the 1 Hz timer number 0 from timer_config goes off.
 * We are using it to TODO???.
 * 
 * arg_ptr -- pointer to event data (unused)
 */
void task_1hz_0(void * arg_ptr)
{
    // TODO/FIXME do real stuff
    //gps_get_info(&gps_info, GPS_TYPE_GPRMC);
    //set_time(gps_info.hours, gps_info.minutes, gps_info.seconds);
    --battery_level;
    ++heart_rate_bpm;
    //----
    lcd_builder_step_data.steps = get_steps();
    date_time_increment_second();
}

/**
 * Handle request packets sent by the phone and send a reply.
 * 
 * data -- a buffer containing the data from the BLE request packet.
 * len -- the length of the data in the buffer.
 */
static ble_watch_request_handler_t request_handler(uint8_t * data, uint16_t len)
{
    static uint8_t packet_buf[PACKET_BUF_LEN];
    uint8_t packet_type;
    packet_type = packets_decode_request_packet(data, (uint8_t) len);

    // TODO finish
    switch (packet_type) {
        case PACKET_TYPE_REQUEST_PED_STEP_COUNT:
            // Send step count
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            uint32_t step_count_rev = __REV(lcd_builder_step_data.steps);
            packets_build_reply_packet(
                packet_buf,
                PACKET_TYPE_REPLY_PED_STEP_COUNT,
                (void *) &step_count_rev,
                sizeof(lcd_builder_step_data.steps),
                true); 
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;

        case PACKET_TYPE_REQUEST_GPS_DATA: 
            // Send latitude
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf,
                PACKET_TYPE_REPLY_GPS_LATITUDE,
                (void *) gps_info.latitude,
                sizeof(gps_info.latitude),
                true); 
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            // Send longitude
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_GPS_LONGITUDE,
                (void *) gps_info.longitude,
                sizeof(gps_info.longitude),
                true);
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            // Send speed
            uint32_t speed_rev = __REV(gps_info.speed); 
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
    //return 0; // FIXME what is the return value supposed to be? It's not used.
}

/**
 * Watch app main.
 */
int main(void)
{
    // TODO remove when using custom PCB
    dev_board_gpio_init();

    // Init application timer and scheduler
    timers_init(true);
    scheduler_init();

    // Init buttons
    buttons_init();

    // Init IMU
    mympu_open(200);

    // Init LCD and state machine
    spi_init();
    state_machine_init();

    // TODO
    // Init GPS
    //gps_init();
    //gps_config();
    //gps_enable();
    //gps_get_info(&gps_info, GPS_TYPE_GPRMC);

    // Init BLE
    ble_init(request_handler, state_machine_on_ble_adv_con);

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
