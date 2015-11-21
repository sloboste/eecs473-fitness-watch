/*********************************************************************
FIXME This large comment is very unnecessary...

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
Steven Sloboda
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

//#include "led.h"


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
  size = 19;
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

// /**************************************************************************/
// /*!
//     @brief    This is a simple test function to print out all charachters
// */
// /**************************************************************************/ 

// void charTest(){
//   clearDisplay();
//   transferChar('a');
//   transferChar('b');
//   transferChar('c');
//   transferChar('d');
//   transferChar('e');
//   transferChar('f');
//   transferChar('g');
//   transferChar('h');
//   transferChar('i');
//   transferChar('j');
//   transferChar('k');
//   transferChar('l');
//   Cursor.line++;
//   transferChar('m');
//   transferChar('n');
//   transferChar('o');
//   transferChar('p');
//   transferChar('q');
//   transferChar('r');
//   transferChar('s');
//   transferChar('t');
//   transferChar('u');
//   transferChar('v');
//   transferChar('w');
//   transferChar('x');
//   Cursor.line++;
//   transferChar('y');
//   transferChar('z');
//   transferSmallNumInt(0);
//   transferSmallNumInt(1);
//   transferSmallNumInt(2);
//   transferSmallNumInt(3);
//   transferSmallNumInt(4);
//   transferSmallNumInt(5);
//   transferSmallNumInt(6);
//   transferSmallNumInt(7);
//   transferSmallNumInt(8);
//   transferSmallNumInt(9);
//   Cursor.line++;
//   transferBigNumInt(0);
//   transferBigNumInt(1);
//   transferBigNumInt(2);
//   transferBigNumInt(3);
//   transferBigNumInt(4);
//   transferBigNumInt(5);
//   Cursor.line++;
//   transferBigNumInt(6);
//   transferBigNumInt(7);
//   transferBigNumInt(8);
//   transferBigNumInt(9);
//   Cursor.line += 20;
//   Cursor.row = 0;
//   transferBatteryLevel(0);
//   transferBatteryLevel(1);
//   transferBatteryLevel(2);
//   transferBatteryLevel(3);
//   transferBatteryLevel(4);
// }
