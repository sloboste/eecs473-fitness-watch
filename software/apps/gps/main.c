/*
 * GPS app test
 */

// Standard Libraries
#include <stdint.h>
#include <stdbool.h>

// Nordic
#include "nrf_gpio.h"
#include "app_uart.h"
#include "app_util.h"
#include "app_error.h"

// Platform, Peripherals, Devices, Services
#include "blue_dev_board.h"
#include "GPS.h"


int main(void) {
    nrf_gpio_cfg_output(PIN_LED_1);
    nrf_gpio_pin_clear(PIN_LED_1);

    GPS_init();

    GPS_sendMsg(PMTK_STANDBY, 13);
    nrf_delay_ms(4000);
    GPS_sendMsg(PGCMD_ANTENNA, 14);

    while (1) {
        nrf_gpio_pin_toggle(PIN_LED_1);
        nrf_delay_ms(500);
    }
}

