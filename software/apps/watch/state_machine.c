#include <stdint.h>
#include <stdbool.h>

#include "softdevice_handler.h"
#include "nrf_delay.h"

#include "state_machine.h"
#include "lcd_builder.h"
#include "lcd_driver.h"
#include "ble_config.h"
#include "date_time.h"
#include "scheduler_config.h"
#include "timer_config.h"
#include "flash.h"

// FIXME remove
#include "blue_dev_board.h"
#include "nrf_gpio.h"
//---


// The current state of the state machine
static enum state_machine_state_enum current_state;


void state_machine_init()
{
    lcd_clearDisplay();
    current_state = STATE_WATCH_FACE;

    lcd_builder_init_structs(); // FIXME I think using this function is funky...

    flash_load_step_count(&lcd_builder_step_data.steps_offset);
    lcd_builder_step_data.steps = lcd_builder_step_data.steps_offset;
    flash_load_step_yesterday(&lcd_builder_step_data.yesterday_steps);
    flash_load_step_goal(lcd_builder_step_data.goal);
    
    state_machine_refresh_screen();
}

void state_machine_refresh_screen()
{
    switch (current_state) {
        case STATE_WATCH_FACE:
            lcd_builder_build_watch_face();
            break;

        case STATE_STEPS:
        case STATE_STEPS_GOAL:
            lcd_builder_build_steps();
            break;

        case STATE_RUN_TIMER_OFF:
        case STATE_RUN_TIMER_ON:
            lcd_builder_build_run();
            break;

        case STATE_GPS_OFF:
        case STATE_GPS_ON:
            lcd_builder_build_gps();
            break;

        case STATE_STOPWATCH_TIMER_OFF:
        case STATE_STOPWATCH_TIMER_ON:
            lcd_builder_build_stopwatch();
            break;

        default: // ERROR
            break;
    }
    lcd_refresh();
}

void state_machine_on_ble_adv_con(uint8_t ble_state)
{
    lcd_builder_bluetooth_state = ble_state;
    app_sched_event_put(NULL, 0, state_machine_refresh_screen);
}

// Cycle
void state_machine_on_button_0()
{
    switch (current_state) {
        case STATE_WATCH_FACE:
            current_state = STATE_STEPS;
            lcd_clearDisplay();
            state_machine_refresh_screen();
            break;

        case STATE_STEPS:
            current_state = STATE_RUN_TIMER_OFF;
            lcd_clearDisplay();
            state_machine_refresh_screen();
            break;

        case STATE_STEPS_GOAL:
            // Cycle numbers in step goal
            if (++lcd_builder_step_data.goal[lcd_builder_step_data.goal_digit] > '9') {
                lcd_builder_step_data.goal[lcd_builder_step_data.goal_digit] = '0';
            }
            state_machine_refresh_screen();
            break;

        case STATE_RUN_TIMER_OFF:
            current_state = STATE_GPS_OFF;
            lcd_clearDisplay();
            state_machine_refresh_screen();
            break;

        case STATE_RUN_TIMER_ON:
            // Run timer reset
            current_state = STATE_RUN_TIMER_OFF;
            timer_stop_1hz_periodic_1();
            lcd_builder_run_data.timer_running = false;
            lcd_builder_build_run();
            state_machine_refresh_screen();
            break;

        case STATE_GPS_OFF:
            current_state = STATE_STOPWATCH_TIMER_OFF;
            lcd_clearDisplay();
            state_machine_refresh_screen();
            break;

        case STATE_GPS_ON:
            // No effect 
            break;

        case STATE_STOPWATCH_TIMER_OFF:
            current_state = STATE_WATCH_FACE;
            lcd_clearDisplay();
            state_machine_refresh_screen();
            break;

        case STATE_STOPWATCH_TIMER_ON:
            // Lap timer 
            lcd_builder_stopwatch_timer_lap();
            break;

        default: // ERROR
            // No effect 
            break;
    }
}

// Select
void state_machine_on_button_1()
{
    switch (current_state) {
        case STATE_WATCH_FACE:
            if (lcd_builder_bluetooth_state == BLE_STATE_ADVERTISING) {
                ble_advertising_stop();
            } else if (lcd_builder_bluetooth_state == BLE_STATE_IDLE) {
                ble_advertising_start();
            }
            state_machine_refresh_screen();
            break;

        case STATE_STEPS:
            // Go to goal setting
            current_state = STATE_STEPS_GOAL;
            lcd_builder_step_data.goal_digit = 0;
            state_machine_refresh_screen();
            break;

        case STATE_STEPS_GOAL:
            // Go to next goal digit
            if (++lcd_builder_step_data.goal_digit > 4) {
                current_state = STATE_STEPS;
            }
            state_machine_refresh_screen();
            break;

        case STATE_RUN_TIMER_OFF:
            // Start timer
            current_state = STATE_RUN_TIMER_ON;
            lcd_builder_run_timer_reset();
            timer_start_1hz_periodic_1(); 
            lcd_builder_run_data.timer_running = true;
            state_machine_refresh_screen();
            break;

        case STATE_RUN_TIMER_ON:
            // Pause or restart timer
            if (lcd_builder_run_data.timer_running) {
                timer_stop_1hz_periodic_1();
                lcd_builder_run_data.timer_running = false;
            } else {
                timer_start_1hz_periodic_1();
                lcd_builder_run_data.timer_running = true;
            }
            state_machine_refresh_screen();
            break;

        // TODO
        case STATE_GPS_OFF:
            break;
        case STATE_GPS_ON:
            break;

        case STATE_STOPWATCH_TIMER_OFF:
            lcd_builder_stopwatch_timer_reset();
            timer_start_10hz_periodic();
            current_state = STATE_STOPWATCH_TIMER_ON;
            break;

        case STATE_STOPWATCH_TIMER_ON:
            timer_stop_10hz_periodic();
            current_state = STATE_STOPWATCH_TIMER_OFF;
            break;

        default: // ERROR
            break;
    }

}

// Power off
void state_machine_on_button_2()
{
    // Note: It hardfaults if this delay is not here. Maybe because of a
    // timer interrupt that goes off before the system has a chance to finish
    // shutting down. 
    lcd_clearDisplay();
    lcd_builder_build_sleep_message();
    lcd_refresh();
    flash_store_step_count(&lcd_builder_step_data.steps);
    flash_store_step_yesterday(&lcd_builder_step_data.yesterday_steps);
    flash_store_step_goal(lcd_builder_step_data.goal);
    flash_store_date_time(&date_time);
    nrf_delay_ms(1000);
    lcd_clearDisplay();
    sd_power_system_off();
    // Never returns
}
