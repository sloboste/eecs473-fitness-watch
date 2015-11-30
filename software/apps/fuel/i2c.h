#define SDA 1
#define SCL 2
#define MAX17043_ADDRESS  0x36
#define MAX17043_SOC      0x04 // R - 16-bit state of charge (SOC)

#define START 1
#define NOSTART 0
#define END 1
#define NOEND 0

#include "nrf_delay.h"
#include "twi_sw_master.h"

#include "nrf_gpio.h"

bool read_SDA( void ){
    nrf_gpio_cfg_input(SDA, NRF_GPIO_PIN_PULLUP);
    return nrf_gpio_pin_read(SDA);
} // Set SDA as input and return current level of line, 0 or 1

void set_SCL( void ){
    nrf_gpio_cfg_output(SCL);
    nrf_gpio_pin_set(SCL);
} // Actively drive SDA signal high

void clear_SCL( void ){
    nrf_gpio_cfg_output(SCL);
    nrf_gpio_pin_clear(SCL);
} // Actively drive SCL signal low

void set_SDA( void ){
    nrf_gpio_cfg_output(SDA);
    nrf_gpio_pin_set(SDA);
} // Actively drive SDA signal high

void clear_SDA( void ){
    nrf_gpio_cfg_output(SDA);
    nrf_gpio_pin_clear(SDA);
} // Actively drive SDA signal low

void arbitration_lost( void );

bool started = false; // global data

void i2c_start_cond( void ) 
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

void i2c_stop_cond( void )
{
  // set SDA to 0
  clear_SDA();
  set_SCL();
  nrf_delay_us(1);

  // SCL is high, set SDA from 0 to 1
  set_SDA();


  started = false;

}

// Write a bit to I2C bus
void i2c_write_bit( bool bit ) 
{
  if( bit ) 
  {
    set_SDA();
  } 
  else 
  {
    clear_SDA();
  }

  nrf_delay_us(2); // t SU:DAT

  set_SCL();

  nrf_delay_us(4); // t HIGH

  // while( read_SCL() == 0 ) 
  // { // Clock stretching
  //   // You should add timeout to this loop
  // }

  clear_SCL();

  nrf_delay_us(2); // t LOW

}

// Read a bit from I2C bus
bool i2c_read_bit( void ) 
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

// Write a byte to I2C bus. Return 0 if ack by the slave.
bool i2c_write_byte( bool          send_start ,
                     bool          send_stop  ,
                     unsigned char byte         ) 
{
  unsigned bit;
  bool     nack;

  if( send_start ) 
  {
    i2c_start_cond();
  }

  for( bit = 0; bit < 8; bit++ ) 
  {
    i2c_write_bit( ( byte & 0x80 ) != 0 );
    byte <<= 1;
  }
  // i2c_write_bit( 0 );

  nack = i2c_read_bit();

  if (send_stop) 
  {
    i2c_stop_cond();
  }

  return nack;

}

// Read a byte from I2C bus
unsigned char i2c_read_byte( bool nack , bool send_stop ) 
{
  unsigned char byte = 0;
  unsigned char bit;

  for( bit = 0; bit < 8; bit++ ) 
  {
    byte |= i2c_read_bit();
    byte <<= 1;
  }

  i2c_write_bit( nack );

  if( send_stop ) 
  {
    i2c_stop_cond();
  }

  return byte;

}

// uint16_t read16(uint8_t address)
// {
// uint8_t msb, lsb;
// int16_t timeout = 1000;

// Wire.beginTransmission(MAX17043_ADDRESS);
// Wire.write(address);
// Wire.endTransmission(false);

// Wire.requestFrom(MAX17043_ADDRESS, 2);
// while ((Wire.available() < 2) && (timeout-- > 0))
//   delay(1);
// msb = Wire.read();
// lsb = Wire.read();

// return ((uint16_t) msb << 8) | lsb;
// }

// bool i2c_write_sw(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t const *data){
//   slave_addr = slave_addr << 1;
//   int tx_size = length+1;
//   uint8_t w2_data[tx_size];
//   w2_data[0] = reg_addr;
//   for(int i = 1; i < tx_size; ++i){
//       w2_data[i] = *(data+i-1);
//   }
  
//   return !twi_master_sw_transfer(slave_addr, w2_data, sizeof(w2_data), TWI_ISSUE_STOP);;
// }


// bool i2c_read_sw(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data){
//   slave_addr = slave_addr << 1;
//   bool transfer_succeeded;

//   transfer_succeeded  = twi_master_sw_transfer(slave_addr, &reg_addr, 1, TWI_DONT_ISSUE_STOP);

//   transfer_succeeded &= twi_master_sw_transfer(slave_addr|TWI_READ_BIT, data, length, TWI_ISSUE_STOP);

//   return !transfer_succeeded;
// }


/*
uint16_t read2byte(){
  uint8_t data[2];
  uint8_t msb, lsb;
  nrf_gpio_cfg_output(SDA);
  // i2c_write_sw(MAX17043_ADDRESS, MAX17043_SOC, 2, data);
  //i2c_read_sw(MAX17043_ADDRESS, MAX17043_SOC, 2, data);
  i2c_write_byte(START, NOEND, 0x6C);
  nrf_delay_us(10);
  i2c_write_byte(NOSTART, NOEND, 0x04);
  nrf_delay_us(10);
  i2c_write_byte(START, NOEND, 0x6D);
  nrf_delay_us(10);
  msb = i2c_read_byte(0, NOEND);
  nrf_delay_us(10);
  lsb = i2c_read_byte(1, END);
  //return ((uint16_t) data[0] << 8) | data[1];  

  return ((uint16_t) msb << 8) | lsb;  
}
*/



/**
 * Initialize the battery fuel gauge.
 *
void fuel_gauge_init()
{

}
*/

/**
 * Get the battery level from the fuel gauge with resolution of 1%.
 *
 * Returns the battery level as a uint8_t in the range [0, 100].
 */
uint8_t fuel_get_battery_level()
{
    uint8_t msb;
    uint8_t lsb;

    nrf_gpio_cfg_output(SDA);

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
