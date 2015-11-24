/**
 * Configuration/implementation for the watch BLE service. The watch is the
 * server for a remote client. This service contains two characteristics: a
 * request characteristic and a reply characteristic. The client will issue
 * requests by writing a value to the request characteristic. The watch server
 * will then reply by sending one or more notifications on the reply
 * characteristic. The implementation of the structure of the data sent between
 * the client and server is not defined by this service; it is only limited by
 * the 20 byte per packet maximum specified by the BLE protocol.
 */

#ifndef BLE_WATCH_H
#define BLE_WATCH_H

#include <stdbool.h>
#include <stdint.h>

#include "nordic_common.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_gatts.h"


// The UUID for the watch service is 78c1XXXX-801a-11e5-8bcf-feff819cdc9f.
// Replace the Xs with the 16 bit UUIDs for each characteristic.
#define ble_watch_BASE_UUID {0x9f, 0xdc, 0x9c, 0x81, 0xff, 0xfe, 0xcf, 0x8b,\
                             0xe5, 0x11, 0x1a, 0x80, 0x00, 0x00, 0xc1, 0x78}

#define ble_watch_UUID_SERVICE  0x0001
#define ble_watch_UUID_REQUEST  0x0002
#define ble_watch_UUID_REPLY    0x0003

#define ble_watch_REQUEST_CHAR_LEN  20
#define ble_watch_REPLY_CHAR_LEN    20


// Event handler signature for BLE watch request events.
typedef void (*ble_watch_request_handler_t) (uint8_t * data, uint16_t len);

// The struct representing the BLE watch service information.
typedef struct ble_watch_struct {
    uint16_t service_handle;
    uint16_t conn_handle;
    ble_gatts_char_handles_t request_char_handles;
    ble_gatts_char_handles_t reply_char_handles;
    ble_watch_request_handler_t request_handler;
} ble_watch_t;

/**
 * Set up the BLE watch service.
 *
 * 
 */
extern void ble_watch_init(ble_watch_request_handler_t handler);

/**
 * Call whenever a BLE stack event is received by the application 
 */
extern void ble_watch_on_ble_evt(ble_evt_t * evt_ptr);

/**
 * Update reply characteristic value.
 * When the client has subscribed to the reply characteristic, this function can
 * be used to send notifications to the client that it will interpret as a
 * response to the last thing it wrote to the request characteristic. 
 */ 
extern void ble_watch_send_reply_packet(uint8_t * reply_ptr, uint8_t len);

#endif
