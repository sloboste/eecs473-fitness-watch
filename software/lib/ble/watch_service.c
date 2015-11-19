#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nordic_common.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_gatts.h"

#include "watch_service.h"

//FIXME remove
#include "nrf_gpio.h"
#include "blue_dev_board.h"
//---

// Object to represent the watch service
static ble_watch_t ble_watch_object;

// Request characteristic
static uint8_t request_buf[ble_watch_REQUEST_CHAR_LEN];
static char * request_name = "Request";

// Reply characteristic
static uint8_t reply_buf[ble_watch_REPLY_CHAR_LEN];
static char * reply_name = "Reply";
//bool reply_notifications_enabled = false;


uint32_t ble_watch_send_reply_packet(uint8_t * reply_ptr, uint8_t len)
{
    uint32_t err_code = NRF_SUCCESS;

    // Sanity check
    if (len > ble_watch_REPLY_CHAR_LEN) {
        return 1; // FIXME
    }

    // Build a characteristic value
    ble_gatts_value_t gatts_value;
    memset(&gatts_value, 0, sizeof(gatts_value));
    
    gatts_value.len = len; 
    gatts_value.offset = 0;
    gatts_value.p_value = reply_ptr; 
    
    // Set the new characteristic value 
    err_code = sd_ble_gatts_value_set(
        ble_watch_object.conn_handle,
        ble_watch_object.reply_char_handles.value_handle,
        &gatts_value);

    if (err_code != NRF_SUCCESS) {
        return err_code;
    }

    // Send notification if needed
    if (ble_watch_object.conn_handle != BLE_CONN_HANDLE_INVALID) {
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));
        
        hvx_params.handle = ble_watch_object.reply_char_handles.value_handle,
        hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(ble_watch_object.conn_handle, &hvx_params);
    }
    
    return err_code;
}

/**
 * Handle the event when the client connects to this device.
 */
static void on_connect(ble_evt_t * evt_ptr)
{
    ble_watch_object.conn_handle = evt_ptr->evt.gap_evt.conn_handle;
}

/**
 * Handle the event when the client disconnects from this device.
 */
static void on_disconnect(ble_evt_t * evt_ptr)
{
    ble_watch_object.conn_handle = BLE_CONN_HANDLE_INVALID;
}

/**
 * Handle a GATT service write event.
 * This will call the request_handler when the request characteristic is
 * written.
 */
static void on_write(ble_evt_t * evt_ptr)
{
    ble_gatts_evt_write_t * evt_write_ptr = &evt_ptr->evt.gatts_evt.params.write;

    if ((evt_write_ptr->handle ==
         ble_watch_object.request_char_handles.value_handle) &&
        (evt_write_ptr->len <= ble_watch_REQUEST_CHAR_LEN)) {

        // FIXME remove
        //ble_watch_send_reply_packet(evt_write_ptr->data, evt_write_ptr->len);

        // Call event handler
        if (ble_watch_object.request_handler != NULL) {
            ble_watch_object.request_handler(evt_write_ptr->data,
                                             evt_write_ptr->len); 
        } 
    }
}

void ble_watch_on_ble_evt(ble_evt_t * evt_ptr)
{
    switch (evt_ptr->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(evt_ptr);
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(evt_ptr);
            break;
        case BLE_GATTS_EVT_WRITE:
            on_write(evt_ptr);
            break;
    }
}

/**
 * Add a custom characteristic to the watch service.
 *
 * uuid -- the 16 bit UUID for the charactersitic to combine with the 128 bit   
 *         service UUID.                                                        
 * value_len -- length in bytes of the characteristic value.                    
 * name -- char array that is the name to display for this characteristic.      
 * read -- does the client have read access to this characteristic? Boolean.    
 * write -- does the client have write access to this characteristic? Boolean.  
 * notify -- does this characteristic support notifications? Boolean.           
 * char_handles_ptr -- the ble_gatts_char_handles_t object dedicated to store   
 *                     this characteristic's attribute handles. 
 */
static uint32_t char_add(uint16_t uuid, uint16_t value_len, char * value_buf,
                         uint8_t name_len, char * name, uint8_t read,
                         uint8_t write, uint8_t notify,
                         ble_gatts_char_handles_t * char_handles_ptr)
{
    uint32_t err_code;
    
    // Setup the characteristic UUID with the softdevice.
    ble_uuid_t char_uuid;
    ble_uuid128_t base_uuid = ble_watch_BASE_UUID;
    char_uuid.uuid = uuid;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    
    // Setup the characteristic value attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    if (read) {
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    } else {
        BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    }
    if (write) {
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    } else {
        BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    }

    // Setup the client characteristic configuration descriptor attribute
    // metadata. 
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    // Setup the user description characteristic attribute metadata.
    ble_gatts_attr_md_t desc_md;
    memset(&desc_md, 0, sizeof(desc_md));
    desc_md.vloc = BLE_GATTS_VLOC_STACK;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&desc_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&desc_md.write_perm);
    
    // Setup the characteristic attribute metadata
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = read;
    char_md.char_props.write = write;
    char_md.char_props.notify = notify;
    char_md.char_user_desc_max_size = name_len;
    char_md.char_user_desc_size = name_len;
    char_md.p_char_user_desc = name;
    char_md.p_user_desc_md = &desc_md;
    char_md.p_cccd_md = &cccd_md;

    // Create the characteristic value attribute
    ble_gatts_attr_t attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = value_len;
    attr_char_value.init_offs = 0;
    // NOTE: possible error when value_len > 512 ??
    attr_char_value.max_len   = value_len;
    attr_char_value.p_value   = value_buf;
    
    // Add the characteristic we just built
    err_code = sd_ble_gatts_characteristic_add(
        ble_watch_object.service_handle, &char_md, &attr_char_value,
        char_handles_ptr);
    
    return err_code;
}

uint32_t ble_watch_init(ble_watch_request_handler_t handler)
{
    uint32_t err_code;

    // Register the service UUID with the softdevice
    ble_uuid_t service_uuid;
    ble_uuid128_t base_uuid = ble_watch_BASE_UUID;
    service_uuid.uuid = ble_watch_UUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);

    // Register the service with the softdevice
    err_code = sd_ble_gatts_service_add(
        BLE_GATTS_SRVC_TYPE_PRIMARY,
        &service_uuid,
        &(ble_watch_object.service_handle));

    // Register the request characteristic with the softdevice
    err_code = char_add(
        ble_watch_UUID_REQUEST, ble_watch_REQUEST_CHAR_LEN, &request_buf,
        strlen(request_name), request_name, 0, 1, 0,
        &ble_watch_object.request_char_handles);

    // Register the reply characteristic with the softdevice
    err_code = char_add(
        ble_watch_UUID_REPLY, ble_watch_REPLY_CHAR_LEN, &reply_buf,
        strlen(reply_name), reply_name, 1, 0, 1,
        &ble_watch_object.reply_char_handles);

    // Register the request handler with the watch service
    ble_watch_object.request_handler = handler;

    // FIXME bad err_code will be ignored if not last value
    return err_code;
}
