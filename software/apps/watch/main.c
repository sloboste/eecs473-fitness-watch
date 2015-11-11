#include <stdbool.h>
#include <stdint.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "softdevice_handler.h"

// Timer
#include "app_timer.h"

// Scheduler
#include "app_scheduler.h"
#include "app_timer_appsh.h"
#include "nordic_common.h"
#include "app_util_platform.h"

#include "nrf_delay.h"

//#include "blue_dev_board.h"
#include "green_dev_board.h"

//#include "timer_config.h"
#include "ble_config.h"
#include "heart_rate_service.h"
#include "battery_service.h"
#include "gps_service.h"
#include "ped_service.h"

//#include "gps.h"

// timer FIXME
#define APP_TIMER_PRESCALER 0      // value of the RTC1 prescaler register     
#define APP_TIMER_MAX_TIMERS 4      // max number of timers in this app         
#define APP_TIMER_OP_QUEUE_SIZE 6   // Size of timer operations queue
// ---

// scheduler FIXME
#define SCHED_MAX_EVENT_DATA_SIZE MAX(APP_TIMER_SCHED_EVT_SIZE, 0) //sizeof(task_led_arg_t)) // max size of the event data
#define SCHED_QUEUE_SIZE 10
// ---

/**
 * Handle when an error ocurrs during runtime.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num,
                       const uint8_t * p_file_name)
{
    int i;
    // Blink the LEDs in a special pattern so that we know there is an error
    #ifdef GREEN_DEV_BOARD_H
    nrf_gpio_pin_clear(PIN_LED_0);
    nrf_gpio_pin_clear(PIN_LED_1);
    
    for (i = 0; i < 200; ++i) {
        nrf_gpio_pin_set(PIN_LED_0);
        nrf_delay_ms(100);
        nrf_gpio_pin_clear(PIN_LED_0);
        nrf_gpio_pin_set(PIN_LED_0);
        nrf_delay_ms(100);
        nrf_gpio_pin_clear(PIN_LED_0);
    }

    nrf_gpio_pin_set(PIN_LED_0);
    nrf_gpio_pin_set(PIN_LED_1);
    #endif

    #ifdef BLUE_DEV_BOARD_H
    nrf_gpio_pin_set(PIN_LED_1);
    nrf_gpio_pin_set(PIN_LED_2);
    nrf_gpio_pin_set(PIN_LED_3);
    nrf_gpio_pin_set(PIN_LED_4);

    for (i = 0; i < 200; ++i) {
        nrf_gpio_pin_clear(PIN_LED_1);
        nrf_delay_ms(100);
        nrf_gpio_pin_set(PIN_LED_1);
        nrf_gpio_pin_clear(PIN_LED_2);
        nrf_delay_ms(100);
        nrf_gpio_pin_set(PIN_LED_2);
        nrf_gpio_pin_clear(PIN_LED_3);
        nrf_delay_ms(100);
        nrf_gpio_pin_set(PIN_LED_3);
        nrf_gpio_pin_clear(PIN_LED_4);
        nrf_delay_ms(100);
        nrf_gpio_pin_set(PIN_LED_4);
    }

    nrf_gpio_pin_clear(PIN_LED_1);
    nrf_gpio_pin_clear(PIN_LED_2);
    nrf_gpio_pin_clear(PIN_LED_3);
    nrf_gpio_pin_clear(PIN_LED_4);
    #endif

    // On assert, the system can only recover on reset.
    NVIC_SystemReset();
}


/**
 * Initialize the gpio pins based on the dev board we are using.
 */
void gpio_init(void)
{
    #ifdef GREEN_DEV_BOARD_H
    nrf_gpio_cfg_output(PIN_LED_0);
    nrf_gpio_cfg_output(PIN_LED_1);

    nrf_gpio_pin_clear(PIN_LED_0);
    nrf_gpio_pin_clear(PIN_LED_1);
    #endif

    #ifdef BLUE_DEV_BOARD_H
    nrf_gpio_cfg_output(PIN_LED_1);
    nrf_gpio_cfg_output(PIN_LED_2);
    nrf_gpio_cfg_output(PIN_LED_3);
    nrf_gpio_cfg_output(PIN_LED_4);

    nrf_gpio_pin_set(PIN_LED_1);
    nrf_gpio_pin_set(PIN_LED_2);
    nrf_gpio_pin_set(PIN_LED_3);
    nrf_gpio_pin_set(PIN_LED_4);
    #endif
}


void ble_update_task(void * arg)
{
    // FIXME
    // Nonesense to test 
    static uint8_t battery_level = 100;
    static uint16_t heart_rate_bpm = 1000;
    bas_update(battery_level--);
    hrs_update(heart_rate_bpm++);
}


static app_timer_id_t timer_id_500_milli_hz;

void task_500_milli_hz(void * arg_ptr)
{
    nrf_gpio_pin_toggle(PIN_LED_1);
    ble_update_task(NULL);
}


/**
 * Watch app.
 */
int main(void)
{
    // Component initialization
    gpio_init();

    //timers_init();
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS,                   
                   APP_TIMER_OP_QUEUE_SIZE, app_timer_evt_schedule);
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
    app_timer_create(&timer_id_500_milli_hz, APP_TIMER_MODE_REPEATED,
                     task_500_milli_hz);
    app_timer_start(timer_id_500_milli_hz,
                    APP_TIMER_TICKS(500, APP_TIMER_PRESCALER),
                    app_timer_evt_schedule);

    ble_init();

    // Begin BLE advertisement
    advertising_start();

    // Main loop
    while (1) {
        app_sched_execute();
        sd_app_evt_wait();         
    }
}
