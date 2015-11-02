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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nordic_common.h"
#include "ble_bas.h"
#include "ble_srv_common.h"

// 16 bit UUID for this service
#define bas_UUID BLE_UUID_BATTERY_SERVICE 

// Handle for the service
ble_bas_t bas_handle;

// The default initial value (in %) of the battery level characteristic
const uint8_t bas_INIT_LEVEL = 50;

// Initialization for heart rate service
uint32_t bas_init(void)
{
    uint32_t err_code;

    ble_bas_init_t bas_init;
    memset(&bas_init, 0, sizeof(bas_init));

    bas_init.evt_handler = NULL;
    bas_init.support_notification = true;
    bas_init.p_report_ref = NULL;
    bas_init.initial_batt_level = bas_INIT_LEVEL;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(
        &bas_init.battery_level_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(
        &bas_init.battery_level_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(
        &bas_init.battery_level_char_attr_md.write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(
        &bas_init.battery_level_report_read_perm);

    err_code = ble_bas_init(&bas_handle, &bas_init);

    return err_code;
}

// FIXME: Get rid of this later when we have real data...
void bas_update(void)
{
    static uint8_t level = 100;
    ble_bas_battery_level_update(&bas_handle, level--);
}

#endif
