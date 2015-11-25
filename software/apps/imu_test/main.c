/* Test app for the mpu step counter. This will get the step count and display
 * it to the screen. 
 * 
 * MAKE SURE THE PIN ASSIGNMENTS ARE CORRECT IN 
 * software/nrf5x-base/sdk/nrf51_sdk_9.9.0/components/drivers_nrf/twi_master/incubated/config/twi_master_config.h
 * AS WELL AS IN
 * software/lib/lcd/spi_driver.h
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "app_error.h"
#include "nrf.h"
#include "softdevice_handler.h"

#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "mpu.h"

#include "spi_driver.h"
#include "lcd_driver.h"

//#include "blue_dev_board.h"
#include "pcb.h"


void app_error_handler(uint32_t error_code, uint32_t line_num,
                       const uint8_t * p_file_name)
{
    // On assert, the system can only recover on reset.
    NVIC_SystemReset();
}

int main(void)
{
    uint32_t step_count = 0;
    
    nrf_gpio_cfg_output(PIN_LED_1);
    nrf_gpio_pin_set(PIN_LED_1);

    mympu_open(200);

    spi_init();
    lcd_initCursor();

    while (1) {
        step_count = get_steps(); 
        lcd_clearDisplay();
        lcd_setCursor(1, 37);
        lcd_transferBigNumInt(step_count); 
        lcd_refresh();
        nrf_delay_ms(1000);
    }
}
