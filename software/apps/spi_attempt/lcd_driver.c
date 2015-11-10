/*********************************************************************
This is an Arduino library for our Monochrome SHARP Memory Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1393

These displays use SPI to communicate, 3 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "nrf_gpio.h"
//#include "nrf_drv_config.h"

#include "lcd_driver.h"
#include "spi_driver.h"
#include "charData.h"

#include "led.h"

/**************************************************************************
    Sharp Memory Display Connector
    -----------------------------------------------------------------------
    Pin   Function        Notes
    ===   ==============  ===============================
      1   VIN             3.3-5.0V (into LDO supply)
      2   3V3             3.3V out
      3   GND
      4   SCLK            Serial Clock
      5   MOSI            Serial Data Input
      6   CS              Serial Chip Select
      9   EXTMODE         COM Inversion Select (Low = SW clock/serial)
      7   EXTCOMIN        External COM Inversion Signal
      8   DISP            Display On(High)/Off(Low)

 **************************************************************************/

#define DATA_WRITE				(0x80)
//#define SHARPMEM_BIT_VCOM       (0x40)
#define LCD_CLEAR      		(0x20)
//#define TOGGLE_VCOM             do { _sharpmem_vcom = _sharpmem_vcom ? 0x00 : SHARPMEM_BIT_VCOM; } while(0);
uint8_t bitmap[(LCD_WIDTH * LCD_HEIGHT)/8] = { 0 };

void initCursor(){
  //CURSOR Cursor:
  Cursor.line = 0;
  Cursor.row = 0;
}

void initStructs(){
  // GPS
  GPS_DATA.longitude = 0;
  GPS_DATA.latitude = 0;
  GPS_DATA.altitude = 0;
  GPS_DATA.velocity =0;

  // RUN
  RUN_DATA.dist = 0;
  RUN_DATA.pace = 0;
  RUN_DATA.timer = 0;

  // TIMER
  TIMER_DATA.lapTimes[0] = 0;
  TIMER_DATA.lapTimes[1] = 0;
  TIMER_DATA.lapTimes[2] = 0;
  TIMER_DATA.lapTimes[3] = 0;
  TIMER_DATA.timer = 0;
  TIMER_DATA.numLaps = 0;
}

  void buildGPS_LCD(){

  }

void buildTimer_LCD();

void buildRun_LCD(){
  clearLines(13,96);
  setCursor(0, 14);
  transferChar('t', 9);
  transferChar('i', 9);
  transferChar('m', 9);
  transferChar('e', 9);

  setCursor(1, 28);
  transferBigNumInt(1,19);
  transferBigNumInt(2,19);
  transferSpecialLargeChar(':');
  transferBigNumInt(3,19);
  transferBigNumInt(4,19);

  drawDottedLine(68);

  setCursor(0, 70);
  transferChar('d', 9);
  transferChar('i', 9);
  transferChar('s', 9);
  transferChar('t', 9);
  transferSpecialChar(':',9);
  Cursor.row++;
  transferSmallNumInt(1,9);
  transferSpecialChar('.',9);
  transferSmallNumInt(2,9);
  transferSmallNumInt(3,9);

  drawDottedLine(82);

  setCursor(0, 84);
  transferChar('p', 9);
  transferChar('a', 9);
  transferChar('c', 9);
  transferChar('e', 9);
  transferSpecialChar(':',9);
  Cursor.row++;
  transferSmallNumInt(4,9);
  transferSpecialChar(':',9);
  transferSmallNumInt(5,9);
  transferSmallNumInt(6,9);
  }

// void writeStringToBitmap(char &s , int length){
//   int i;
//   for int(i = 0; i < length; i++)
//   {
//     transferChar(s[i]);
//   }
// }

void transferChar(char c, uint8_t size){
  int i;
  for (i = 0; i < size; i++){
    transferToBitmap(chars[9*((int)c - 97)+i]);
    Cursor.line++;
  }
  Cursor.line -= size;
  Cursor.row++;
  if (Cursor.row >= 12)
  {
    Cursor.row = 0;
    Cursor.line = Cursor.line + size;
    if (Cursor.line >= 97)
    {
      Cursor.line = 1;
    }
  }
}

void transferSpecialChar(char c, uint8_t size){
  int i;
  int offset;
  if (c == ':')
  {
    offset = 26;
  }
  if (c == '.')
  {
    offset = 27;
  }
  if (c == '/')
  {
    offset = 28;
  }
  for (i = 0; i < size; i++){
    transferToBitmap(chars[9*offset+i]);
    Cursor.line++;
  }
  Cursor.line -= size;
  Cursor.row++;
  if (Cursor.row >= 12)
  {
    Cursor.row = 0;
    Cursor.line = Cursor.line + size;
    if (Cursor.line >= 97)
    {
      Cursor.line = 1;
    }
  }
}

void transferSpecialLargeChar(char c){
  uint8_t i;
  uint8_t offset;
  uint8_t size;
  size = 19;
  if (c == ':')
  {
    offset = 10;
  }
  for (i = 0; i < size; i++){
    transferToBitmap(bignums[19*2*offset+i]);
    Cursor.line++;
  }
  Cursor.line -= size;
  Cursor.row++;
  if (Cursor.row >= 12)
  {
    Cursor.row = 0;
    Cursor.line = Cursor.line + size;
    if (Cursor.line >= 97)
    {
      Cursor.line = 1;
    }
  }
}

void transferSmallNumInt(int num, uint8_t size){
  int divisor;
  int numCopy;
  uint8_t realNum;
  uint8_t i;
  uint8_t j;
  uint8_t numLength;
  numCopy = num;
  divisor = 1;
  for (numLength = 0; numCopy != 0 || numLength == 0;){
    numCopy = numCopy / 10;
    numLength++;
  }
  for (i = 0; i < numLength-1; i++){
    divisor = divisor * 10;
  }
  for (i = 0; i < numLength; i++){
    realNum = num/divisor;
    for (j = 0; j < size; j++)
    {
      transferToBitmap(smallnums[9*(realNum)+j]);
      Cursor.line++;
    }
    Cursor.line -= size;
    num = num % divisor;
    divisor = divisor / 10;
    Cursor.row++;
    if (Cursor.row >= 12)
    {
      Cursor.row = 0;
      Cursor.line = Cursor.line + size;
      if (Cursor.line >= 97)
      {
        Cursor.line = 1;
      }
    }
  }
}

void transferBigNumInt(int num, uint8_t size){
  int divisor;
  int numCopy;
  uint8_t realNum;
  uint8_t i;
  uint8_t j;
  uint8_t numLength;
  numCopy = num;
  divisor = 1;
  for (numLength = 0; numCopy != 0 || numLength == 0;){
    numCopy = numCopy / 10;
    numLength++;
  }
  for (i = 0; i < numLength-1; i++){
    divisor = divisor * 10;
  }
  for (i = 0; i < numLength; i++){
    realNum = num/divisor;
    for (j = 0; j < size; j++)
    {
      transferToBitmap(bignums[19*2*(realNum)+(j*2)]);
      Cursor.row++;
      transferToBitmap(bignums[19*2*(realNum)+(j*2)+1]);
      Cursor.line++;
      Cursor.row--;
    }
    Cursor.line -= size;
    num = num % divisor;
    divisor = divisor / 10;
    Cursor.row += 2;
    if (Cursor.row >= 12)
    {
      Cursor.row = 0;
      Cursor.line = Cursor.line + size;
      if (Cursor.line >= 97)
      {
        Cursor.line = 1;
      }
    }
  }
}

void transferBatteryLevel(int num){
  int divisor;
  int numCopy;
  uint8_t realNum;
  uint8_t i;
  uint8_t j;
  uint8_t numLength;
  uint8_t size;
  size = 9;
  numCopy = num;
  divisor = 1;
  for (numLength = 0; numCopy != 0 || numLength == 0;){
    numCopy = numCopy / 10;
    numLength++;
  }
  for (i = 0; i < numLength-1; i++){
    divisor = divisor * 10;
  }
  for (i = 0; i < numLength; i++){
    realNum = num/divisor;
    for (j = 0; j < size; j++)
    {
      transferToBitmap(battery[9*2*(realNum)+(j*2)]);
      Cursor.row++;
      transferToBitmap(battery[9*2*(realNum)+(j*2)+1]);
      Cursor.line++;
      Cursor.row--;
    }
    Cursor.line -= size;
    num = num % divisor;
    divisor = divisor / 10;
    Cursor.row += 2;
    if (Cursor.row >= 12)
    {
      Cursor.row = 0;
      Cursor.line = Cursor.line + size;
      if (Cursor.line >= 97)
      {
        Cursor.line = 1;
      }
    }
  }
}

// BOTH THESE DOUBLE FUNCTION WILL NOT OUTPUT A POINT YET //

void transferSmallNumDouble(double num, uint8_t numSize, uint8_t size){
  double divisor;
  double numCopy;
  uint8_t realNum;
  uint8_t i;
  uint8_t j;
  uint8_t numLength;
  numCopy = num;
  divisor = 1;
  for (i = 0; i < numSize; i++){
    num = num * 10;
  }
  for (numLength = 0; numCopy != 0;){
    numCopy = numCopy / 10;
    numLength++;
  }
  for (i = 0; i < numLength-1; i++){
    divisor = divisor * 10;
  }
  for (i = 0; i < numLength || numLength == 0; i++){
    realNum = num/divisor;
    for (j = 0; j < size; j++)
    {
      transferToBitmap(smallnums[9*(realNum)+i]);
      Cursor.line++;
    }
    Cursor.line -= size;
    //num = num % divisor;
    divisor = divisor / 10;
    Cursor.row++;
    if (Cursor.row >= 12)
    {
      Cursor.row = 0;
      Cursor.line = Cursor.line + size;
      if (Cursor.line >= 97)
      {
        Cursor.line = 1;
      }
    }
  }
}

void transferBigNumDouble(double num, uint8_t numSize, uint8_t size){
  double divisor;
  double numCopy;
  uint8_t realNum;
  uint8_t i;
  uint8_t j;
  uint8_t numLength;
  for (i = 0; i < numSize; i++){
    num = num * 10;
  }
  numCopy = num;
  divisor = 1;
  for (numLength = 0; numCopy != 0;){
    numCopy = numCopy / 10;
    numLength++;
  }
  for (i = 0; i < numLength-1; i++){
    divisor = divisor * 10;
  }
  for (i = 0; i < numLength || numLength == 0; i++){
    realNum = num/divisor;
    for (j = 0; j < size; j++)
    {
      transferToBitmap(bignums[19*(realNum)+(j*2)]);
      Cursor.row++;
      transferToBitmap(bignums[19*(realNum)+(j*2)+1]);
      Cursor.line++;
      Cursor.row--;
    }
    Cursor.line -= size;
    //num = num % divisor;
    divisor = divisor / 10;
    Cursor.row++;
    if (Cursor.row >= 12)
    {
      Cursor.row = 0;
      Cursor.line = Cursor.line + size;
      if (Cursor.line >= 97)
      {
        Cursor.line = 1;
      }
    }
  }
}

void transferToBitmap(uint8_t data){
  bitmap[Cursor.line*12+Cursor.row] = data;
}


void setCursor(int x, int y){
  Cursor.line = y;
  Cursor.row = x;
}

// uint8_t bitmapToUint(int x, int y){
//   uint8_t returnVal = 0;
//   int i;
//   int offset = y*96;
//   for (i = 0; i < 8; i++)
//   {
//     returnVal = returnVal | (bitmap[offset + i] << i);
//   }
//   return returnVal;
// }

/* ************* */
/* CONSTRUCTORS  */
/* ************* */
// void LCD(uint8_t CLK, uint8_t MOSI, uint8_t SS) {
//   clk = clk;
//   mosi = mosi;
//   ss = ss;

//   // Set pin state before direction to make sure they start this way (no glitching)
//   nrf_gpio_pin_set(ss);
//   nrf_gpio_pin_clear(clk);
//   nrf_gpio_pin_set(mosi);
  
//   nrf_gpio_pin_dir_set(ss, NRF_GPIO_PIN_DIR_OUTPUT);
//   nrf_gpio_pin_dir_set(clk, NRF_GPIO_PIN_DIR_OUTPUT);
//   nrf_gpio_pin_dir_set(mosi, NRF_GPIO_PIN_DIR_OUTPUT);
  
  // clkport     = portOutputRegister(digitalPinToPort(_clk));
  // clkpinmask  = digitalPinToBitMask(_clk);
  // dataport    = portOutputRegister(digitalPinToPort(_mosi));
  // datapinmask = digitalPinToBitMask(_mosi);
  
  // // Set the vcom bit to a defined state
  // _sharpmem_vcom = SHARPMEM_BIT_VCOM;

// }

/* *************** */
/* PRIVATE METHODS */
/* *************** */

 
/**************************************************************************/
/*!
    @brief  Reverses order of bits in input uint8_t.
*/
/**************************************************************************/

uint8_t reverseBitOrder(uint8_t MSB){
		uint8_t LSB = 0;
		LSB |= (((0x01) & MSB)<<7);
    LSB |= (((0x02) & MSB)<<5);
    LSB |= (((0x04) & MSB)<<3);
    LSB |= (((0x08) & MSB)<<1);
    LSB |= (((0x10) & MSB)>>1);
    LSB |= (((0x20) & MSB)>>3);
    LSB |= (((0x40) & MSB)>>5);
    return LSB;
}

/* ************** */
/* PUBLIC METHODS */
/* ************** */



/**************************************************************************/
/*! 
    @brief Clears the screen
*/
/**************************************************************************/
void clearDisplay() 
{
  int i;
  initCursor();
	nrf_gpio_pin_set(SPI_SS_PIN);
  spi_write(LCD_CLEAR);
  spi_write(0x00);
  nrf_gpio_pin_clear(SPI_SS_PIN);
  for (i = 0; i < 96*12; i++)
  {
    bitmap[i] = 0x00;
  }
}

void clearLines(uint8_t start, uint8_t end){
  uint8_t i;
  uint8_t j;
  for (i = (start-1); i < (end-1); i++)
  {
    for (j = 0; j < 12; j++)
    {
      bitmap[i*12+j] = 0x00;
    }
  }
}

/**************************************************************************/
/*! 
    @brief Renders the contents of the pixel buffer on the LCD
*/
/**************************************************************************/
void refresh(void) 
{

	nrf_gpio_pin_set(SPI_SS_PIN);
  uint8_t addr;
  uint8_t byteNum;
  //uint16_t totalbytes = (LCD_WIDTH) / 8;

  //uint8_t pixle;
  spi_write(DATA_WRITE);
  for(addr = 1; addr <= 96; addr++)
  {

      spi_write(reverseBitOrder(addr));
      for(byteNum = 0; byteNum < 12; byteNum++)
      {
          spi_write(~bitmap[(addr-1)*12+byteNum]);
      }
      spi_write(0x00);
  }
  spi_write(0x00);
  nrf_gpio_pin_clear(SPI_SS_PIN);
}

void drawDottedLine(uint8_t line){
  uint8_t i;
  setCursor(0, line-1);
  for (i = 0; i < 12; i++)
  {
    transferToBitmap(0xFF);
    Cursor.row++;
  }
}

void charTest(){
  clearDisplay();
  transferChar('a', 9);
  transferChar('b', 9);
  transferChar('c', 9);
  transferChar('d', 9);
  transferChar('e', 9);
  transferChar('f', 9);
  transferChar('g', 9);
  transferChar('h', 9);
  transferChar('i', 9);
  transferChar('j', 9);
  transferChar('k', 9);
  transferChar('l', 9);
  Cursor.line++;
  transferChar('m', 9);
  transferChar('n', 9);
  transferChar('o', 9);
  transferChar('p', 9);
  transferChar('q', 9);
  transferChar('r', 9);
  transferChar('s', 9);
  transferChar('t', 9);
  transferChar('u', 9);
  transferChar('v', 9);
  transferChar('w', 9);
  transferChar('x', 9);
  Cursor.line++;
  transferChar('y', 9);
  transferChar('z', 9);
  transferSmallNumInt(0,9);
  transferSmallNumInt(1,9);
  transferSmallNumInt(2,9);
  transferSmallNumInt(3,9);
  transferSmallNumInt(4,9);
  transferSmallNumInt(5,9);
  transferSmallNumInt(6,9);
  transferSmallNumInt(7,9);
  transferSmallNumInt(8,9);
  transferSmallNumInt(9,9);
  Cursor.line++;
  transferBigNumInt(0,19);
  transferBigNumInt(1,19);
  transferBigNumInt(2,19);
  transferBigNumInt(3,19);
  transferBigNumInt(4,19);
  transferBigNumInt(5,19);
  Cursor.line++;
  transferBigNumInt(6,19);
  transferBigNumInt(7,19);
  transferBigNumInt(8,19);
  transferBigNumInt(9,19);
  Cursor.line += 20;
  Cursor.row = 0;
  transferBatteryLevel(0);
  transferBatteryLevel(1);
  transferBatteryLevel(2);
  transferBatteryLevel(3);
  transferBatteryLevel(4);
}

void buildTopBar_LCD(){
  clearLines(1,11);
  drawDottedLine(12);
  setCursor(0,1);
  transferBatteryLevel(3);
  Cursor.row++;
  transferChar('e', 9);
  transferChar('i', 9);
  transferChar('r', 9);
  Cursor.row++;
  transferSmallNumInt(1,9);
  transferSmallNumInt(2,9);
  transferSpecialChar(':',9);
  transferSmallNumInt(3,9);
  transferSmallNumInt(4,9);
}
