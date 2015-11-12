/*
 * LED Blink App
 */

// Standard Libraries
#include <stdint.h>
#include <stdbool.h>

// Nordic Libraries
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "app_timer.h"

// Platform, Peripherals, Devices, Services
//#include "blees.h"
#include "led.h"

#define SPI_MASTER_0_ENABLE
#include "spi_master.h"
#include "lcd_driver.h"


/*******************************************************************************
 *   DEFINES
 ******************************************************************************/
#include "nrf_drv_config.h"

#define BLINK_TIMER_PRESCALER       0   // Value of RTC1 PRESCALER register
#define BLINK_TIMER_MAX_TIMERS      4   // Maximum number of simultaneous timers
#define BLINK_TIMER_OP_QUEUE_SIZE   4   // Size of timer operation queues
#define BLINK_RATE  APP_TIMER_TICKS(500, BLINK_TIMER_PRESCALER) // Blink every 0.5 seconds

// NOTE: added this
#define LED_PIN 21 // led 0 on the green board
//#define CLK 15 
//#define DI 14
//#define CS 13

#define APP_IRQ_PRIORITY_LOW 3
//#define SPI_MASTER_0

#define DELAY_MS                 1000                   /**< Timer Delay in milli-seconds. */

/** @def  TX_RX_MSG_LENGTH
 * number of bytes to transmit and receive. This amount of bytes will also be tested to see that
 * the received bytes from slave are the same as the transmitted bytes from the master */
#define TX_RX_MSG_LENGTH         128

//static uint8_t m_tx_data_spi[TX_RX_MSG_LENGTH]; /**< SPI master TX buffer. */
//static uint8_t m_rx_data_spi[TX_RX_MSG_LENGTH];

/*******************************************************************************
 *   STATIC AND GLOBAL VARIABLES
 ******************************************************************************/

static app_timer_id_t test_timer;


/*******************************************************************************
 *   HANDLERS AND CALLBACKS
 ******************************************************************************/

/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    // APPL_LOG("[APPL]: ASSERT: %s, %d, error 0x%08x\r\n", p_file_name, line_num, error_code);
    // nrf_gpio_pin_set(ASSERT_LED_PIN_NO);

    // This call can be used for debug purposes during development of an application.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Un-comment the line below to use.
    // ble_debug_assert_handler(error_code, line_num, p_file_name);

    // On assert, the system can only recover with a reset.
    NVIC_SystemReset();
}

/**@brief Function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num     Line number of the failing ASSERT call.
 * @param[in] p_file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name) {
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}

//service error callback
static void service_error_handler(uint32_t nrf_error) {
    APP_ERROR_HANDLER(nrf_error);
}

/**@brief Function for dispatching a system event to interested modules.
 *
 * @details This function is called from the System event interrupt handler after a system
 *          event has been received.
 *
 * @param[in]   sys_evt   System stack event.
 */
static void sys_evt_dispatch(uint32_t sys_evt) {
    // pstorage_sys_event_handler(sys_evt);
    // on_sys_evt(sys_evt);
}

// Timer fired handler
static void timer_handler (void* p_context) {
    led_toggle(LED_PIN);
}


/*******************************************************************************
 *   INIT FUNCTIONS
 ******************************************************************************/

static void timers_init(void) {
    uint32_t err_code;

    APP_TIMER_INIT(BLINK_TIMER_PRESCALER, BLINK_TIMER_MAX_TIMERS,
            BLINK_TIMER_OP_QUEUE_SIZE, false);

    err_code = app_timer_create(&test_timer, APP_TIMER_MODE_REPEATED,
            timer_handler);
    APP_ERROR_CHECK(err_code);
}


/*******************************************************************************
 *   HELPER FUNCTIONS
 ******************************************************************************/

/** @brief Function for the Power manager.
 */
static void power_manage (void) {
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

// Start the timers
static void timers_start(void) {
    uint32_t err_code = app_timer_start(test_timer, BLINK_RATE, NULL);
    APP_ERROR_CHECK(err_code);
}


/*******************************************************************************
 *   MAIN LOOP
 ******************************************************************************/

int main(void) {
    //uint32_t err_code;

    // Initialization
   //  led_init(LED_PIN);
    led_init(22);
    //led_off(LED_PIN);
    led_off(22);

   //  //spi_master_init();
   //  nrf_delay_ms(500);
   //  led_on(22);
   //  led_on(21);

   //  nrf_delay_ms(500);

   //  led_off(LED_PIN);
   //  led_off(22);

   //  nrf_delay_ms(500);
   //  led_on(22);
   //  led_on(21);

   //  spi_init();

   //  //clearDisplay();


   //   //nrf_gpio_pin_dir_set(15, NRF_GPIO_PIN_DIR_OUTPUT);
   // //  // nrf_gpio_pin_dir_set(DI, NRF_GPIO_PIN_DIR_OUTPUT);
   // //  // nrf_gpio_pin_dir_set(CS, NRF_GPIO_PIN_DIR_OUTPUT);
   //  int SPI_SS_PIN = 24;

   // //  //nrf_gpio_pin_clear(15);
   //  spi_init();
   // // Setup clock
   //  nrf_gpio_pin_set(SPI_SS_PIN);

   //  // spi_write(0x20);
   //  // spi_write(0x00);
   //  //clearDisplay();
   //  nrf_gpio_pin_clear(SPI_SS_PIN);

   //  nrf_delay_ms(1000);

   //  nrf_gpio_pin_set(SPI_SS_PIN);


   //  uint8_t x;
   //  uint8_t addr;
   //  uint8_t y;
   //  //uint8_t pixle;
   //  spi_write(0x80);
   //  for(x = 1; x <= 96; x++)
   //  {
   //      addr = 0;
   //      addr |= (((0x01) & x)<<7);
   //      addr |= (((0x02) & x)<<5);
   //      addr |= (((0x04) & x)<<3);
   //      addr |= (((0x08) & x)<<1);
   //      addr |= (((0x10) & x)>>1);
   //      addr |= (((0x20) & x)>>3);
   //      addr |= (((0x40) & x)>>5);

   //      spi_write(addr);
   //      for(y = 0; y < 96/8; y++)
   //      {
   //          if (y%2 == 0)
   //              spi_write(0x00);
   //          else
   //              spi_write(0x00);
   //      }
   //      spi_write(0x00);
   //  }
   //  spi_write(0x00);
   //  nrf_delay_ms(1000);
   //  nrf_gpio_pin_clear(SPI_SS_PIN);
   //  clearDisplay();

   // // SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_8000MS_CALIBRATION,
   //                          //false);
 
   //  //spi_send_recv(SPI_MASTER_0, m_tx_data_spi, m_rx_data_spi, TX_RX_MSG_LENGTH);
   //  uint8_t test = 0x33;

    // 1
    led_on(22);
    nrf_delay_ms(500);
    led_off(22);
    nrf_delay_ms(500);

    spi_init();

    // 2

    int SPI_SS_PIN = 24;
    nrf_gpio_pin_dir_set(SPI_SS_PIN, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_set(SPI_SS_PIN);
    nrf_gpio_pin_clear(SPI_SS_PIN);


    // 3

    nrf_gpio_pin_clear(SPI_SS_PIN);

    // 4

    clearDisplay();

    // 5


    charTest();

    refresh();

    nrf_delay_ms(2000);

    clearDisplay();

    buildTopBar_LCD();

    refresh();

    led_on(22);
    nrf_delay_ms(500);
    led_off(22);
    nrf_delay_ms(500);

    nrf_delay_ms(2000);


    led_on(22);
    nrf_delay_ms(500);
    led_off(22);
    nrf_delay_ms(500);

    buildRun_LCD();

    refresh();

    nrf_delay_ms(2000);





    // Cursor.row = 0;
    // Cursor.line = 48;
    // transferToBitmap(0x01);
    // Cursor.row++;
    // transferToBitmap(0x02);
    // Cursor.row++;
    // transferToBitmap(0x03);
    // Cursor.row++;
    // transferToBitmap(0x04);
    // Cursor.row++;
    // transferToBitmap(0x05);
    // Cursor.row++;
    // transferToBitmap(0x06);
    // Cursor.row++;
    // transferToBitmap(0x07);
    // Cursor.row++;
    // transferToBitmap(0x08);
    // Cursor.row++;
    // transferToBitmap(0x09);
    // Cursor.row++;
    // transferToBitmap(0x0A);
    // Cursor.row++;
    // transferToBitmap(0x0B);
    // Cursor.row++;
    // transferToBitmap(0x0C);
    // Cursor.row++;

    // 6

    refresh();



    // 7

    // uint8_t x;
    // uint8_t addr;
    // uint8_t y;
    //uint8_t pixle;
    // spi_write(0x80);
    // for(x = 1; x <= 96; x++)
    // {
    //     addr = 0;
    //     addr |= (((0x01) & x)<<7);
    //     addr |= (((0x02) & x)<<5);
    //     addr |= (((0x04) & x)<<3);
    //     addr |= (((0x08) & x)<<1);
    //     addr |= (((0x10) & x)>>1);
    //     addr |= (((0x20) & x)>>3);
    //     addr |= (((0x40) & x)>>5);

    //     spi_write(addr);
    //     for(y = 0; y < 96/8; y++)
    //     {
    //         if (y%2 == 0)
    //             spi_write(0x00);
    //         else
    //             spi_write(0x00);
    //     }
    //     spi_write(0x00);
    // }
    // spi_write(0x00);
    // nrf_delay_ms(1000);
    // nrf_gpio_pin_clear(SPI_SS_PIN);


    led_init(22);
    //led_off(LED_PIN);

    while (1) {
        led_off(22);
        nrf_delay_ms(200);
        led_on(22);
        nrf_delay_ms(200);

        //spi_send_recv(SPI_MASTER_0, m_tx_data_spi, m_rx_data_spi, TX_RX_MSG_LENGTH);
    }
}
