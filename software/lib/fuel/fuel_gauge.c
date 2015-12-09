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

//TODO/FIXME try to separate I2C from fuel driver.

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "twi_sw_master.h"

#include "fuel_gauge.h"

#define MAX17043_ADDRESS            0x36

#define MAX17043_SOC                0x04 // R - 16-bit state of charge (SOC)
#define MAX17043_MODE               0x06 // W - Sends special commands to IC

#define MAX17043_MODE_QUICKSTART    0x4000

#define START 1
#define NOSTART 0
#define END 1
#define NOEND 0


static bool started = false;
static uint32_t pin_sda;
static uint32_t pin_scl;
static uint32_t pin_qst;


/**
 * Set SDA as input and return current level of line, 0 or 1.
 */
static bool read_SDA()
{
    nrf_gpio_cfg_input(pin_sda, NRF_GPIO_PIN_PULLUP);
    return nrf_gpio_pin_read(pin_sda);
}

/**
 * Actively drive SDA signal high.
 */
static void set_SCL()
{
    nrf_gpio_cfg_output(pin_scl);
    nrf_gpio_pin_set(pin_scl);
}

/**
 * Actively drive SCL signal low.
 */
static void clear_SCL()
{
    nrf_gpio_cfg_output(pin_scl);
    nrf_gpio_pin_clear(pin_scl);
}

/**
 * Actively drive SDA signal high.
 */
static void set_SDA()
{
    nrf_gpio_cfg_output(pin_sda);
    nrf_gpio_pin_set(pin_sda);
}

/**
 * Actively drive SDA signal low.
 */
static void clear_SDA()
{
    nrf_gpio_cfg_output(pin_sda);
    nrf_gpio_pin_clear(pin_sda);
}

/**
 * TODO comment
 */
static void i2c_start_cond() 
{
    // SCL is high, set SDA from 1 to 0.
    set_SDA();
    nrf_delay_us(2);
    set_SCL();  
    nrf_delay_us(6);
    clear_SDA();
    nrf_delay_us(2);
    clear_SCL();
    started = true;
    nrf_delay_us(3);
}

/**
 * TODO comment
 */
static void i2c_stop_cond()
{
    // set SDA to 0
    clear_SDA();
    set_SCL();
    nrf_delay_us(1);

    // SCL is high, set SDA from 0 to 1
    set_SDA();

    started = false;
}

/**
 * Write a bit to I2C bus
 */
static void i2c_write_bit(bool bit) 
{
    if (bit) {
        set_SDA();
    } else {
        clear_SDA();
    }

    nrf_delay_us(2); // t SU:DAT

    set_SCL();

    nrf_delay_us(4); // t HIGH

    clear_SCL();

    nrf_delay_us(2); // t LOW
}

/**
 * Read a bit from I2C bus
 */
static bool i2c_read_bit() 
{
    bool bit;
    nrf_delay_us(3);
    bit = read_SDA();
    nrf_delay_us(1);
    set_SCL();
    nrf_delay_us(4);

    // SCL is high, now data is valid
    clear_SCL();
    nrf_delay_us(1);

    return bit;
}

/**
 *  Write a byte to I2C bus. Return 0 if ack by the slave.
 */
static bool i2c_write_byte(bool send_start, bool send_stop, unsigned char byte) 
{
    unsigned bit;
    bool nack;

    if (send_start) {
        i2c_start_cond();
    }

    for (bit = 0; bit < 8; bit++) {
        i2c_write_bit((byte & 0x80) != 0);
        byte <<= 1;
    }

    nack = i2c_read_bit();

    if (send_stop) {
        i2c_stop_cond();
    }

    return nack;
}

/**
 *  Read a byte from I2C bus.
 */
static unsigned char i2c_read_byte(bool nack, bool send_stop) 
{
    unsigned char byte = 0;
    unsigned char bit;

    for (bit = 0; bit < 8; bit++) {
        byte |= i2c_read_bit();
        byte <<= 1;
    }

    i2c_write_bit(nack);

    if (send_stop) {
        i2c_stop_cond();
    }

    return byte;
}

void fuel_init(uint32_t sda, uint32_t scl, uint32_t qst)
{
    pin_sda = sda;
    pin_scl = scl;
    pin_qst = qst;

    nrf_gpio_cfg_output(pin_qst);
    nrf_gpio_pin_clear(pin_qst);
}

uint8_t fuel_get_battery_level()
{
    uint8_t msb;
    uint8_t lsb;

    nrf_gpio_cfg_output(pin_sda);

    // Reads from the state of charge register (battery level). The MSB holds
    // the battery level with a resolution of 1% and the LSB provides additional
    // resolution in units of 1/256%.
    // TODO/FIXME explain transaction, magic number bs
    i2c_write_byte(START, NOEND, 0x6C);
    nrf_delay_us(10);
    i2c_write_byte(NOSTART, NOEND, 0x04);
    nrf_delay_us(10);
    i2c_write_byte(START, NOEND, 0x6D);
    nrf_delay_us(10);
    msb = i2c_read_byte(0, NOEND);
    nrf_delay_us(10);
    lsb = i2c_read_byte(1, END);

    return msb;
}

void fuel_quick_start(bool software)
{
    // A quick-start is initiated by a rising edge on the QSTRT pin, or through
    // software by writing 4000h to MODE register.
    if (software) {
        // TODO

    } else {
        nrf_gpio_pin_set(pin_qst);
        nrf_delay_us(200);
        nrf_gpio_pin_clear(pin_qst);
    } 
}
