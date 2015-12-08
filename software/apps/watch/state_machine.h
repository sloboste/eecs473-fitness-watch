/* This file is part of eecs473-fitness-watch.
 *   
 * The code / board schematics created by the authors of eecs473-fitness-watch
 * are free software/hardware: you can redistribute them and/or modify them
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * The code / board schematics are distributed in the hope that they will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the code / board schematics.  If not, see <http://www.gnu.org/licenses/>.
 *
 * All code / schematics not created by the authors of this repository fall
 * under their original licenses.
 *
 *
 * A state machine to control the content displayed on the screen of the watch
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
 * The function to be scheduled in response to the "Sleep" button press. This
 * will power down the watch immediately and never return. When powered down,
 * any button press will wake up the system and run main() from the top.
 * 
 * Note: DO NOT run this in the interrupt context.
 */
extern void state_machine_on_button_2();

#endif
