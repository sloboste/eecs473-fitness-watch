/**
 * Configuration/implementation for a BLE custom pedometer service.
 */

#ifndef __BLE_PED_H
#define __BLE_PED_H

#include <stdbool.h>
#include <stdint.h>

#include "nordic_common.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_gatts.h"

// Following is a random UUID for the pedometer service.
// 68c1XXXX-801a-11e5-8bcf-feff819cdc9f
// Replace the Xs with the 16 bit UUIDs for each characteristic.
#define ble_ped_BASE_UUID {0x9f, 0xdc, 0x9c, 0x81, 0xff, 0xfe, 0xcf, 0x8b, 0xe5, 0x11, 0x1a, 0x80, 0x00, 0x00, 0xc1, 0x68}
#define ble_ped_UUID 0x0001
#define ble_ped_UUID_STEP_COUNT 0x0002
#define ble_ped_UUID_STATUS 0x0003

// TODO we might not need this...
// The types of events that may occur in the pedometer service.
typedef enum {
    PED_TODO, // FIXME invalid 
} ble_ped_evt_type_t;

// The type definition for a BLE pedometer service event. 
typedef struct {
    ble_ped_evt_type_t evt_type;
} ble_ped_evt_t;

// Forward declaration of ble_ped_t type.
typedef struct ble_ped_struct ble_ped_t; 

// TODO we might not need this...
// Event handler signature for BLE pedometer events
typedef void (*ble_ped_evt_handler_t) (ble_ped_evt_t * evt_ptr);

// The struct representing the BLE pedometer service information.
struct ble_ped_struct {
    uint16_t service_handle;
    uint16_t conn_handle;
    ble_gatts_char_handles_t step_count_char_handles;
    ble_gatts_char_handles_t status_char_handles;
    ble_ped_evt_handler_t evt_handler; // TODO we might not need this
};

/**
 * Set up the pedometer service.
 */
extern uint32_t ble_ped_init(void);

/**
 * Call whenever a BLE stack event is received by the application 
 */
extern void ble_ped_on_ble_evt(ble_evt_t * evt_ptr);

/**
 * Update step_count characteristic value.
 */ 
extern uint32_t ble_ped_update_step_count(uint32_t step_count);

/** 
 * Update status characteristic value.
 */
extern uint32_t ble_ped_update_status(uint8_t status);

#endif
