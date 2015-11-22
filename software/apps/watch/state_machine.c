#include <stdint.h>
#include <stdbool.h>

#include "state_machine.h"
#include "lcd_builder.h"
#include "lcd_driver.h"
#include "ble_config.h"

#include "scheduler_config.h"


// The current state of the state machine
static enum state_machine_state_enum current_state;


void state_machine_init()
{
    current_state = STATE_WATCH_FACE;
    initStructs(); // TODO I think using this function is funky...
    clearDisplay();
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
            // No effect 
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


// ?
void state_machine_on_button_2()
{
    switch (current_state) {
        case STATE_WATCH_FACE:
            break;
        case STATE_STEPS:
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
