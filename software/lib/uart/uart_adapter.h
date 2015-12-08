/* This file is part of eecs473-fitness-watch.
 *   
 * The code / board schematics created by the authors of eecs473-fitness-watch
 * are free software/hardware: you can redistribute them and/or modify them
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * The code / board schematics are distributed in the hope that they will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the code / board schematics.  If not, see <http://www.gnu.org/licenses/>.
 *
 * All code / schematics not created by the authors of this repository fall
 * under their original licenses.
 *
 *
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
