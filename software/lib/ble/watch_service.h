/**
 * Configuration/implementation for the watch BLE service.
 */

#ifndef __BLE_WATCH_H
#define __BLE_WATCH_H

#include <stdbool.h>
#include <stdint.h>

#include "nordic_common.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_gatts.h"

// Following is a random UUID for the watch service.
// 78c1XXXX-801a-11e5-8bcf-feff819cdc9f
// Replace the Xs with the 16 bit UUIDs for each characteristic.
#define ble_watch_BASE_UUID {0x9f, 0xdc, 0x9c, 0x81, 0xff, 0xfe, 0xcf, 0x8b, 0xe5, 0x11, 0x1a, 0x80, 0x00, 0x00, 0xc1, 0x78}
#define ble_watch_UUID 0x0001
#define ble_watch_UUID_REQUEST 0x0002
#define ble_watch_UUID_REPLY 0x0003

#define ble_watch_REQUEST_CHAR_LEN  20
#define ble_watch_REPLY_CHAR_LEN    20

// TODO we might not need this...
// The types of events that may occur in the service.
typedef enum {
    WATCH_TODO, // FIXME invalid 
} ble_watch_evt_type_t;

// The type definition for a BLE watch service event. 
typedef struct {
    ble_watch_evt_type_t evt_type;
} ble_watch_evt_t;

// Forward declaration of ble_watch_t type.
typedef struct ble_watch_struct ble_watch_t; 

// Event handler signature for BLE watch events
typedef void (*ble_watch_request_handler_t) (uint8_t * data, uint16_t len);

// The struct representing the BLE watch service information.
struct ble_watch_struct {
    uint16_t service_handle;
    uint16_t conn_handle;
    ble_gatts_char_handles_t request_char_handles;
    ble_gatts_char_handles_t reply_char_handles;
    ble_watch_request_handler_t request_handler;
};

/**
 * Set up the watch service.
 */
extern uint32_t ble_watch_init(ble_watch_request_handler_t handler);

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
extern uint32_t ble_watch_send_reply_packet(uint8_t * reply_ptr, uint8_t len);

#endif
