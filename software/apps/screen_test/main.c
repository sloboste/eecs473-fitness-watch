/* Test the LCD.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "app_error.h"
#include "nrf.h"
#include "softdevice_handler.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "spi_driver.h"
#include "lcd_driver.h"

#include "boards.h"

void app_error_handler(uint32_t error_code, uint32_t line_num,
                       const uint8_t * p_file_name)
{
    // On assert, the system can only recover on reset.
    NVIC_SystemReset();
}

int main(void)
{
    nrf_gpio_cfg_output(PIN_LED_1);
    nrf_gpio_pin_set(PIN_LED_1);

    spi_init(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_SS_PIN);
    lcd_clearDisplay();
    lcd_initCursor();

    uint32_t i = 0;

    while (1) {
        nrf_gpio_pin_toggle(PIN_LED_1);

        lcd_clearDisplay();
        lcd_setCursor(37, 1);
        lcd_transferBigNumInt(i++); 
        lcd_refresh();

        nrf_delay_ms(1000);
    }
}
