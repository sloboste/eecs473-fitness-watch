#include "nordic_common.h"
#include "softdevice_handler.h"
#include "led.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "SEGGER_RTT.h"
#include "twi_master.h"
#include "mpu.h"
#include "nrf_drv_gpiote.h"

#define LED_PIN 18 // led 0 on the green board
#define PIN_IN 17
#define PIN_OUT 19

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    NVIC_SystemReset();
}

static void power_manage (void) { 
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    led_toggle(LED_PIN);
    nrf_drv_gpiote_out_toggle(PIN_OUT);
}

int main(void) {
    led_init(LED_PIN);
    led_on(LED_PIN);

    nrf_drv_gpiote_init();

	nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
	nrf_drv_gpiote_in_init(PIN_IN, &in_config, in_pin_handler);

	nrf_drv_gpiote_in_event_enable(PIN_IN, true);
    // nrf_gpio_cfg_sense_input(16, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_LOW);
    // NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Msk;
    // NVIC_EnableIRQ(GPIOTE_IRQn);

    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
    SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n\r\n");

	mympu_open(200); //run this once

    while (1) {
    	SEGGER_RTT_printf(0, "Steps: %d\r\n", get_steps()); //schedule this
    	//mympu_update();
    	//SEGGER_RTT_printf(0, ":%d\r\n", mympu.steps);
        //led_toggle(LED_PIN);
        //nrf_delay_ms(3000); //wait 3 seconds
        //SEGGER_RTT_printf(0, "Steps: %d\r\n", steps);
        //SEGGER_RTT_printf(0, "Steps: %d\r\n", get_steps());
     }
}