/*
 */

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include "blue_dev_board.h"
//#include "pcb.h"


//Initializes and enables spi
void spi_init();

void spi_write(uint8_t buf);

void spi_read(uint8_t* buf);

void spi_write_reg(uint8_t reg_addr, uint8_t * data, uint8_t num_bytes);

void spi_read_reg(uint8_t reg_addr, uint8_t * data, uint8_t num_bytes);

void spi_disable();

void spi_enable();

#endif
