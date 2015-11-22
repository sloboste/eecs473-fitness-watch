/*
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdbool.h>

// States of the state machine
enum state_machine_state_enum {
    STATE_WATCH_FACE,
    STATE_STEPS,
    STATE_RUN_TIMER_OFF,
    STATE_RUN_TIMER_ON,
    STATE_GPS_OFF,
    STATE_GPS_ON,
    STATE_TIMER_OFF,
    STATE_TIMER_ON,
};

/**
 *
 */
extern void state_machine_init();

/**
 *
 */
extern void state_machine_refresh_screen();

/**
 *
 */
extern void state_machine_on_ble_advertisment_start_stop(uint8_t ble_state);

/**
 *
 */
extern void state_machine_on_button_0();

/**
 *
 */
extern void state_machine_on_button_1();

/**
 *
 */
extern void state_machine_on_button_2();

#endif
