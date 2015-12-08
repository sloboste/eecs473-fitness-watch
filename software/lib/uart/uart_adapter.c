#include <stdbool.h>
#include <stdint.h>

#include "app_uart.h"
#include "app_error.h"
#include "nrf51_bitfields.h" // baudrate macro

#include "uart_adapter.h"

#define UART_TX_BUF_SIZE 128 
#define UART_RX_BUF_SIZE 2048
//4096


/**
 * Handle a UART event.
 * NOTE: This doesn't have to do anything for our use case.
 */
static void uart_event_handler(app_uart_evt_t * p_event)
{
    /*
    switch (p_event->evt_type) {
    case APP_UART_TX_EMPTY:
        break;
    case APP_UART_DATA_READY:
        break;
    case APP_UART_COMMUNICATION_ERROR:
        break;
    case APP_UART_FIFO_ERROR:
        break;
    } 
    */
}

void uart_adapter_init(uint32_t pin_rxd, uint32_t pin_txd, uint32_t pin_rts,
                       uint32_t pin_cts)
{
	uint32_t err_code;

    // Set up the UART connetion parameters.
	const app_uart_comm_params_t comm_params = {
        pin_rxd,
        pin_txd,
        pin_rts,
        pin_cts,
        APP_UART_FLOW_CONTROL_DISABLED,
        false,
        UART_BAUDRATE_BAUDRATE_Baud9600
    };
			
    // Initialzie the UART module with a FIFO buffer.
    APP_UART_FIFO_INIT(
        &comm_params, UART_RX_BUF_SIZE, UART_TX_BUF_SIZE, uart_event_handler,
        APP_IRQ_PRIORITY_LOW, err_code);
			
    APP_ERROR_CHECK(err_code);
}

void uart_adapter_write(char * buf, uint32_t len)
{
    int i;
    uint32_t err_code;
    for (i = 0; i < len; ++i) {
		do {
            err_code = app_uart_put(*((uint8_t *) buf));
        } while (err_code != NRF_SUCCESS);
		++buf;
	}
}

void uart_adapter_read(char * buf, uint32_t len)
{
    int i;
    uint32_t err_code;
    for (i = 0; i < len; ++i) {
		do {
            err_code = app_uart_get((uint8_t *) buf);
        } while (err_code != NRF_SUCCESS);
		++buf;
	}
}

void uart_adapter_stop()
{
    // TODO finish writing this!!!!!! 
}
