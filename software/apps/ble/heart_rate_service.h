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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nordic_common.h"
#include "ble_hrs.h"
#include "ble_srv_common.h"

// 16 bit UUID for this service
#define hrs_UUID BLE_UUID_HEART_RATE_SERVICE

// Handle for the heart rate service
ble_hrs_t hrs_handle;

// Our sensor is located on the user's wrist
uint8_t hrs_BODY_SENSOR_LOCATION = BLE_HRS_BODY_SENSOR_LOCATION_WRIST;

// Initialization for heart rate service
uint32_t hrs_init(void)
{
    uint32_t err_code;

    ble_hrs_init_t init;
    memset(&hrs_init, 0, sizeof(init));

    init.evt_handler = NULL;
    init.is_sensor_contact_supported = false;
    init.p_body_sensor_location = &hrs_BODY_SENSOR_LOCATION;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&init.hrs_hrm_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&init.hrs_hrm_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&init.hrs_hrm_attr_md.write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&init.hrs_bsl_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&init.hrs_bsl_attr_md.write_perm);

    err_code = ble_hrs_init(&hrs_handle, &init);

    return err_code;
}

// FIXME: Get rid of this later when we have real data...
void hrs_update(void)
{
    static uint16_t rate_in_bpm = 0;
    ble_hrs_heart_rate_measurement_send(&hrs_handle, rate_in_bpm++);
}

#endif
