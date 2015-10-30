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

// Include or not the service_changed characteristic. if not enabled, the
// server's database cannot be changed for the lifetime of the device
#define IS_SRVC_CHANGED_CHARACT_PRESENT 0

#define ADVERTISING_LED                 PIN_LED_0   // On when advertising 
#define CONNECTED_LED                   PIN_LED_1   // On when connected

// Time in seconds for which the device must be advertising in non-connectable
// mode. 0 disables timeout.
#define APP_CFG_NON_CONN_ADV_TIMEOUT    0

//The advertising interval for non-connectable advertisement (100 ms). This
// value can vary between 100ms to 10.24s).
#define NON_CONNECTABLE_ADV_INTERVAL    MSEC_TO_UNITS(500, UNIT_0_625_MS)

// Connection parameters
#define CONN_INTERVAL_MIN           BLE_GAP_CP_MIN_CONN_INTVL_MIN
#define CONN_INTERVAL_MAX           BLE_GAP_CP_MIN_CONN_INTVL_MAX
#define CONN_SLAVE_LATENCY          0
#define CONN_SUPERVISION_TIMEOUT    BLE_GAP_CP_CONN_SUP_TIMEOUT_MAX 

#define APP_TIMER_PRESCALER         0 // TODO why?
#define APP_TIMER_MAX_TIMERS        6
#define APP_TIMER_OP_QUEUE_SIZE     4     


static const char * DEVICE_NAME = "Fitness Watch";
static uint16_t current_conn_handle; // Current connection handle



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


// information about the advertisement
ble_advdata_t                           advdata;
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
    ble_gap_conn_sec_mode_t sec_mode;
    ble_gap_conn_params_t conn_params;

    // Full signal strength
    sd_ble_gap_tx_power_set(4);

    // Open conenction (no security)
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    // Set the name of the device so its easier to find
    err_code = sd_ble_gap_device_name_set(
        &sec_mode, (const uint8_t *) DEVICE_NAME, strlen(DEVICE_NAME));

    // Set device appearance
    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_WATCH_SPORTS_WATCH);

    // Set preferred peripheral connection parameters
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

    // Build and set advertising data.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = true;
    advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);

    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(m_adv_params));

    // TODO: change these when it is connectable
    m_adv_params.type = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND;
    m_adv_params.p_peer_addr = NULL;    // Undirected advertisement.
    m_adv_params.fp = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval = NON_CONNECTABLE_ADV_INTERVAL;
    m_adv_params.timeout = APP_CFG_NON_CONN_ADV_TIMEOUT;
}


/**@brief Function for starting advertising.
 */
static void advertising_start(void)
{
    uint32_t err_code;

    err_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(err_code);
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
    // TODO: remove
    nrf_gpio_pin_set(PIN_LED_1);

    //dm_ble_evt_handler(ble_evt_ptr);
    ble_conn_params_on_ble_evt(ble_evt_ptr);
    //on_ble_evt(ble_evt_ptr);
    //ble_advertising_on_ble_evt(ble_evt_ptr);
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


/**
 * @brief Function for application main entry.
 */
int main(void)
{
    // Initialize.
    nrf_gpio_cfg_output(ADVERTISING_LED);
    nrf_gpio_cfg_output(CONNECTED_LED);
    nrf_gpio_pin_clear(ADVERTISING_LED);
    nrf_gpio_pin_clear(CONNECTED_LED);

    timers_init();
    ble_stack_init();
    gap_params_init();
    advertising_init();
    conn_params_init();


    advertising_start();
    nrf_gpio_pin_set(ADVERTISING_LED);

    while (1) {
        power_manage();
    }
}
