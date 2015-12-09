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
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "app_timer.h"
#include "app_error.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "ble_debug_assert_handler.h"
#include "ble_hci.h"

#include "ble_config.h"
#include "watch_service.h"
#include "timer_config.h" // APP_TIMER_PRESCALER


// Name that the device will advertise
static const char * ble_DEVICE_NAME = "Eir Watch";

// Handle for the current connection
static uint16_t ble_current_conn_handle;

// Data sent out in the advertising packet
static ble_advdata_t advdata;

// Data sent out in the scan response packet
static ble_advdata_t srdata;

// Parameters to be passed to the stack when starting advertising
static ble_gap_adv_params_t m_adv_params;

// Call this when advertising start/stop or connect/disconnect
static void (*on_adv_con) (uint8_t);


/**
 * Callback function for asserts in the SoftDevice.
 * 
 * Note: parameters are for debugging and are unused.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}

/**
 * Setup GAP paramters (signal strength, name, appearance, preferred connection
 * parameters) on the softdevice.
 */
static void gap_params_init()
{
    uint32_t error_code;

    // Full signal strength
    error_code = sd_ble_gap_tx_power_set(4);
    APP_ERROR_CHECK(error_code);

    // Do not let the device name be changed remotely
    ble_gap_conn_sec_mode_t sec_mode;
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&sec_mode);

    // Set the name of the device so its easier to find
    error_code = sd_ble_gap_device_name_set(
        &sec_mode, (const uint8_t *) ble_DEVICE_NAME, strlen(ble_DEVICE_NAME));
    APP_ERROR_CHECK(error_code);

    // Set device appearance
    error_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_WATCH_SPORTS_WATCH);
    APP_ERROR_CHECK(error_code);

    // Set preferred peripheral connection parameters. The client may not use
    // these but if it did, that would be nice for us.
    ble_gap_conn_params_t conn_params;
    memset(&conn_params, 0, sizeof(conn_params));
    conn_params.min_conn_interval = CONN_INTERVAL_MIN;
    conn_params.max_conn_interval = CONN_INTERVAL_MAX;
    conn_params.slave_latency = CONN_SLAVE_LATENCY;
    conn_params.conn_sup_timeout = CONN_SUPERVISION_TIMEOUT;
    error_code = sd_ble_gap_ppcp_set(&conn_params);
    APP_ERROR_CHECK(error_code);
}

/**
 * Setup the advertising functionality of the softdevice.
 */
static void advertising_init()
{
    uint32_t error_code;
    ble_uuid_t adv_uuids[] = { 
        {0x1800, BLE_UUID_TYPE_BLE},    // Generic Access Service
    };
    // TODO Can't advertise custom 128 bit uuids without changing nordic code.

    // Build advertising data.
    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = true;
    // TODO Do we want it to be le_only_limited_disc?
    advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    // Bulid scan response data. 
    memset(&srdata, 0, sizeof(srdata));
    srdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    srdata.uuids_complete.p_uuids = adv_uuids;

    // Set advertising and scan response data in the softdevice.
    error_code = ble_advdata_set(&advdata, &srdata);
    APP_ERROR_CHECK(error_code);

    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(m_adv_params));
    m_adv_params.type = BLE_GAP_ADV_TYPE_ADV_IND;
    m_adv_params.p_peer_addr = NULL;    // Undirected advertisement.
    m_adv_params.fp = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval = APP_ADV_INTERVAL;
    m_adv_params.timeout = APP_ADV_TIMEOUT_SECONDS;
}

void ble_advertising_start()
{
    uint32_t error_code;
    error_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(error_code);
    on_adv_con(BLE_STATE_ADVERTISING);
}

void ble_advertising_stop()
{
    uint32_t error_code;
    error_code = sd_ble_gap_adv_stop();
    APP_ERROR_CHECK(error_code);
    on_adv_con(BLE_STATE_IDLE);
}

static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/**
 * Initialize the BLE connection parameters on the softdevice.
 */
static void conn_params_init()
{
    uint32_t error_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                   = NULL;
    cp_init.first_conn_params_update_delay  =
        APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER);
    cp_init.next_conn_params_update_delay   =
        APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER); // 30 seconds
    cp_init.max_conn_params_update_count    = 3;
    cp_init.start_on_notify_cccd_handle     =
        BLE_GATT_HANDLE_INVALID;  // Start procedure on connection
    cp_init.disconnect_on_fail              = true;
    cp_init.evt_handler                     = NULL;
    cp_init.error_handler                   = conn_params_error_handler;

    error_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(error_code);
}

/**
 * General handler for a BLE stack event.
 */
static void on_ble_evt(ble_evt_t * ble_evt_ptr)
{
    switch (ble_evt_ptr->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            ble_current_conn_handle = ble_evt_ptr->evt.gap_evt.conn_handle;
            on_adv_con(BLE_STATE_CONNECTED);
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            ble_current_conn_handle = BLE_CONN_HANDLE_INVALID;
            on_adv_con(BLE_STATE_IDLE);
            break;
        default:
            break;
    }
}

/**
 * Function that dispatches a BLE stack event to all modules with a handler.
 * This function is called from the BLE stack event interrupt handler after a
 * a BLE stack event as been received. This function executes in the interrupt
 * context.
 */
static void ble_evt_dispatch(ble_evt_t * ble_evt_ptr)
{
    on_ble_evt(ble_evt_ptr);
    ble_conn_params_on_ble_evt(ble_evt_ptr);
    ble_watch_on_ble_evt(ble_evt_ptr);
}

/**
 * Initializes the BLE stack. Initializes the SoftDevice and the BLE interrupt
 * event.
 */
static void ble_stack_init()
{
    uint32_t error_code;

    // Initialize the SoftDevice handler module. 
    // Use a really crappy clock because we want fast start. TODO Do we want a better clock?
    SOFTDEVICE_HANDLER_INIT(
        NRF_CLOCK_LFCLKSRC_RC_250_PPM_8000MS_CALIBRATION, false);

    // Enable BLE stack
    ble_enable_params_t ble_enable_params;
    memset(&ble_enable_params, 0, sizeof(ble_enable_params));
    ble_enable_params.gatts_enable_params.service_changed = 
        IS_SRVC_CHANGED_CHARACT_PRESENT;
    error_code = sd_ble_enable(&ble_enable_params);
    APP_ERROR_CHECK(error_code);

    // Register with the softdevice a handler module for BLE events
    error_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(error_code);
}

void ble_init(ble_watch_request_handler_t handler,
              void (*on_ble_adv_con)(uint8_t))
{
    // Register handler
    on_adv_con = on_ble_adv_con;

    // Init Nordic stack
    ble_stack_init();
    gap_params_init();
    advertising_init();
    conn_params_init();

    // Init watch request reply service
    ble_watch_init(handler);
}
