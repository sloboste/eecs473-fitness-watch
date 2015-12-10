/* Application for the Eir fitness watch.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "app_error.h"
#include "nrf.h"
#include "softdevice_handler.h"
#include "nrf_delay.h"

#include "nrf_gpio.h"
#include "app_gpiote.h"

#include "timer_config.h"
#include "scheduler_config.h"

#include "ble_config.h"
#include "watch_service.h"
#include "packets.h"

#include "mpu.h"
#include "fuel_gauge.h"

#include "spi_driver.h"
#include "lcd_builder.h"
#include "state_machine.h"

#include "watch_data.h"
#include "date_time.h"

#include "flash.h"

#include "uart_adapter.h"
#include "gps.h"

#include "boards.h"


#define PIN_BUTTON_CYCLE        PIN_BUTTON_1
#define PIN_BUTTON_SELECT       PIN_BUTTON_2
#define PIN_BUTTON_POWER        PIN_BUTTON_3


// Buffer used when sending notifications over BLE. 
static uint8_t packet_buf[PACKET_BUF_LEN];

// Holds data from GPS
static gps_info_t gps_info;

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

    } else if ((event_pins_high_to_low >> PIN_BUTTON_POWER) & 0x1) {
        // Power off
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
        PIN_BUTTON_POWER, BUTTON_PULL, NRF_GPIO_PIN_SENSE_LOW);
    uint32_t low_to_high_bitmask = 0x00000000;
    uint32_t high_to_low_bitmask = (1 << PIN_BUTTON_CYCLE) +
                                   (1 << PIN_BUTTON_SELECT) +
                                   (1 << PIN_BUTTON_POWER);
    APP_GPIOTE_INIT(1);
    static app_gpiote_user_id_t gpiote_user_id;
    error_code = app_gpiote_user_register(
        &gpiote_user_id, low_to_high_bitmask, high_to_low_bitmask,
        button_handler);
    APP_ERROR_CHECK(error_code);
    error_code = app_gpiote_user_enable(gpiote_user_id);
    APP_ERROR_CHECK(error_code);
}

/**
 * The function to run when the 10 Hz timer from timer_config goes off. We are
 * using it to count tenths of seconds for the stopwatch.
 * 
 * arg_ptr -- pointer to event data (unused)
 */
void task_10hz(void * arg_ptr)
{
    if (++watch_data_stopwatch.timer_tenths > 9) {
        watch_data_stopwatch.timer_tenths = 0;
        if (++watch_data_stopwatch.timer_seconds > 59) { 
            watch_data_stopwatch.timer_seconds = 0;
            if (++watch_data_stopwatch.timer_minutes > 59) { 
                watch_data_stopwatch.timer_minutes = 0;
            }
        }
    }
    state_machine_refresh_screen();
}

/**
 * The function to run when the 1 Hz timer number 1 from timer_config goes off.
 * We are using it to count seconds for the run timer.
 * 
 * arg_ptr -- pointer to event data (unused)
 */
void task_1hz_1(void * arg_ptr)
{
    if (++watch_data_run.timer_seconds > 59) {
        watch_data_run.timer_seconds = 0;
        if (++watch_data_run.timer_minutes > 59) { 
            watch_data_run.timer_minutes = 0;
            if (++watch_data_run.timer_hours > 23) { 
                watch_data_run.timer_hours = 0;
            }
        }
    }
    state_machine_refresh_screen();
}

/**
 * The function to run when the 1 Hz timer number 0 from timer_config goes off.
 * We are using it to TODO???.
 * 
 * arg_ptr -- pointer to event data (unused)
 */
void task_1hz_0(void * arg_ptr)
{
    uint32_t steps;

    date_time_increment_second();

    steps = watch_data_step.steps_offset + pedometer_get_steps();
    watch_data_step.steps = steps; 

    bool got_gprmc = false;
    bool got_gpgga = false;
    if (gps_is_enabled()) {
        while (!(got_gprmc && got_gpgga)) {
            switch (gps_get_info(&gps_info)) { 
                case GPS_TYPE_GPRMC:
                    strcpy(watch_data_gps.longitude, gps_info.longitude); 
                    strcpy(watch_data_gps.latitude, gps_info.latitude); 
                    watch_data_gps.ground_speed =
                        (uint32_t) (KNOT_IN_METERS_PER_SECOND * gps_info.speed); 
                    got_gprmc = true;
                    
                    if (gps_info.hours < 5) {
                        gps_info.hours = 24 + gps_info.hours;
                    } 
                    gps_info.hours -= 5;
                    date_time_update_time_same_day(
                        gps_info.hours, gps_info.minutes, gps_info.seconds);
                    break;
                case GPS_TYPE_GPGGA:
                    strcpy(watch_data_gps.longitude, gps_info.longitude); 
                    strcpy(watch_data_gps.latitude, gps_info.latitude); 
                    watch_data_gps.altitude = gps_info.altitude; 
                    got_gpgga = true;
                    break;
                case GPS_TYPE_NO_FIX:
                    // No data to get, just break out
                    got_gprmc = true;
                    got_gpgga = true;
                    break;
                case GPS_TYPE_INVALID:
                    break;
            }
        }
    }

    state_machine_refresh_screen();
}

/**
 * Handle a GPS log dump request.
 */
static void GPS_log_helper()
{
    uint8_t BUF_LEN = 18;
    uint8_t buf[BUF_LEN];

    bool was_enabled = gps_is_enabled();
    if (!was_enabled) {
        gps_enable();
    }

    timer_stop_1hz_periodic_0();

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
        nrf_delay_ms(100);

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

    if (!was_enabled) {
        gps_disable();
    }

    timer_start_1hz_periodic_0();
}

/**
 * Handle request packets sent by the phone and send a reply.
 * 
 * data -- a buffer containing the data from the BLE request packet.
 * len -- the length of the data in the buffer.
 */
static ble_watch_request_handler_t request_handler(uint8_t * data, uint16_t len)
{
    uint8_t packet_type;
    packet_type = packets_decode_request_packet(data, (uint8_t) len);

    switch (packet_type) {
        case PACKET_TYPE_REQUEST_PED_STEP_COUNT:
            // Send step count
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            uint32_t step_count_rev = __REV(watch_data_step.steps);
            packets_build_reply_packet(
                packet_buf,
                PACKET_TYPE_REPLY_PED_STEP_COUNT,
                (void *) &step_count_rev,
                sizeof(step_count_rev),
                true); 
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;

        case PACKET_TYPE_REQUEST_GPS_DATA: 
            // Send latitude
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf,
                PACKET_TYPE_REPLY_GPS_LATITUDE,
                (void *) watch_data_gps.latitude,
                sizeof(watch_data_gps.latitude), // FIXME use strlen
                true); 
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            // Send longitude
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_GPS_LONGITUDE,
                (void *) watch_data_gps.longitude,
                sizeof(watch_data_gps.longitude), // FIXME use strlen
                true);
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            // Send speed
            uint32_t speed_rev = __REV(watch_data_gps.ground_speed); 
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_GPS_SPEED,
                (void *) &speed_rev,
                sizeof(speed_rev),
                true);
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;

        case PACKET_TYPE_REQUEST_GPS_LOG: 
            // Send GPS logs
            app_sched_event_put(NULL, 0, GPS_log_helper);  
            break;

        case PACKET_TYPE_REQUEST_BATTERY_LEVEL:
            // Send battery level
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_BATTERY_LEVEL,
                (void *) &watch_data_battery_level,
                sizeof(watch_data_battery_level),
                true);
            ble_watch_send_reply_packet(packet_buf, PACKET_BUF_LEN);
            break;

        case PACKET_TYPE_REQUEST_HEART_RATE:
            // Send heart rate
            memset(&packet_buf, 0, PACKET_BUF_LEN);    
            uint32_t heart_rate_rev = __REV16(watch_data_heart_rate);
            packets_build_reply_packet(
                packet_buf, 
                PACKET_TYPE_REPLY_HEART_RATE,
                (void *) &heart_rate_rev,
                sizeof(heart_rate_rev),
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
 * The function to call when a minute passes. We are going to poll the battery
 * level every minute with this function.
 */
static void on_minute_change()
{
    watch_data_battery_level = fuel_get_battery_level();
    state_machine_refresh_screen();
}

/**
 * The function to call when a day passes. We are going to put today's steps
 * into yesterday's steps.
 */
static void on_day_change()
{
    watch_data_step.yesterday_steps = watch_data_step.steps;
    watch_data_step.steps = 0;
    watch_data_step.steps_offset = 0;
    pedometer_reset_steps();
}

/**
 * Watch app main.
 */
int main(void)
{
    // NOTE: don't change the init order. It may break if you do.

    // Init LED pins
    init_leds();

    // Init application timer and scheduler
    timers_init(true);
    scheduler_init();

    // Init persistent storage
    flash_init();

    // Init buttons
    buttons_init();

    // Init SW I2C and fuel gague
    fuel_init(PIN_SW_I2C_SDA, PIN_SW_I2C_SCL, PIN_FUEL_QST);
    fuel_quick_start(false);
    
    // Init IMU
    pedometer_init();

    // Init time keeping mechanism
    date_time_init(on_minute_change, on_day_change);
    flash_load_date_time(&date_time);
    // FIXME date load/store broke...
    date_time.day_num = 10;
    date_time.month_num = 12;
    date_time.year_2digit = 15;
    update_day_month_str();
    //----

    // Init SPI, LCD, and state machine
    spi_init(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_SS_PIN);
    state_machine_init();

    // Init UART, GPS
    uart_adapter_init(PIN_RXD, PIN_TXD, PIN_RTS, PIN_CTS);
    gps_init();
    gps_disable();

    // Init BLE
    ble_init(request_handler, state_machine_on_ble_adv_con);

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
