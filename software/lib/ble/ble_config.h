/*  Configuration of the BLE softdevice stack for the fitness watch application.
 */

#ifndef __BLE_CONFIG_H
#define __BLE_CONFIG_H


// No structural changes to attributes are allowed
#define IS_SRVC_CHANGED_CHARACT_PRESENT 0

// Time in seconds for which the device must be advertising. 0 disables timeout.
#define APP_ADV_TIMEOUT_SECONDS    0    // TODO change

//The advertising interval. This value can be between 100ms to 10.24s.
#define APP_ADV_INTERVAL    MSEC_TO_UNITS(160, UNIT_0_625_MS)

// Connection parameters TODO customize
#define CONN_INTERVAL_MIN           BLE_GAP_CP_MIN_CONN_INTVL_MIN
#define CONN_INTERVAL_MAX           BLE_GAP_CP_MIN_CONN_INTVL_MAX
#define CONN_SLAVE_LATENCY          0
#define CONN_SUPERVISION_TIMEOUT    BLE_GAP_CP_CONN_SUP_TIMEOUT_MAX 


// Call this to set up the BLE configuration.
// NOTE: the timers must have already been set up properly
extern void ble_init(void);

// Call this to start sending BLE advertising packets
extern void advertising_start(void);

// TODO Call this to stop sending BLE advertising packets
//void advertising_stop(void);

#endif
