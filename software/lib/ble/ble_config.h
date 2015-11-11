/**
 *  Configuration of the BLE softdevice stack for the fitness watch application.
 */

#ifndef __BLE_CONFIG_H
#define __BLE_CONFIG_H

// TODO add include for ble_gap stuff
//#include "???"

/**
 * No structural changes to attributes are allowed.
 */
#define IS_SRVC_CHANGED_CHARACT_PRESENT 0

// TODO Should we change this or just use the stop_advertising function?
/**
 * Time in seconds for which the device must be advertising. 0 disables timeout.
 */
#define APP_ADV_TIMEOUT_SECONDS         0    

/**
 * The advertising interval. This affects discovery and connect performance.
 * Apple recommends 20ms so we'll do that.
 * https://developer.apple.com/hardwaredrivers/BluetoothDesignGuidelines.pdf
 */
#define APP_ADV_INTERVAL                MSEC_TO_UNITS(20, UNIT_0_625_MS)

// TODO Customize
/** 
 * BLE connection parameters. See link for description.
 * https://devzone.nordicsemi.com/question/60/what-is-connection-parameters/
 */
#define CONN_INTERVAL_MIN               BLE_GAP_CP_MIN_CONN_INTVL_MIN
#define CONN_INTERVAL_MAX               BLE_GAP_CP_MIN_CONN_INTVL_MAX
#define CONN_SLAVE_LATENCY              0
#define CONN_SUPERVISION_TIMEOUT        BLE_GAP_CP_CONN_SUP_TIMEOUT_MAX 


/**
 * Call this to set up the BLE configuration.
 *
 * NOTE: the timers must have already been set up properly otherwise the
 *       softdevice won't work properly.
 */
extern void ble_init(void);

/**
 * Call this to start sending BLE advertising packets.
 */
extern void advertising_start(void);

// TODO finish this function
/**
 * Call this to stop sending BLE advertising packets.
 */
//void advertising_stop(void);

#endif
