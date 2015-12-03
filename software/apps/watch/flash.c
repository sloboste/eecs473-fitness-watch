#include <stdbool.h>
#include <stdint.h>

#include "app_error.h"
#include "pstorage.h"
#include "pstorage_platform.h"
#include "nrf_delay.h"

#include "date_time.h"

#include "blue_dev_board.h" // FIXME remove
#include "nrf_gpio.h" // FIXME remove


#define FLASH_BLOCK_SIZE        16 
#define FLASH_NUM_BLOCKS        2


static volatile bool flash_busy = false;
static pstorage_handle_t base_block_id;                                            
static pstorage_handle_t date_block_id;                                            

static pstorage_ntf_cb_t flash_callback(                                              
    pstorage_handle_t * p_handle, uint8_t op_code, uint32_t result,             
    uint8_t * p_data, uint32_t data_len)                                        
{                                                                               
    //nrf_gpio_pin_toggle(PIN_LED_1);
    return 0; // FIXME what is the correct return value?
}          

void flash_init()                                           
{                                                                               
    uint32_t error_code;                                                        

    pstorage_module_param_t module_param;                                       
    module_param.cb = flash_callback;                                                 
    module_param.block_size = FLASH_BLOCK_SIZE;                                               
    module_param.block_count = FLASH_NUM_BLOCKS;                                                

    softdevice_sys_evt_handler_set(pstorage_sys_event_handler);                           
    APP_ERROR_CHECK(error_code);                                                

    pstorage_init();                                                            
    APP_ERROR_CHECK(error_code);                                                

    error_code = pstorage_register(&module_param, &base_block_id);              
    APP_ERROR_CHECK(error_code);                                                

    pstorage_block_identifier_get(&base_block_id, 1, &date_block_id);
}                                 

static void load_uint32(uint32_t * data_ptr, uint8_t block_offset)
{
    uint32_t error_code;
    error_code = pstorage_load((uint8_t *) data_ptr, &base_block_id,
                               sizeof(*data_ptr), block_offset);          
    APP_ERROR_CHECK(error_code);
    nrf_delay_ms(50);
}

static void store_uint32(uint32_t * data_ptr, uint8_t block_offset)
{
    uint32_t error_code;
    error_code = pstorage_update(&base_block_id, (uint8_t*) data_ptr,
                                 sizeof(*data_ptr), block_offset);
    APP_ERROR_CHECK(error_code);                                                
    nrf_delay_ms(50);
}

void flash_load_step_count(uint32_t * step_count_ptr)
{
    load_uint32(step_count_ptr, 0); 
}

void flash_store_step_count(uint32_t * step_count_ptr)
{
    store_uint32(step_count_ptr, 0); 
}

void flash_load_step_yesterday(uint32_t * step_yesterday_ptr)
{
    load_uint32(step_yesterday_ptr, 4); 
}

void flash_store_step_yesterday(uint32_t * step_yesterday_ptr)
{
    store_uint32(step_yesterday_ptr, 4); 
}

void flash_load_step_goal(char * step_goal_ptr)
{
    uint32_t step_goal;
    load_uint32(&step_goal, 8); 
    sprintf(step_goal_ptr, "%05u", step_goal);
}

void flash_store_step_goal(char * step_goal_ptr)
{
    uint32_t step_goal = atoi(step_goal_ptr);
    store_uint32(&step_goal, 8); 
}

void flash_load_date_time(date_time_t * date_time_ptr)
{
    uint32_t error_code;
    error_code = pstorage_load((uint8_t *) date_time_ptr, &date_block_id,
                               sizeof(*date_time_ptr), 0);          
    APP_ERROR_CHECK(error_code);
    nrf_delay_ms(10);
}

void flash_store_date_time(date_time_t * date_time_ptr)
{
    uint32_t error_code;
    error_code = pstorage_update(&date_block_id, (uint8_t*) date_time_ptr,
                                 sizeof(*date_time_ptr), 0);
    APP_ERROR_CHECK(error_code);                                                
    nrf_delay_ms(10);
}
