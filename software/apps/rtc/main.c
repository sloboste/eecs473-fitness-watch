/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 * @brief RTC example.
 * @defgroup rtc_example Real-time clock example
 * @{
 * @ingroup nrf_examples_nrf6310
 *
 * @brief RTC module example
 *
 * This example enables the RTC with a TICK frequency of 8Hz. It also configures
 * and enables the TICK- and the COMPARE0-interrupts. The compare interrupt
 * handler will be triggered 3 seconds after the RTC starts. The TICK interrupt
 * handler clears the interrupt request and toggles PIN8. The COMPARE0 handler
 * clears the interrupt request an set PIN9 to 1.
 * @image html example_board_setup_a.png "Use board setup A for this example."
 */

#include <stdbool.h>
#include <stddef.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "time_keeper.h"

#define GPIO_TOGGLE_TICK_EVENT    (18)                       /*!< Pin number to toggle when there is a tick event in RTC */
#define GPIO_TOGGLE_COMPARE_EVENT (19)                       /*!< Pin number to toggle when there is compare event in RTC */
#define LFCLK_FREQUENCY           (32768UL)                 /*!< LFCLK frequency in Hertz, constant */
#define RTC_FREQUENCY             (800UL)                     /*!< required RTC working clock RTC_FREQUENCY Hertz. Changable */
#define COMPARE_COUNTERTIME       (1UL)                     /*!< Get Compare event COMPARE_TIME seconds after the counter starts from 0 */

#define COUNTER_PRESCALER         ((LFCLK_FREQUENCY/RTC_FREQUENCY) - 1)  /* f = LFCLK/(prescaler + 1) */

const uint32_t time = 0;

//error handler needed
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    NVIC_SystemReset();
}

/** Starts the internal LFCLK XTAL oscillator
 */
static void lfclk_config(void)
{
  NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_LFCLKSTART = 1;
  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
  {
  }
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
}


/** Configures PIN8 and PIN9 as outputs
 */
static void gpio_config(void)
{
  nrf_gpio_cfg_output(GPIO_TOGGLE_TICK_EVENT);
  nrf_gpio_cfg_output(GPIO_TOGGLE_COMPARE_EVENT);

  nrf_gpio_pin_write(GPIO_TOGGLE_TICK_EVENT, 0);
  nrf_gpio_pin_write(GPIO_TOGGLE_COMPARE_EVENT, 0);
}

/** RTC0 interrupt handler.
 * Triggered on TICK and COMPARE0 match.
 */

/**
 * main function
 * \return 0. int return type required by ANSI/ISO standard.
 */
int main(void)
{
  gpio_config();
  lfclk_config();
  rtc_config();
  NRF_RTC0->TASKS_START = 1;
  while (true)
  {
  }
}

/**
 *@}
 **/