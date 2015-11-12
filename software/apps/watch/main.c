#include <stdbool.h>
#include <stdint.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "softdevice_handler.h"

#include "nrf_delay.h"

//#include "blue_dev_board.h"
#include "green_dev_board.h"

#include "timer_config.h"
#include "scheduler_config.h"

#include "ble_config.h"
#include "heart_rate_service.h"
#include "battery_service.h"
#include "gps_service.h"
#include "ped_service.h"

#include "mpu.h"
//#include "inv_mpu.h" // FIXME remove
//#include "SEGGER_RTT.h" // FIXME remove

//#include "gps.h"

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
    static uint8_t battery_level = 100;
    bas_update(battery_level--);

    static uint16_t heart_rate_bpm = 1000;
    hrs_update(heart_rate_bpm++);

    static uint32_t step_count = 0;
    //ble_ped_update_step_count(step_count++);
    step_count = get_steps();
    //SEGGER_RTT_printf(0, "Steps: %d\r\n", step_count);
    ble_ped_update_step_count(step_count);
    //ble_ped_update_step_count(mpu_reg_dump());
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
    // FIXME remove
    //SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
    //SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n\r\n"); 

    // Component initialization
    gpio_init();
    timers_init(true);
    scheduler_init();
    ble_init();
    mympu_open(200);

    app_timer_create(&timer_id_500_milli_hz, APP_TIMER_MODE_REPEATED,
                     task_500_milli_hz);
    app_timer_start(timer_id_500_milli_hz,
                    APP_TIMER_TICKS(500, APP_TIMER_PRESCALER),
                    app_timer_evt_schedule);


    // Begin BLE advertisement
    advertising_start();

    // Main loop
    while (1) {
        app_sched_execute();
        sd_app_evt_wait();         
    }
}
