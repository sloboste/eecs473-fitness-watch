/**
 * A standard interface for UART. This removes platform dependency so that our
 * UART dependent code can be easily/rapidly prototyped on other platforms with
 * minimal changes.
 */

#ifndef __UART_ADAPTER_H
#define __UART_ADAPTER_H

#include <stdint.h>


/**
 * Initialize the UART module. 
 */
extern void uart_adapter_init(uint32_t pin_rxd, uint32_t pin_txd,
                              uint32_t pin_rts, uint32_t pin_cts);

/**
 * Write exactly len bytes from buf to UART.
 * NOTE: this is a blocking call.
 */
extern void uart_adapter_write(char * buf, uint32_t len);

/**
 * Read exactly len bytes from UART and put them into buf.
 * NOTE: this is a blocking call.
 */
extern void uart_adapter_read(char * buf, uint32_t len);

/**
 * Stop the UART module.
 * NOTE: must call uart_adapter_init before using again.
 */
extern void uart_adapter_stop();

#endif
