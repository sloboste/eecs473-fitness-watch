/*  Configuration for the BLE standard Heart Rate Service. Uses the Nordic
 *  implementation of the service.
 * 
 *  NOTE: The application must call the following function in the function that
 *        handles BLE stack events.
 *              
 *           ble_hrs_on_ble_evt(ble_evt_t *)
 */

#ifndef __BLE_HRS_H
#define __BLE_HRS_H

#include <stdint.h>

#include "ble_hrs.h"

// 16 bit UUID for this service
#define hrs_UUID BLE_UUID_HEART_RATE_SERVICE

// Initialization for heart rate service
extern uint32_t hrs_init(void);

// Handle for the heart rate service
ble_hrs_t hrs_handle;


// FIXME: Get rid of this later when we have real data...
extern void hrs_update(void);

#endif
