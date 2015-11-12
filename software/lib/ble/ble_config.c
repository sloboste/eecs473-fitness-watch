#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "app_timer.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
//#include "nordic_common.h"
//#include "softdevice_handler.h"
//#include "ble_debug_assert_handler.h"
#include "ble_hci.h"

#include "ble_config.h"
#include "battery_service.h"
#include "heart_rate_service.h"
#include "gps_service.h"
#include "ped_service.h"
#include "timer_config.h" // FIXME APP_TIMER_PRESCALER...
//#define APP_TIMER_PRESCALER 0


// Name that the device will advertise
static const char * ble_DEVICE_NAME = "Watchman 0.1";

// Handle for the current connection
static uint16_t ble_current_conn_handle;

// Data sent out in the advertising packet
static ble_advdata_t advdata;

// Data sent out in the scan response packet
static ble_advdata_t srdata;

// Parameters to be passed to the stack when starting advertising
static ble_gap_adv_params_t m_adv_params;


// TODO does this need to be here?
/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product.
 *          You need to analyze how your product is supposed to react in case
 *          of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}


/**
 * Setup GAP paramters (signal strength, name, appearance, preferred connection
 * parameters) on the softdevice.
 */
static void gap_params_init(void)
{
    uint32_t                err_code;

    // Full signal strength
    sd_ble_gap_tx_power_set(4);

    // Do not let the device name be changed remotely
    ble_gap_conn_sec_mode_t sec_mode;
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&sec_mode);

    // Set the name of the device so its easier to find
    err_code = sd_ble_gap_device_name_set(
        &sec_mode, (const uint8_t *) ble_DEVICE_NAME, strlen(ble_DEVICE_NAME));

    // Set device appearance
    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_WATCH_SPORTS_WATCH);

    // Set preferred peripheral connection parameters. The client may not use
    // these but if it did, that would be nice for us.
    ble_gap_conn_params_t conn_params;
    memset(&conn_params, 0, sizeof(conn_params));
    conn_params.min_conn_interval = CONN_INTERVAL_MIN;
    conn_params.max_conn_interval = CONN_INTERVAL_MAX;
    conn_params.slave_latency = CONN_SLAVE_LATENCY;
    conn_params.conn_sup_timeout = CONN_SUPERVISION_TIMEOUT;
    err_code = sd_ble_gap_ppcp_set(&conn_params);

    APP_ERROR_CHECK(err_code);
}


/**
 * Setup the advertising functionality of the softdevice.
 */
static void advertising_init(void)
{
    uint32_t                  err_code;
    ble_uuid_t adv_uuids[] = { 
        {0x1800, BLE_UUID_TYPE_BLE},    // Generic Access Service
        {hrs_UUID, BLE_UUID_TYPE_BLE},
        {bas_UUID, BLE_UUID_TYPE_BLE},
        //{ble_gps_UUID, BLE_UUID_TYPE_VENDOR_BEGIN},
    };
    // TODO: Can't advertise custom 128 bit uuids without changing nordic code.
    //       doing that would exceed advertising packet byte limit anyway so why
    //       do it? 

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
    err_code = ble_advdata_set(&advdata, &srdata);
    APP_ERROR_CHECK(err_code);

    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(m_adv_params));
    m_adv_params.type = BLE_GAP_ADV_TYPE_ADV_IND;
    m_adv_params.p_peer_addr = NULL;    // Undirected advertisement.
    m_adv_params.fp = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval = APP_ADV_INTERVAL;
    m_adv_params.timeout = APP_ADV_TIMEOUT_SECONDS;
}


/**
 * Start the BLE advertising procedure. 
 */
void advertising_start(void)
{
    uint32_t err_code;

    err_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(err_code);
}


static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


// FIXME
/**
 * Function for handling the connection parameters module.
 * This will be called for all events in the connection parameters module which
 * are passed to the application.
 * 
 * NOTE: usually not necessary if we set disconnect on fail to true in the
 *       connection params init.
 */
/*static void on_conn_params_evt(ble_conn_params_evt_t * evt_ptr)
{
    uint32_t err_code;

    if (evt_ptr->evt_type == BLE_CONN_PARAMS_EVT_FAILED) {
        err_code = sd_ble_gap_disconnect(
            ble_current_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}*/


/**
 * Initialize the BLE connection parameters on the softdevice.
 */
static void conn_params_init(void)
{
    uint32_t err_code;
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
    cp_init.disconnect_on_fail              = true; //false;
    cp_init.evt_handler                     = NULL; //on_conn_params_evt;
    cp_init.error_handler                   = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}


/**
 * General handler for a BLE stack event.
 */
static void on_ble_evt(ble_evt_t * ble_evt_ptr)
{
    switch (ble_evt_ptr->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            ble_current_conn_handle = ble_evt_ptr->evt.gap_evt.conn_handle;
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            ble_current_conn_handle = BLE_CONN_HANDLE_INVALID;
            // FIXME Probably don't want to restart advertising all the time
            advertising_start(); 
            break;
        default:
            break;
    }
}


/**
 * Function that dispatches a BLE stack event to all modules with a handler.
 * This function is called from the BLE stack event interrupt handler after a
 * a BLE stack event as been received.
 */
static void ble_evt_dispatch(ble_evt_t * ble_evt_ptr)
{
    on_ble_evt(ble_evt_ptr);
    ble_conn_params_on_ble_evt(ble_evt_ptr);
    ble_hrs_on_ble_evt(&hrs_handle, ble_evt_ptr);
    ble_bas_on_ble_evt(&bas_handle, ble_evt_ptr);
    ble_gps_on_ble_evt(ble_evt_ptr);
    ble_ped_on_ble_evt(ble_evt_ptr);
}


/**
 * Initializes the BLE stack. Initializes the SoftDevice and the BLE interrupt
 * event.
 */
static void ble_stack_init(void)
{
    // TODO Do we want a better clock?
    // Initialize the SoftDevice handler module.
    // Use a really crappy clock because we want fast start
    SOFTDEVICE_HANDLER_INIT(
        NRF_CLOCK_LFCLKSRC_RC_250_PPM_8000MS_CALIBRATION, false);

    // Enable BLE stack
    uint32_t err_code;
    ble_enable_params_t ble_enable_params;
    memset(&ble_enable_params, 0, sizeof(ble_enable_params));
    ble_enable_params.gatts_enable_params.service_changed = 
        IS_SRVC_CHANGED_CHARACT_PRESENT;
    err_code = sd_ble_enable(&ble_enable_params);

    // Register with the softdevice a handler module for BLE events
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}

/**
 * Initialize all of the services for the watch.
 */
static void services_init(void)
{
    uint32_t err_code;

    // Init heart rate service 
    err_code = hrs_init();
    APP_ERROR_CHECK(err_code);

    // Init battery service
    err_code = bas_init();
    APP_ERROR_CHECK(err_code);

    // Init gps service
    err_code = ble_gps_init();
    APP_ERROR_CHECK(err_code);

    // Init pedometer service
    err_code = ble_ped_init();
    APP_ERROR_CHECK(err_code);
}


/**
 * Initialize all of the BLE functionality for the watch.
 */
void ble_init(void)
{
    ble_stack_init();
    gap_params_init();
    advertising_init();
    conn_params_init();
    services_init();
}
