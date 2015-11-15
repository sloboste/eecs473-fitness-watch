#define SDA 8
#define SCL 9
#define MAX17043_ADDRESS  0x36
#define MAX17043_SOC      0x04 // R - 16-bit state of charge (SOC)

#define START 1
#define NOSTART 0
#define END 1
#define NOEND 0

#include "nrf_delay.h"

bool read_SCL( void ){
    nrf_gpio_cfg_input(SCL, NRF_GPIO_PIN_NOPULL );
    return nrf_gpio_pin_read(SCL);
}; // Set SCL as input and return current level of line, 0 or 1

bool read_SDA( void ){
    nrf_gpio_cfg_input(SDA, NRF_GPIO_PIN_NOPULL );
    return nrf_gpio_pin_read(SDA);
}; // Set SDA as input and return current level of line, 0 or 1

void set_SCL( void ){
    nrf_gpio_cfg_output(SCL);
    nrf_gpio_pin_set(SCL);
}; // Actively drive SDA signal high

void clear_SCL( void ){
    nrf_gpio_cfg_output(SCL);
    nrf_gpio_pin_clear(SCL);
}; // Actively drive SCL signal low

void set_SDA( void ){
    nrf_gpio_cfg_output(SDA);
    nrf_gpio_pin_set(SDA);
}; // Actively drive SDA signal high

void clear_SDA( void ){
    nrf_gpio_cfg_output(SDA);
    nrf_gpio_pin_clear(SDA);
}; // Actively drive SDA signal low

void arbitration_lost( void );

bool started = false; // global data

void i2c_start_cond( void ) 
{
  // if( started ) 
  // { 
  //   // if started, do a restart cond
  //   // set SDA to 1
  //   read_SDA();
  //   I2C_delay();

  //   while( read_SCL() == 0 ) 
  //   {  // Clock stretching
  //     // You should add timeout to this loop
  //   }

  //   // Repeated start setup time, minimum 4.7us
  //   I2C_delay();

  // }

  // if( read_SDA() == 0 ) 
  // {
  //   arbitration_lost();
  // }

  // SCL is high, set SDA from 1 to 0.
  set_SCL();  
  nrf_delay_us(1);
  clear_SDA();
  nrf_delay_us(1);
  clear_SCL();
  started = true;

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

  nrf_delay_us(1); // t SU:DAT

  set_SCL();

  nrf_delay_us(2); // t HIGH

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
  set_SCL();
  nrf_delay_us(1);
  // while( read_SCL() == 0 ) 
  // { // Clock stretching
  //   // You should add timeout to this loop
  // }

  // SCL is high, now data is valid
  bit = read_SDA();
  nrf_delay_us(1);
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
    byte = ( byte << 1 ) | i2c_read_bit();
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

uint16_t read2byte(){
  uint8_t msb;
  uint8_t lsb;

  i2c_write_byte(START, NOEND, 0x6C);
  i2c_write_byte(NOSTART, NOEND, 0x04);
  i2c_write_byte(START, NOEND, 0x6D);
  i2c_read_byte(0, END);

  i2c_read_byte(1, END);

  return ((uint16_t) msb << 8) | lsb;  
}

float getSOC(){
    uint16_t soc;
    float percent;
    soc = read2byte();
    percent = (soc & 0xFF00) >> 8;
    percent += (float) (((uint8_t) soc) / 256.0);

    return percent;
}


