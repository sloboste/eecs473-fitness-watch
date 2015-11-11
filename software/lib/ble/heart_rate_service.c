#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nordic_common.h"
#include "ble_hrs.h"
#include "ble_srv_common.h"

#include "heart_rate_service.h"


// Our sensor is located on the user's wrist
static uint8_t hrs_BODY_SENSOR_LOCATION = BLE_HRS_BODY_SENSOR_LOCATION_WRIST;


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

void hrs_update(uint16_t heart_rate_bpm)
{
    ble_hrs_heart_rate_measurement_send(&hrs_handle, heart_rate_bpm);
}
