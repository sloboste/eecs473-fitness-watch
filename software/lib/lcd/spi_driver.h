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
