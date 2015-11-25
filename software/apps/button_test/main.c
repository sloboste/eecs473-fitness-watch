/* Test app for the buttons on the watch. TODO
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "app_error.h"
#include "nrf.h"
#include "softdevice_handler.h"

#include "nrf_gpio.h"
#include "app_gpiote.h"
#include "nrf_delay.h"

#include "blue_dev_board.h"
//#include "pcb.h"


void board_led_init(void)
{
    nrf_gpio_cfg_output(PIN_LED_1);
    nrf_gpio_cfg_output(PIN_LED_2);
    nrf_gpio_cfg_output(PIN_LED_3);

    nrf_gpio_pin_set(PIN_LED_1);
    nrf_gpio_pin_set(PIN_LED_2);
    nrf_gpio_pin_set(PIN_LED_3);
}

void app_error_handler(uint32_t error_code, uint32_t line_num,
                       const uint8_t * p_file_name)
{
    // On assert, the system can only recover on reset.
    NVIC_SystemReset();
}

/**
 * Button press/release interrupt handler.
 * 
 * event_pins_low_to_high -- bitmask for GPIO pins that experienced a low to
 *                           high transition.
 * event_pins_high_to_low -- bitmask for GPIO pins that experienced a high to
 *                           low transition.
 *
 * Note: this fuction executes in the interrupt context so schedule any
 *       function calls in response to the button.
 */
static void button_handler(uint32_t event_pins_low_to_high,
                           uint32_t event_pins_high_to_low)
{
    uint32_t error_code;
    if ((event_pins_high_to_low >> PIN_BUTTON_1) & 0x1) {
        // Cycle
        nrf_gpio_pin_clear(PIN_LED_1);
        nrf_delay_ms(100);
        nrf_gpio_pin_set(PIN_LED_1);

    } else if ((event_pins_high_to_low >> PIN_BUTTON_2) & 0x1) {
        // Select
        nrf_gpio_pin_clear(PIN_LED_2);
        nrf_delay_ms(100);
        nrf_gpio_pin_set(PIN_LED_2);

    } else if ((event_pins_high_to_low >> PIN_BUTTON_3) & 0x1) {
        // Wakeup/Sleep
        nrf_gpio_pin_clear(PIN_LED_3);
        nrf_delay_ms(100);
        nrf_gpio_pin_set(PIN_LED_3);

    }
}

/**
 * Initialize the button GPIO pins to generate interrupts on push.
 */
static void buttons_init()
{
    uint32_t error_code;
    nrf_gpio_cfg_sense_input(
        PIN_BUTTON_1, BUTTON_PULL, NRF_GPIO_PIN_SENSE_LOW);
    nrf_gpio_cfg_sense_input(
        PIN_BUTTON_2, BUTTON_PULL, NRF_GPIO_PIN_SENSE_LOW);
    nrf_gpio_cfg_sense_input(
        PIN_BUTTON_3, BUTTON_PULL, NRF_GPIO_PIN_SENSE_LOW);
    uint32_t low_to_high_bitmask = 0x00000000;
    uint32_t high_to_low_bitmask = (1 << PIN_BUTTON_1) +
                                   (1 << PIN_BUTTON_2) +
                                   (1 << PIN_BUTTON_3);
    APP_GPIOTE_INIT(1);
    static app_gpiote_user_id_t gpiote_user_id;
    error_code = app_gpiote_user_register(
        &gpiote_user_id, low_to_high_bitmask, high_to_low_bitmask,
        button_handler);
    APP_ERROR_CHECK(error_code);
    error_code = app_gpiote_user_enable(gpiote_user_id);
    APP_ERROR_CHECK(error_code);
}

int main(void)
{
    board_led_init();
    buttons_init();

    while (1) {
        __WFI(); 
    }
}
