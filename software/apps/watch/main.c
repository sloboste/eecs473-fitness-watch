/* 
 */

#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h" 

#include "app_scheduler.h"
#include "app_timer_appsh.h"
#include "nordic_common.h"
#include "app_util_platform.h"

#include "green_dev_board.h"

#define APP_TIMER_PRESCALER 16      // value of the RTC1 prescaler register
#define APP_TIMER_MAX_TIMERS 2      // max number of timers in this app
#define APP_TIMER_OP_QUEUE_SIZE 3   // Size of timer operations queue

#define SCHED_MAX_EVENT_DATA_SIZE MAX(APP_TIMER_SCHED_EVT_SIZE, sizeof(timer_0_pin)) // max size of the event data
#define SCHED_QUEUE_SIZE 10


static app_timer_id_t led_0_timer_id;
static app_timer_id_t led_1_timer_id;
static uint32_t timer_0_ms = 200;
static uint32_t timer_1_ms = 600;
static uint32_t timer_0_pin = PIN_LED_0;
static uint32_t timer_1_pin = PIN_LED_1;

static void lfclk_request(void);
static void timer_0_handler(void * p_context);
static void timer_1_handler(void * p_context);
static void create_timers(void);
static void timer_start(uint8_t timer);
static void timer_stop(void);
static void task_led(void * arg);


// Function starting the internal LFCLK oscillator.
// This is needed by RTC1 which is used by the application timer
// (When softdevice is enabled this is not needed because lfck will be running)
// NOTE: we won't need this function since we are using BLE
static void lfclk_request(void)
{
    uint32_t err_code = nrf_drv_clock_init(NULL);
    APP_ERROR_CHECK(err_code);
    nrf_drv_clock_lfclk_request();
}

// This executes in interrupt context
static void timer_0_handler(void * p_context)
{
    //app_sched_event_put(&pin, sizeof(pin), button_scheduler_event_handler);
    app_sched_event_put(&timer_0_pin, sizeof(timer_0_pin), task_led);
}

// This executes in interrupt context
static void timer_1_handler(void * p_context)
{
    app_sched_event_put(&timer_1_pin, sizeof(timer_1_pin), task_led);
}

// This executes in thread context
static void task_led(void * arg)
{
    uint32_t * pin_num_ptr = (uint32_t *) arg;
    nrf_gpio_pin_toggle(*pin_num_ptr);
}


static void create_timers(void)
{
    uint32_t err_code;

    err_code = app_timer_create(
        &led_0_timer_id, APP_TIMER_MODE_REPEATED, timer_0_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(
        &led_1_timer_id, APP_TIMER_MODE_REPEATED, timer_1_handler);
    APP_ERROR_CHECK(err_code);
}


static void timer_start(uint8_t timer)
{
    uint32_t err_code;
    app_timer_id_t timer_id;
    uint32_t timer_ms;

    if (timer == 0) {
        timer_id = led_0_timer_id;
        timer_ms = timer_0_ms;
    } else if (timer == 1) {
        timer_id = led_1_timer_id;
        timer_ms = timer_1_ms;
    } 

    err_code = app_timer_start(
        timer_id, APP_TIMER_TICKS(timer_ms, APP_TIMER_PRESCALER), NULL);
    APP_ERROR_CHECK(err_code);
}


static void timer_0_stop(uint8_t timer)
{
    uint32_t err_code;
    app_timer_id_t timer_id;

    if (timer == 0) {
        timer_id = led_0_timer_id;
    } else if (timer == 1) {
        timer_id = led_1_timer_id;
    } 

    err_code = app_timer_stop(timer_id);
    APP_ERROR_CHECK(err_code);
}


int main(void)
{
    nrf_gpio_cfg_output(PIN_LED_0);
    nrf_gpio_cfg_output(PIN_LED_1);
    nrf_gpio_cfg_input(PIN_BUTTON_0, GPIO_PIN_CNF_PULL_Pullup);

    nrf_gpio_pin_clear(PIN_LED_0);
    nrf_gpio_pin_clear(PIN_LED_1);

    lfclk_request();

    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS,
                   APP_TIMER_OP_QUEUE_SIZE, app_timer_evt_schedule);

    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE); 

    create_timers();

    timer_start(0);
    timer_start(1);
    
    while (1) {
        app_sched_execute();
        __WFI();
    }
}
