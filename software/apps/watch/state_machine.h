/*
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H


// States of the state machine
enum state_machine_state_enum {
    STATE_WATCH_FACE_BLE_OFF,
    STATE_WATCH_FACE_BLE_ON,
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
