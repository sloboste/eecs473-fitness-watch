/*  Configuration for the BLE standard Battery Service. Uses the Nordic
 *  implementation of the service.
 * 
 *  NOTE: The application must call the following function in the function that
 *        handles BLE stack events.
 *              
 *           ble_bas_on_ble_evt(ble_evt_t *)
 */

#ifndef __BLE_BAS_H
#define __BLE_BAS_H

#include <stdint.h>

#include "ble_bas.h"

// 16 bit UUID for this service
#define bas_UUID BLE_UUID_BATTERY_SERVICE 

// Handle for the service
ble_bas_t bas_handle;

// Initialization for heart rate service
extern uint32_t bas_init(void);

// FIXME: Get rid of this later when we have real data...
extern void bas_update(void);

#endif
