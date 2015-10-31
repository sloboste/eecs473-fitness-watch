#include <stdbool.h>
#include <stdint.h>

#include "green_dev_board.h"

#include "nrf.h"
#include "nrf_gpio.h"
#include "ble_advdata.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "ble_debug_assert_handler.h"
#include "ble_conn_params.h"
#include "ble_hci.h"
#include "app_timer.h"
#include "app_scheduler.h"
#include "app_timer_appsh.h"
#include "ble_hrs.h"
#include "ble_bas.h"

#include "nrf_delay.h"

// Include or not the service_changed characteristic. if not enabled, the
// server's database cannot be changed for the lifetime of the device
#define IS_SRVC_CHANGED_CHARACT_PRESENT 0

#define ADVERTISING_LED                 PIN_LED_0   // On when advertising 
#define CONNECTED_LED                   PIN_LED_1   // On when connected

#define ADVERTISING_BUTTON              PIN_BUTTON_0  // Activate advertising

// Time in seconds for which the device must be advertising. 0 disables timeout.
#define APP_ADV_TIMEOUT_SECONDS    0

//The advertising interval. This value can be between 100ms to 10.24s.
#define APP_ADV_INTERVAL    MSEC_TO_UNITS(160, UNIT_0_625_MS)

// Connection parameters
#define CONN_INTERVAL_MIN           BLE_GAP_CP_MIN_CONN_INTVL_MIN
#define CONN_INTERVAL_MAX           BLE_GAP_CP_MIN_CONN_INTVL_MAX
#define CONN_SLAVE_LATENCY          0
#define CONN_SUPERVISION_TIMEOUT    BLE_GAP_CP_CONN_SUP_TIMEOUT_MAX 

#define APP_TIMER_PRESCALER         0 // TODO why?
#define APP_TIMER_MAX_TIMERS        6
#define APP_TIMER_OP_QUEUE_SIZE     4     

#define SCHED_MAX_EVENT_DATA_SIZE   sizeof(app_timer_event_t)
#define SCHED_QUEUE_SIZE            10

static const char * DEVICE_NAME = "Watchman 1.0";
static uint16_t current_conn_handle; // Current connection handle


// handle for the heart rate service
ble_hrs_t hrs_handle;
uint8_t hrs_body_sensor_location = BLE_HRS_BODY_SENSOR_LOCATION_WRIST;
static void heart_rate_update(void);

// handle for the battery level service
ble_bas_t bas_handle;
static void battery_update(void);


void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);
static void gap_params_init(void);
static void advertising_init(void);
static void advertising_start(void);
static void ble_stack_init(void);
static void power_manage(void);
static void ble_evt_dispatch(ble_evt_t * ble_evt_ptr);
static void conn_params_init(void);
static void on_conn_params_evt(ble_conn_params_evt_t * evt_ptr);
static void conn_params_error_handler(uint32_t nrf_error);
static void timers_init(void);
static void on_ble_evt(ble_evt_t * ble_evt_ptr);


// information about the advertisement
ble_advdata_t                           advdata;
ble_advdata_t                           scanrspdata;
// Parameters to be passed to the stack when starting advertising
static ble_gap_adv_params_t             m_adv_params;


/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    // This call can be used for debug purposes during application development.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Un-comment the line below to use.
    // ble_debug_assert_handler(error_code, line_num, p_file_name);

    // On assert, the system can only recover on reset.
    NVIC_SystemReset();
}


/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}

// Setup TX power and the device name
static void gap_params_init(void)
{
    uint32_t                err_code;

    // Full signal strength
    sd_ble_gap_tx_power_set(4);

    // Open conenction (no security)
    ble_gap_conn_sec_mode_t sec_mode;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    // Set the name of the device so its easier to find
    err_code = sd_ble_gap_device_name_set(
        &sec_mode, (const uint8_t *) DEVICE_NAME, strlen(DEVICE_NAME));

    // Set device appearance
    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_WATCH_SPORTS_WATCH);

    // Set preferred peripheral connection parameters
    ble_gap_conn_params_t conn_params;
    memset(&conn_params, 0, sizeof(conn_params));
    conn_params.min_conn_interval = CONN_INTERVAL_MIN;
    conn_params.max_conn_interval = CONN_INTERVAL_MAX;
    conn_params.slave_latency = CONN_SLAVE_LATENCY;
    conn_params.conn_sup_timeout = CONN_SUPERVISION_TIMEOUT;
    err_code = sd_ble_gap_ppcp_set(&conn_params);

    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 */
static void advertising_init(void)
{
    uint32_t                  err_code;
    ble_uuid_t adv_uuids[] = { 
        {0x1800, BLE_UUID_TYPE_BLE},    // Generic Access Service
        {BLE_UUID_HEART_RATE_SERVICE, BLE_UUID_TYPE_BLE},
        {BLE_UUID_BATTERY_SERVICE, BLE_UUID_TYPE_BLE},
    };
    // TODO: can't advertise custom 128 bit uuids without modifying nordic code

    // Build and set advertising and scan response data.
    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = true;
    advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE; // TODO want to be le_only_limited_disc
    advdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    advdata.uuids_complete.p_uuids = adv_uuids;

    /*
    memset(&scanrspdata, 0, sizeof(scanrspdata));
    scanrspdata.service_data_count = 1;
    p_service_data_array = services_ptr;
    */

    err_code = ble_advdata_set(&advdata, NULL); //&scanrspdata);
    APP_ERROR_CHECK(err_code);

    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(m_adv_params));
    m_adv_params.type = BLE_GAP_ADV_TYPE_ADV_IND;
    m_adv_params.p_peer_addr = NULL;    // Undirected advertisement.
    m_adv_params.fp = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval = APP_ADV_INTERVAL;
    m_adv_params.timeout = APP_ADV_TIMEOUT_SECONDS;
    
}


/**@brief Function for starting advertising.
 */
static void advertising_start(void)
{
    uint32_t err_code;

    err_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(err_code);
    nrf_gpio_pin_set(ADVERTISING_LED);
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{

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

/**@brief Function for doing power management.
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}





// function for dispatching a BLE stack event to all modules with a BLE stack
// handler. This function is called from the BLE stack event interrupt handler
// after a BLE stack event has been received.
static void ble_evt_dispatch(ble_evt_t * ble_evt_ptr)
{
    on_ble_evt(ble_evt_ptr);
    ble_conn_params_on_ble_evt(ble_evt_ptr);
    //ble_advertising_on_ble_evt(ble_evt_ptr);
    //dm_ble_evt_handler(ble_evt_ptr);
    ble_hrs_on_ble_evt(&hrs_handle, ble_evt_ptr);
    ble_bas_on_ble_evt(&bas_handle, ble_evt_ptr);
}

// Initialize the connection parameters module
static void conn_params_init(void)
{
    uint32_t err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                   = NULL;
    cp_init.first_conn_params_update_delay  = APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER);
    cp_init.next_conn_params_update_delay   = APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER); // 30 seconds
    cp_init.max_conn_params_update_count    = 3;
    cp_init.start_on_notify_cccd_handle     = BLE_GATT_HANDLE_INVALID;  // Start procedure on connection
    cp_init.disconnect_on_fail              = false;
    cp_init.evt_handler                     = on_conn_params_evt;
    cp_init.error_handler                   = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

// function for handling the connection parameters module
// This will be called for all events in the connection parameters module which
// are passed to the application.
static void on_conn_params_evt(ble_conn_params_evt_t * evt_ptr)
{
    uint32_t err_code;

    if (evt_ptr->evt_type == BLE_CONN_PARAMS_EVT_FAILED) {
        err_code = sd_ble_gap_disconnect(current_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}

static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

static void timers_init(void)
{
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS,
                   APP_TIMER_OP_QUEUE_SIZE, false);
}

static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

static void on_ble_evt(ble_evt_t * ble_evt_ptr)
{
    uint32_t err_code;    

    switch (ble_evt_ptr->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            current_conn_handle = ble_evt_ptr->evt.gap_evt.conn_handle;
            nrf_gpio_pin_set(CONNECTED_LED);
            nrf_gpio_pin_clear(ADVERTISING_LED);
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            current_conn_handle = BLE_CONN_HANDLE_INVALID;
            nrf_gpio_pin_clear(CONNECTED_LED);
            advertising_start();
            break;
        // TODO: add more if needed
        default:
            break;
    }
}

static void battery_update(void)
{
    static uint8_t level = 100;
    ble_bas_battery_level_update(&bas_handle, level--);
}

static void heart_rate_update(void)
{
    static uint16_t rate_in_bpm = 0;
    ble_hrs_heart_rate_measurement_send(&hrs_handle, rate_in_bpm++);
}

void services_init(void)
{
    uint32_t err_code;

    // Init heart rate service 
    ble_hrs_init_t hrs_init;
    memset(&hrs_init, 0, sizeof(hrs_init));
    hrs_init.evt_handler = NULL;
    hrs_init.is_sensor_contact_supported = false;
    hrs_init.p_body_sensor_location = &hrs_body_sensor_location;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&hrs_init.hrs_hrm_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&hrs_init.hrs_hrm_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&hrs_init.hrs_hrm_attr_md.write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&hrs_init.hrs_bsl_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&hrs_init.hrs_bsl_attr_md.write_perm);
    err_code = ble_hrs_init(&hrs_handle, &hrs_init);
    APP_ERROR_CHECK(err_code);

    // Init battery service
    ble_bas_init_t bas_init;
    memset(&bas_init, 0, sizeof(bas_init));
    bas_init.evt_handler = NULL;
    bas_init.support_notification = true;
    bas_init.p_report_ref = NULL;
    bas_init.initial_batt_level = 100;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(
        &bas_init.battery_level_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(
        &bas_init.battery_level_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(
        &bas_init.battery_level_char_attr_md.write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(
        &bas_init.battery_level_report_read_perm);
    err_code = ble_bas_init(&bas_handle, &bas_init);
    APP_ERROR_CHECK(err_code);
}


/**
 * @brief Function for application main entry.
 */
int main(void)
{
    // Initialize.
    nrf_gpio_cfg_input(ADVERTISING_BUTTON, GPIO_PIN_CNF_PULL_Pullup);

    nrf_gpio_cfg_output(ADVERTISING_LED);
    nrf_gpio_cfg_output(CONNECTED_LED);

    nrf_gpio_pin_clear(ADVERTISING_LED);
    nrf_gpio_pin_clear(CONNECTED_LED);

    timers_init();
    ble_stack_init();
    scheduler_init();
    gap_params_init();
    advertising_init();
    conn_params_init();
    services_init();

    // Wait for button press
    while (nrf_gpio_pin_read(ADVERTISING_BUTTON)) {}; 

    advertising_start();

    while (1) {
        //app_sched_execute();
        power_manage();

        // FIXME remove later
        battery_update();
        heart_rate_update();
    }
}
