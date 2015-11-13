#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nordic_common.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_gatts.h"

#include "gps_service.h"


// Object to represent the gps service
static ble_gps_t ble_gps_object;

// Latitude characteristic
#define LATITUDE_LEN 16
static char latitude_buf[LATITUDE_LEN];
static char * latitude_name = "GPS Latutude";
static bool latitude_notifications_enabled = false; 

// Longitude characteristic
//#define LONGITUDE_LEN 16
//static char longitude_buf[LONGITUDE_LEN];
//static char * longitude_name = "GPS Longitude";
//static bool longitude_notifications_enabled = false; 

// Speed characteristic
//#define SPEED_LEN 4
//static char speed_buf[SPEED_LEN];
//static char * speed_name = "GPS Speed";
//static bool speed_notifications_enabled = false; 

// Status characteristic
#define STATUS_LEN 1
static char status_buf[STATUS_LEN];
static char * status_name = "GPS Status";


uint32_t ble_gps_update_latitude(char * location, uint32_t len)
{
    uint32_t err_code = NRF_SUCCESS;

    // Build a characteristic value
    ble_gatts_value_t gatts_value;
    memset(&gatts_value, 0, sizeof(gatts_value));
    
    gatts_value.len = LATITUDE_LEN; 
    gatts_value.offset = 0;
    gatts_value.p_value = (uint8_t *) location; 
    
    // Set the new characteristic value
    err_code = sd_ble_gatts_value_set(
        ble_gps_object.conn_handle,
        ble_gps_object.latitude_char_handles.value_handle,
        &gatts_value);

    if (err_code != NRF_SUCCESS) {
        return err_code;
    }

    // Send notification if needed
    if (ble_gps_object.conn_handle != BLE_CONN_HANDLE_INVALID) {
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));
        
        hvx_params.handle = ble_gps_object.latitude_char_handles.value_handle,
        hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(ble_gps_object.conn_handle, &hvx_params);
    }
    
    return err_code;
}

/*
uint32_t ble_gps_update_longitude(char * location, uint32_t len)
{
    uint32_t err_code = NRF_SUCCESS;

    // Build a characteristic value
    ble_gatts_value_t gatts_value;
    memset(&gatts_value, 0, sizeof(gatts_value));
    
    gatts_value.len = LONGITUDE_LEN; 
    gatts_value.offset = 0;
    gatts_value.p_value = (uint8_t *) location; 
    
    // Set the new characteristic value
    err_code = sd_ble_gatts_value_set(
        ble_gps_object.conn_handle,
        ble_gps_object.longitude_char_handles.value_handle,
        &gatts_value);

    if (err_code != NRF_SUCCESS) {
        return err_code;
    }

    // Send notification if needed
    if (ble_gps_object.conn_handle != BLE_CONN_HANDLE_INVALID) {
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));
        
        hvx_params.handle = ble_gps_object.longitude_char_handles.value_handle,
        hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(ble_gps_object.conn_handle, &hvx_params);
    }
    
    return err_code;
}

uint32_t ble_gps_update_speed(uint32_t speed)
{
    uint32_t err_code = NRF_SUCCESS;
    
    // Bytes will be sent in reverse order
    uint8_t speed_reversed[4] = {
        ((uint8_t *) &speed)[3],
        ((uint8_t *) &speed)[2],
        ((uint8_t *) &speed)[1],
        ((uint8_t *) &speed)[0],
    };

    // Build a characteristic value
    ble_gatts_value_t gatts_value;
    memset(&gatts_value, 0, sizeof(gatts_value));
    
    gatts_value.len = SPEED_LEN; 
    gatts_value.offset = 0;
    gatts_value.p_value = (uint8_t *) &speed_reversed; 
    
    // Set the new characteristic value
    err_code = sd_ble_gatts_value_set(
        ble_gps_object.conn_handle,
        ble_gps_object.speed_char_handles.value_handle,
        &gatts_value);

    if (err_code != NRF_SUCCESS) {
        return err_code;
    }

    // Send notification if needed
    if (ble_gps_object.conn_handle != BLE_CONN_HANDLE_INVALID) {
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));
        
        hvx_params.handle = ble_gps_object.speed_char_handles.value_handle,
        hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(ble_gps_object.conn_handle, &hvx_params);
    }
    
    return err_code;
}

uint32_t ble_gps_update_status(uint8_t status)
{
    uint32_t err_code = NRF_SUCCESS;

    // Build a characteristiv value
    ble_gatts_value_t gatts_value;
    memset(&gatts_value, 0, sizeof(gatts_value));
    
    gatts_value.len = STATUS_LEN; 
    gatts_value.offset = 0;
    gatts_value.p_value = (uint8_t *) &status; 
    
    // Set the new characteristic value
    err_code = sd_ble_gatts_value_set(
        ble_gps_object.conn_handle,
        ble_gps_object.status_char_handles.value_handle,
        &gatts_value);

    if (err_code != NRF_SUCCESS) {
        return err_code;
    }

    // Send notification if needed
    if (ble_gps_object.conn_handle != BLE_CONN_HANDLE_INVALID) {
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));
        
        hvx_params.handle = ble_gps_object.status_char_handles.value_handle,
        hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(ble_gps_object.conn_handle, &hvx_params);
    }
    
    return err_code;
}
*/

/**
 * Handle the event when the client connects to this device.
 */
static void on_connect(ble_evt_t * evt_ptr)
{
    ble_gps_object.conn_handle = evt_ptr->evt.gap_evt.conn_handle;
}

/**
 * Handle the event when the client disconnects from this device.
 */
static void on_disconnect(ble_evt_t * evt_ptr)
{
    ble_gps_object.conn_handle = BLE_CONN_HANDLE_INVALID;
}

// We don't need to do anything on a write. If you want to change that though,
// use the following as an example.
/*static void on_write(ble_evt_t * evt_ptr)
{

    ble_gatts_evt_write_t * evt_write_ptr =
        &evt_ptr->evt.gatts_evt.params.write;

    if ((evt_write_ptr->handle == ble_gps_object.location_char.cccd_handle) &&
        (evt_write_ptr->len == 2)) {
        // Call event handler because cccd written
        if (ble_gps_object->evt_handler != NULL) {
            // Do stuff        
        } 
    }
}*/

void ble_gps_on_ble_evt(ble_evt_t * evt_ptr)
{
    switch (evt_ptr->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(evt_ptr);
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(evt_ptr);
            break;
        //case BLE_GATTS_EVT_WRITE:
        //    on_write(evt_ptr);
        //    break;
    }
}

/**
 * Add a custom characteristic to the GPS service.
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
    
    // Set up the characteristic UUID with the softdevice
    ble_uuid_t char_uuid;
    ble_uuid128_t base_uuid = ble_gps_BASE_UUID;
    char_uuid.uuid = uuid;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    
    // Setup the characteristic value attribute metadata 
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    // Setup the the client characteristic configuration descriptior attribute
    // metadata
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    // Setup the user description characteristic attribute metadata
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
        ble_gps_object.service_handle, &char_md, &attr_char_value,
        char_handles_ptr);

    return err_code;
}

uint32_t ble_gps_init(void)
{
    uint32_t err_code;
    
    // Register the service UUID with the softdevice
    ble_uuid_t service_uuid;
    ble_uuid128_t base_uuid = ble_gps_BASE_UUID;
    service_uuid.uuid = ble_gps_UUID;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);

    // Register the service with the softdevice 
    err_code = sd_ble_gatts_service_add(
        BLE_GATTS_SRVC_TYPE_PRIMARY,
        &service_uuid,
        &(ble_gps_object.service_handle));

    // Register the latitude characteristic with the softdevice 
    err_code = char_add(
        ble_gps_UUID_LATITUDE, LATITUDE_LEN, &latitude_buf,
        strlen(latitude_name), latitude_name, 1, 0, 1,
        &ble_gps_object.latitude_char_handles);

/*
    // Register the longitude characteristic with the softdevice 
    err_code = char_add(
        ble_gps_UUID_LONGITUDE, LONGITUDE_LEN, &longitude_buf,
        strlen(longitude_name), longitude_name, 1, 0, 1,
        &ble_gps_object.longitude_char_handles);

    // Register the speed characteristic with the softdevice 
    err_code = char_add(
        ble_gps_UUID_SPEED, SPEED_LEN, &speed_buf,
        strlen(speed_name), speed_name, 1, 0, 1,
        &ble_gps_object.speed_char_handles);

    // Register the status characteristic with the softdevice 
    err_code = char_add(
        ble_gps_UUID_STATUS, STATUS_LEN, &status_buf,
        strlen(status_name), status_name, 1, 0, 1,
        &ble_gps_object.status_char_handles);
*/

    // FIXME bad err_code will be ignored if not last value
    return err_code;
}
