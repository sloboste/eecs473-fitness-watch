
#include <stdint.h>
#include <stdbool.h>

#include "nordic_common.h"
#include "softdevice_handler.h"

#include "i2c.h"
#include "nrf_drv_config.h"

#include "nrf_gpio.h"

#include "spi_driver.h"
#include "lcd_driver.h"

#include "blue_dev_board.h"

#include "nrf_delay.h"



void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    NVIC_SystemReset();
}

 void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}

int main(void) {
    uint8_t battery_percent = 120;

    nrf_gpio_cfg_output(PIN_LED_1);
    nrf_gpio_cfg_output(PIN_LED_2);
    nrf_gpio_pin_set(PIN_LED_1);
    nrf_gpio_pin_set(PIN_LED_2);

    spi_init();
    lcd_initCursor();
    lcd_clearDisplay();

    twi_master_sw_init();

    while(1) {
        nrf_gpio_pin_toggle(PIN_LED_1);
        lcd_clearDisplay();
        battery_percent = fuel_get_battery_level();
        lcd_setCursor(1, 37);
        lcd_transferBigNumInt(battery_percent);
        lcd_refresh();
        nrf_delay_ms(1000);
    }
}
