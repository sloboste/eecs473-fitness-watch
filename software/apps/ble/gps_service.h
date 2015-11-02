/*  Configuration/implementation for a BLE custom GPS service.
 */

#ifndef __BLE_GPS_H
#define __BLE_GPS_H

#include <stdbool.h>
#include <stdint.h>

#include "nordic_common.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_gatts.h"


// 58c1XXXX-801a-11e5-8bcf-feff819cdc9f
#define ble_gps_BASE_UUID {0x9f, 0xdc, 0x9c, 0x81, 0xff, 0xfe, 0xcf, 0x8b, 0xe5, 0x11, 0x1a, 0x80, 0x00, 0x00, 0xc1, 0x58}
#define ble_gps_UUID 0x0001
#define ble_gps_UUID_LOCATION 0x0002
#define ble_gps_UUID_SPEED 0x0003
#define ble_gps_UUID_STATUS 0x0004

// TODO we might not need this...
// The types of events that may occur in the GPS service.
typedef enum {
    TODO, // FIXME invalid 
} ble_gps_evt_type_t;

// The type definition for a BLE GPS service event. 
typedef struct {
    ble_gps_evt_type_t evt_type;
} ble_gps_evt_t;

// Forward declaration of ble_gps_t type.
typedef struct ble_gps_struct ble_gps_t; 

// TODO we might not need this...
// Event handler signature for BLE GPS events
typedef void (*ble_gps_evt_handler_t) (ble_gps_evt_t * evt_ptr);

// The struct representing the BLE GPS service information.
struct ble_gps_struct {
    uint16_t service_handle;
    uint16_t conn_handle;
    ble_gatts_char_handles_t location_char_handles;
    ble_gatts_char_handles_t speed_char_handles;
    ble_gatts_char_handles_t status_char_handles;
    ble_gps_evt_handler_t evt_handler; // TODO we might not need this
};

// Initialize the gps service
extern uint32_t ble_gps_init(void);

// Call whenever a BLE stack event is received by the application 
extern void ble_gps_on_ble_evt(ble_evt_t * evt_ptr);

// Call to update location characteristic
extern uint32_t ble_gps_update_location(uint32_t location);

// Call to update speed characteristic
extern uint32_t ble_gps_update_speed(uint32_t speed);

// Call to update status characteristic
extern uint32_t ble_gps_update_status(uint8_t status);

#endif
