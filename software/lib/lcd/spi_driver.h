/*
 */

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

/**
 * TODO
 */
extern void spi_init(uint32_t sck_pin, uint32_t miso_pin, uint32_t mosi_pin,
                     uint32_t ss_pin);

/**
 * TODO
 */
extern void spi_write(uint8_t buf);

/**
 * TODO
 */
extern void spi_read(uint8_t* buf);

/**
 * TODO
 */
extern void spi_write_reg(uint8_t reg_addr, uint8_t * data, uint8_t num_bytes);

/**
 * TODO
 */
extern void spi_read_reg(uint8_t reg_addr, uint8_t * data, uint8_t num_bytes);

/**
 * TODO
 */
extern void spi_disable();

/**
 * TODO
 */
extern void spi_enable();

#endif
