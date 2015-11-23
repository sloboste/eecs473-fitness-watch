#include <stdint.h>
#include <stdbool.h>

#include "state_machine.h"
#include "lcd_builder.h"
#include "lcd_driver.h"
#include "ble_config.h"

#include "scheduler_config.h"
#include "timer_config.h"

#include "nrf_gpio.h" // FIXME remove
#include "blue_dev_board.h" // FIXME remove


// The current state of the state machine
static enum state_machine_state_enum current_state;


void state_machine_init()
{
    clearDisplay();
    current_state = STATE_WATCH_FACE;
initStructs(); // TODO I think using this function is funky...
    date_time_init(state_machine_refresh_screen);
    state_machine_refresh_screen();
}


// ?
void state_machine_refresh_screen()
{
    switch (current_state) {
        case STATE_WATCH_FACE:
            buildWatchFace_LCD();
            break;

        case STATE_STEPS:
        case STATE_STEPS_GOAL:
            buildSteps_LCD();
            break;

        case STATE_RUN_TIMER_OFF:
        case STATE_RUN_TIMER_ON:
            buildRun_LCD();
            break;

        case STATE_GPS_OFF:
        case STATE_GPS_ON:
            buildGPS_LCD();
            break;

        case STATE_TIMER_OFF:
        case STATE_TIMER_ON:
            buildTimer_LCD();
            break;

        default: // ERROR
            break;
    }
    refresh();
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
            clearDisplay();
            state_machine_refresh_screen();
            break;

        case STATE_STEPS:
            current_state = STATE_RUN_TIMER_OFF;
            clearDisplay();
            state_machine_refresh_screen();
            break;

        case STATE_STEPS_GOAL:
            // FIXME why is this visibly slow?
            // Cycle numbers in step goal
            ++STEPS_DATA.goal[STEPS_DATA.goal_digit];
            if (STEPS_DATA.goal[STEPS_DATA.goal_digit] > 9) {
                STEPS_DATA.goal[STEPS_DATA.goal_digit] = 0;
            }
            break;

        case STATE_RUN_TIMER_OFF:
            current_state = STATE_GPS_OFF;
            clearDisplay();
            state_machine_refresh_screen();
            break;

        case STATE_RUN_TIMER_ON:
            // No effect 
            break;

        case STATE_GPS_OFF:
            current_state = STATE_TIMER_OFF;
            clearDisplay();
            state_machine_refresh_screen();
            break;

        case STATE_GPS_ON:
            // No effect 
            break;

        case STATE_TIMER_OFF:
            current_state = STATE_WATCH_FACE;
            clearDisplay();
            state_machine_refresh_screen();
            break;

        case STATE_TIMER_ON:
            // Lap timer 
            timerLap();
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
                advertising_stop();
            } else if (lcd_builder_bluetooth_state == BLE_STATE_IDLE) {
                advertising_start();
            }
            state_machine_refresh_screen();
            break;

        case STATE_STEPS:
            // Go to goal setting
            current_state = STATE_STEPS_GOAL;
            STEPS_DATA.goal_digit = 0;
            state_machine_refresh_screen();
            break;

        case STATE_STEPS_GOAL:
            // Go to next goal digit
            ++STEPS_DATA.goal_digit;
            if (STEPS_DATA.goal_digit == 5) {
                current_state = STATE_STEPS;
            }
            state_machine_refresh_screen();
            break;

        case STATE_RUN_TIMER_OFF:
            break;
        case STATE_RUN_TIMER_ON:
            break;
        case STATE_GPS_OFF:
            break;
        case STATE_GPS_ON:
            break;
        case STATE_TIMER_OFF:
            timerReset();
            timer_start_10hz_periodic();
            current_state = STATE_TIMER_ON;
            break;
        case STATE_TIMER_ON:
            timer_stop_10hz_periodic();
            current_state = STATE_TIMER_OFF;
            break;
        default: // ERROR
            break;
    }

}


// ?
void state_machine_on_button_2()
{
    switch (current_state) {
        case STATE_WATCH_FACE:
            break;
        case STATE_STEPS:
            break;
        case STATE_STEPS_GOAL:
            break;
        case STATE_RUN_TIMER_OFF:
            break;
        case STATE_RUN_TIMER_ON:
            break;
        case STATE_GPS_OFF:
            break;
        case STATE_GPS_ON:
            break;
        case STATE_TIMER_OFF:
            break;
        case STATE_TIMER_ON:
            break;
        default: // ERROR
            break;
    }

}
