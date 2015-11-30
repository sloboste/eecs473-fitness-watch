/* Fuel gauge driver and I2C software bit banging driver. This has to be
 * combined due to the dependency of the fuel gauge functions on the gpio pin
 * configuration.
 */

#ifndef SW_I2C_H
#define SW_I2C_H

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "twi_sw_master.h"

#include "blue_dev_board.h"

#define MAX17043_ADDRESS  0x36
#define MAX17043_SOC      0x04 // R - 16-bit state of charge (SOC)

#define START 1
#define NOSTART 0
#define END 1
#define NOEND 0


static bool started = false;


/**
 * Set SDA as input and return current level of line, 0 or 1.
 */
bool read_SDA()
{
    nrf_gpio_cfg_input(PIN_SW_I2C_SDA, NRF_GPIO_PIN_PULLUP);
    return nrf_gpio_pin_read(PIN_SW_I2C_SDA);
}

/**
 * Actively drive SDA signal high.
 */
void set_SCL()
{
    nrf_gpio_cfg_output(PIN_SW_I2C_SCL);
    nrf_gpio_pin_set(PIN_SW_I2C_SCL);
}

/**
 * Actively drive SCL signal low.
 */
void clear_SCL()
{
    nrf_gpio_cfg_output(PIN_SW_I2C_SCL);
    nrf_gpio_pin_clear(PIN_SW_I2C_SCL);
}

/**
 * Actively drive SDA signal high.
 */
void set_SDA()
{
    nrf_gpio_cfg_output(PIN_SW_I2C_SDA);
    nrf_gpio_pin_set(PIN_SW_I2C_SDA);
}

/**
 * Actively drive SDA signal low.
 */
void clear_SDA()
{
    nrf_gpio_cfg_output(PIN_SW_I2C_SDA);
    nrf_gpio_pin_clear(PIN_SW_I2C_SDA);
}

/**
 * TODO
 */
void i2c_start_cond() 
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
 * TODO
 */
void i2c_stop_cond()
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
void i2c_write_bit(bool bit) 
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
bool i2c_read_bit() 
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
bool i2c_write_byte(bool send_start, bool send_stop, unsigned char byte) 
{
    unsigned bit;
    bool     nack;

    if(send_start) {
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
unsigned char i2c_read_byte(bool nack, bool send_stop) 
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


/**
 * Get the battery level from the fuel gauge with resolution of 1%.
 *
 * Returns the battery level as a uint8_t in the range [0, 100].
 */
uint8_t fuel_get_battery_level()
{
    uint8_t msb;
    uint8_t lsb;

    nrf_gpio_cfg_output(PIN_SW_I2C_SDA);

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

#endif
