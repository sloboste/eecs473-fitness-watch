/* This file is part of eecs473-fitness-watch.
 *   
 * The code / board schematics created by the authors of eecs473-fitness-watch
 * are free software/hardware: you can redistribute them and/or modify them
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * The code / board schematics are distributed in the hope that they will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the code / board schematics.  If not, see <http://www.gnu.org/licenses/>.
 *
 * All code / schematics not created by the authors of this repository fall
 * under their original licenses.
 */

#include <stdbool.h>
#include <stdint.h>

#include "app_error.h"
#include "pstorage.h"
#include "pstorage_platform.h"
#include "nrf_delay.h"

#include "date_time.h"


#define FLASH_BLOCK_SIZE        16 
#define FLASH_NUM_BLOCKS        2


static volatile bool flash_busy = false;
static pstorage_handle_t base_block_id;                                            
static pstorage_handle_t date_block_id;                                            


static pstorage_ntf_cb_t flash_callback(                                              
    pstorage_handle_t * p_handle, uint8_t op_code, uint32_t result,             
    uint8_t * p_data, uint32_t data_len)                                        
{                                                                               
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

void flash_load_step_goal(char * step_goal_str)
{
    uint32_t step_goal;
    load_uint32(&step_goal, 8); 
    sprintf(step_goal_str, "%05u", step_goal);
}

void flash_store_step_goal(char * step_goal_str)
{
    uint32_t step_goal = atoi(step_goal_str);
    store_uint32(&step_goal, 8); 
}

void flash_load_date_time(date_time_t * date_time_ptr)
{
    uint32_t error_code;
    error_code = pstorage_load((uint8_t *) date_time_ptr, &date_block_id,
                               sizeof(*date_time_ptr), 0);          
                               //16, 0);          
    APP_ERROR_CHECK(error_code);
    nrf_delay_ms(50);
}

void flash_store_date_time(date_time_t * date_time_ptr)
{
    uint32_t error_code;
    error_code = pstorage_update(&date_block_id, (uint8_t*) date_time_ptr,
                                 sizeof(*date_time_ptr), 0);
                                 //16, 0);          
    APP_ERROR_CHECK(error_code);                                                
    nrf_delay_ms(50);
}
