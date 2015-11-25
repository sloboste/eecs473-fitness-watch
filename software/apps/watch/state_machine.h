/* A state machine to control the content displayed on the screen of the watch
 * and the functions executed in each state.
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdbool.h>

// States of the state machine
enum state_machine_state_enum {
    STATE_WATCH_FACE,
    STATE_STEPS,
    STATE_STEPS_GOAL,
    STATE_RUN_TIMER_OFF,
    STATE_RUN_TIMER_ON,
    STATE_GPS_OFF,
    STATE_GPS_ON,
    STATE_STOPWATCH_TIMER_OFF,
    STATE_STOPWATCH_TIMER_ON,
};

/**
 * Initialize the state machine. This will put it into the state where it
 * displays the watch face.
 */
extern void state_machine_init();

/**
 * Refresh the contents of the screen so that all updated data is displayed.
 * This function works regardless the current state of the state machine.
 */
extern void state_machine_refresh_screen();

/**
 * The callback function to register with ble_config that responds to BLE
 * advertising or connection state changes.
 *
 * ble_state -- the new BLE state (one of ble_config's BLE_STATE_X).
 * 
 * Note: this will be called in the thread context.
 */
extern void state_machine_on_ble_adv_con(uint8_t ble_state);

/**
 * The function to be scheduled in response to the "Cycle" button press.
 *
 * Note: DO NOT run this in the interrupt context.
 */
extern void state_machine_on_button_0();

/**
 * The function to be scheduled in response to the "Select" button press.
 *
 * Note: DO NOT run this in the interrupt context.
 */
extern void state_machine_on_button_1();

/**
 * TODO what is this for?
 */
extern void state_machine_on_button_2();

#endif
