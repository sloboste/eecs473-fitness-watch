#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nordic_common.h"
#include "ble_bas.h"
#include "ble_srv_common.h"

#include "battery_service.h"


// The default initial value (in %) of the battery level characteristic
static const uint8_t bas_INIT_LEVEL = 50;


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

void bas_update(uint8_t battery_level)
{
    ble_bas_battery_level_update(&bas_handle, battery_level);
}
