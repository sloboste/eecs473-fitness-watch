/* 
 */

// Timer
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h" 

// Scheduler
#include "app_scheduler.h"
#include "app_timer_appsh.h"
#include "nordic_common.h"
#include "app_util_platform.h"

#include "nrf_delay.h"

#include "green_dev_board.h"

// Timer
#define APP_TIMER_PRESCALER 16      // value of the RTC1 prescaler register
#define APP_TIMER_MAX_TIMERS 2      // max number of timers in this app
#define APP_TIMER_OP_QUEUE_SIZE 3   // Size of timer operations queue

// Scheduler
#define SCHED_MAX_EVENT_DATA_SIZE MAX(APP_TIMER_SCHED_EVT_SIZE, sizeof(task_led_arg_t)) // max size of the event data
#define SCHED_QUEUE_SIZE 10

typedef struct task_led_arg_struct {
    uint32_t pin;
    uint32_t cpu_time_ms;
} task_led_arg_t;

static app_timer_id_t led_0_timer_id;
static app_timer_id_t led_1_timer_id;
static uint32_t timer_0_ms = 1000;
static uint32_t timer_1_ms = 200;
static task_led_arg_t task_led_0_arg = {PIN_LED_0, 200};
static task_led_arg_t task_led_1_arg = {PIN_LED_1, 100};

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
    app_sched_event_put(
        (void *) &task_led_0_arg, sizeof(task_led_0_arg), task_led);
}

// This executes in interrupt context
static void timer_1_handler(void * p_context)
{
    app_sched_event_put(
        (void *) &task_led_1_arg, sizeof(task_led_1_arg), task_led);
}

// This executes in thread context
static void task_led(void * arg)
{
    task_led_arg_t * arg_ptr  = (task_led_arg_t *) arg;
    nrf_gpio_pin_set(arg_ptr->pin);
    nrf_delay_ms(arg_ptr->cpu_time_ms);
    nrf_gpio_pin_clear(arg_ptr->pin);
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
