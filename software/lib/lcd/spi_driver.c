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

#include <stdint.h>

#include "nrf_gpio.h"
#include "spi_driver.h"
#include "nrf.h"

#define NRF_SPI NRF_SPI0

#define WRITE_REG 0x0A
#define READ_REG 0x0B


static uint32_t slave_select;

void spi_init(uint32_t sck_pin, uint32_t miso_pin, uint32_t mosi_pin,
              uint32_t ss_pin)
{
    // initialize spi
    slave_select = ss_pin;
    nrf_gpio_cfg_output(slave_select);
    nrf_gpio_pin_clear(slave_select);
    NRF_SPI->PSELSCK = sck_pin;
    NRF_SPI->PSELMOSI = mosi_pin;
    NRF_SPI->PSELMISO = miso_pin;
    NRF_SPI->FREQUENCY = SPI_FREQUENCY_FREQUENCY_K500;
    NRF_SPI->CONFIG = (uint32_t)(SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos) |
                      (SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos) |
                      (SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos);
    NRF_SPI->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);
    NRF_SPI->EVENTS_READY = 0;

}

void spi_clear_slave_select()
{
    nrf_gpio_pin_clear(slave_select);
}

void spi_set_slave_select()
{
    nrf_gpio_pin_set(slave_select);
}

void spi_write(uint8_t buf)
{
    // Clear the ready event

    NRF_SPI->EVENTS_READY = 0;

    NRF_SPI->TXD = buf;

    // Wait until byte has transmitted
    while (NRF_SPI->EVENTS_READY == 0);

    uint8_t throw = NRF_SPI->RXD;

    NRF_SPI->EVENTS_READY = 0;
}

void spi_read(uint8_t* buf)
{
    // Clear ready event
    NRF_SPI->EVENTS_READY = 0;

    NRF_SPI->TXD = 0x00;

    // Wait until byte has been received
    while(NRF_SPI->EVENTS_READY == 0);

    buf[0] = NRF_SPI->RXD;

    NRF_SPI->EVENTS_READY = 0;
}

void spi_write_reg(uint8_t reg_addr, uint8_t * data, uint8_t num_bytes)
{
    nrf_gpio_pin_clear(slave_select);
    spi_write(WRITE_REG);
    spi_write(reg_addr);

    // FIXME use for loop
    int i = 0;
    do {
        spi_write(data[i]);
        i++;
    } while (i < num_bytes);

    nrf_gpio_pin_set(slave_select);
}

void spi_read_reg(uint8_t reg_addr, uint8_t * data, uint8_t num_bytes)
{
    nrf_gpio_pin_clear(slave_select);
    spi_write(READ_REG);
    spi_write(reg_addr);

    // FIXME use for loop
    int i = 0;
    do {
        spi_read(data + i);
        i++;
    } while (i < num_bytes);

    nrf_gpio_pin_set(slave_select);
}


void spi_disable()
{
    NRF_SPI->ENABLE = (SPI_ENABLE_ENABLE_Disabled << SPI_ENABLE_ENABLE_Pos);
}

void spi_enable()
{
    NRF_SPI->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);
}
