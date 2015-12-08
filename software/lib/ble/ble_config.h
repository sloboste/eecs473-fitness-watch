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
 * Configuration of the BLE softdevice stack for the Eir watch application.
 */

#ifndef BLE_CONFIG_H
#define BLE_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

#include "app_util.h"
#include "ble_gap.h"

#include "watch_service.h"


//No structural changes to attributes are allowed.
#define IS_SRVC_CHANGED_CHARACT_PRESENT 0

// Time in seconds for which the device must be advertising. 0 disables timeout.
#define APP_ADV_TIMEOUT_SECONDS         0    

// The advertising interval. This affects discovery and connect performance.
// Apple recommends 20ms so we'll do that.
// https://developer.apple.com/hardwaredrivers/BluetoothDesignGuidelines.pdf
#define APP_ADV_INTERVAL                MSEC_TO_UNITS(20, UNIT_0_625_MS)

// TODO Tune these
// BLE connection parameters. See link for description.
// https://devzone.nordicsemi.com/question/60/what-is-connection-parameters/
#define CONN_INTERVAL_MIN               BLE_GAP_CP_MIN_CONN_INTVL_MIN
#define CONN_INTERVAL_MAX               BLE_GAP_CP_MIN_CONN_INTVL_MAX
#define CONN_SLAVE_LATENCY              0
#define CONN_SUPERVISION_TIMEOUT        BLE_GAP_CP_CONN_SUP_TIMEOUT_MAX 

// The states of the BLE device that are passed to on_ble_adv_con when called.
#define BLE_STATE_IDLE          0
#define BLE_STATE_ADVERTISING   1
#define BLE_STATE_CONNECTED     2


/**
 * Call this to set up the BLE configuration.
 *
 * Note: the timers must have already been set up properly otherwise the
 *       softdevice won't work properly (it needs Nordic's app_timer module for
 *       the connection parameter timing).
 *
 * handler -- the request handler function for the BLE watch service.
 * on_ble_adv_con -- the handler function for BLE advertising and connection
 *                   status changes. Use this to know when the device is
 *                   advertising, connected, or neither.
 */
extern void ble_init(ble_watch_request_handler_t handler,
                     void (*on_ble_adv_con)(uint8_t));

/**
 * Start sending BLE advertising packets. The device will contiue to advertise
 * until it is connected to or ble_advertising_stop is called.
 */
extern void ble_advertising_start();

/**
 * Stop sending BLE advertising packets.
 */
void ble_advertising_stop();

#endif
