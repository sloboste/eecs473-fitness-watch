/*********************************************************************
This is a library built specifically for the EIR watch. The 
capabilities of this library are limited to only the necessary 
functions required to get the desired functionality from the LCD.

This LCD (LS013B4DN04) uses SPI to communicate, 3 pins are required to  
interface, not including ground and power.

The Watchmen invest time and resources providing this open source code, 
so please support the EIR watch project and open-source hardware by 
referring the developers to Google and other really cool potential jobs
we may want. Our names can be found below:

Joshua Kaufman
Nathan Immerman
Steven Slaboda
Tyler Kohan
Amit Shah

We are legends of the computer engineering world.

Written by Joshua Kaufman for the EIR watch by The Watchmen.  
*********************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "nrf_gpio.h"

#include "lcd_driver.h"
#include "spi_driver.h"
#include "charData.h"

#include "led.h"

/**************************************************************************
    Sharp Memory Display Connector
    -----------------------------------------------------------------------
    Function        Notes
    ==============  ===============================
    VIN             3.3
    3V3             3.3V out
    GND
    SCLK            Serial Clock
    MOSI            Serial Data Input
    CS              Serial Chip Select
 **************************************************************************/

/* ************* */
/* LCD Constants */
/* ************* */

#define DATA_WRITE				(0x80)
//#define SHARPMEM_BIT_VCOM       (0x40)
#define LCD_CLEAR      		(0x20)

/* ********************* */
/* Initialize the bitmap */
/* ********************* */

uint8_t bitmap[(LCD_WIDTH * LCD_HEIGHT)/8] = { 0 };

/**************************************************************************/
/*!
    @brief    Initializes the cursor to row 0 and line 0. This is the top
    @         left position of the LCD.
*/
/**************************************************************************/ 

void initCursor(){
  Cursor.line = 0;
  Cursor.row = 0;
}

/**************************************************************************/
/*!
    @brief    Initializes the data structures for the peripherals. The 
    @         current value and data types are not necessarily correct
*/
/**************************************************************************/  

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

/**************************************************************************/
/*!
    @brief    Macro function which builds the "GPS" screen on the bitmap.
    @         Currently empty.  Will implement between 11/11/15 - 11/13/15.

    @size     Uses all 12 rows on the bottom 83 lines 
*/
/**************************************************************************/  

  void buildGPS_LCD(){

  }

/**************************************************************************/
/*!
    @brief    Macro function which builds the "TIMER" screen on the bitmap.
    @         Currently empty.  Will implement between 11/11/15 - 11/13/15.

    @size     Uses all 12 rows on the bottom 83 lines  
*/
/**************************************************************************/  

void buildTimer_LCD();

/**************************************************************************/
/*!
    @brief    Macro function which builds the "RUN" screen on the bitmap.
    @         Currently is not dynamic. Dynamics will be implemented
    @         between 11/11/15 - 11/13/15.

    @size     Uses all 12 rows on the bottom 83 lines 
*/
/**************************************************************************/

void buildRun_LCD(){
  clearLines(13,96);
  setCursor(0, 14);
  transferChar('t');
  transferChar('i');
  transferChar('m');
  transferChar('e');

  setCursor(1, 28);
  transferBigNumInt(1);
  transferBigNumInt(2);
  transferSpecialBigChar(':');
  transferBigNumInt(3);
  transferBigNumInt(4);

  drawLine(68);

  setCursor(0, 70);
  transferChar('d');
  transferChar('i');
  transferChar('s');
  transferChar('t');
  transferSpecialChar(':');
  Cursor.row++;
  transferSmallNumInt(1);
  transferSpecialChar('.');
  transferSmallNumInt(2);
  transferSmallNumInt(3);

  drawLine(82);

  setCursor(0, 84);
  transferChar('p');
  transferChar('a');
  transferChar('c');
  transferChar('e');
  transferSpecialChar(':');
  Cursor.row++;
  transferSmallNumInt(4);
  transferSpecialChar(':');
  transferSmallNumInt(5);
  transferSmallNumInt(6);
  }

/**************************************************************************/
/*!
    @brief    Takes in a lowercase letter and places it at the cursor 
    @         location on the bitmap.  Function auto increments and 
    @         repositions cursor to keep text from falling off the screen. 
    @         This function should be used if you want a small font.   

    @size     Uses 1 row1 on 9 lines 
*/
/**************************************************************************/  

void transferChar(char c){
  int i;
  uint8_t size;
  size = 9;
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

/**************************************************************************/
/*!
    @brief    Takes in a ':', '.', or '/' char and places it at the cursor 
    @         location on the bitmap.  Function auto increments and 
    @         repositions cursor to keep text from falling off the screen.
    @         This function should be used if you want a small font.  

    @size     Uses 1 row on 9 lines 
*/
/**************************************************************************/  

void transferSpecialChar(char c){
  int i;
  int offset;
  uint8_t size;
  size = 9;
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

/**************************************************************************/
/*!
    @brief    Currently can only take a ':' char and place it at the cursor 
    @         location on the bitmap.  Function auto increments and 
    @         repositions cursor to keep text from falling off the screen.
    @         This function should be used if you want a large font.  

    @size     Uses 1 row on 19 lines  Currently only ':' char exists,
    @         with the introduction of new chars this size may vary
    @         depending on the char.
*/
/**************************************************************************/ 

void transferSpecialBigChar(char c){
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

/**************************************************************************/
/*!
    @brief    Takes in an integer of any size and places it at the cursor 
    @         location on the bitmap.  Function auto increments and 
    @         repositions cursor to keep numbers from falling off the 
    @         screen. This function should be used if you want to display
    @         small numbers. 

    @size     Uses 1 row on 9 lines
*/
/**************************************************************************/ 

void transferSmallNumInt(int num){
  int divisor;
  int numCopy;
  uint8_t size;
  uint8_t realNum;
  uint8_t i;
  uint8_t j;
  uint8_t numLength;
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

/**************************************************************************/
/*!
    @brief    Takes in an integer of any size and places it at the cursor 
    @         location on the bitmap.  Function auto increments and 
    @         repositions cursor to keep numbers from falling off the 
    @         screen. This function should be used if you want to display
    @         large numbers.  

    @size     Uses 2 rows on 19 lines 
*/
/**************************************************************************/ 

void transferBigNumInt(int num){
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

/**************************************************************************/
/*!
    @brief    Takes in an integer of size 0 - 4, inclusive, and places the
    @         corresponding battery level image at the cursor location.
    @         0 being for very low, and 4 being for full.

    @size     Uses 2 rows on 9 lines
*/
/**************************************************************************/ 

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

/**************************************************************************/
/*!
    @brief    Transfers the uint8_t "data" to the bitmap at the current
    @         cursors location

    @size     Uses 1 row on 1 line
*/
/**************************************************************************/ 

void transferToBitmap(uint8_t data){
  bitmap[Cursor.line*12+Cursor.row] = data;
}

/**************************************************************************/
/*!
    @brief    Sets the cursors row to 'x' and line to 'y'
*/
/**************************************************************************/ 

void setCursor(int x, int y){
  Cursor.line = y;
  Cursor.row = x;
}
 
/**************************************************************************/
/*!
    @brief    Returns reversed bit order with respect to the input "MSB".
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

/**************************************************************************/
/*! 
    @brief    Clears the screen and bitmap.
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

/**************************************************************************/
/*! 
    @brief    Clears the bitmap between "start" and "end" positions
    @         inclusivley.
*/
/**************************************************************************/

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
    @brief    Sends the contents of the pixel buffer to the LCD.
*/
/**************************************************************************/

void refresh(void) 
{
	nrf_gpio_pin_set(SPI_SS_PIN);
  uint8_t addr;
  uint8_t byteNum;
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

/**************************************************************************/
/*!
    @brief    Creates a line at the line number specified as input.

    @size     Uses all 12 rows on 1 line
*/
/**************************************************************************/ 

void drawLine(uint8_t line){
  uint8_t i;
  setCursor(0, line-1);
  for (i = 0; i < 12; i++)
  {
    transferToBitmap(0xFF);
    Cursor.row++;
  }
}

/**************************************************************************/
/*!
    @brief    This is a simple test function to print out all charachters
*/
/**************************************************************************/ 

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

/**************************************************************************/
/*!
    @brief    Macro function which builds the "TOP BAR" portion of the bitmap.
    @         Currently is not dynamic. Dynamics will be implemented
    @         between 11/11/15 - 11/13/15.

    @size     Uses all 12 rows on the top 13 lines 
*/
/**************************************************************************/

void buildTopBar_LCD(){
  clearLines(1,11);
  drawLine(12);
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
