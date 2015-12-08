/*********************************************************************
This LCD (LS013B4DN04) uses SPI to communicate, 3 pins are required to  
interface, not including ground and power.

Written by Joshua Kaufman for the EIR watch by The Watchmen.  
*********************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "nrf_gpio.h"

#include "lcd_driver.h"
#include "spi_driver.h"
#include "charData.h"


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
void lcd_initCursor(){
    lcd_Cursor.line = 0;
    lcd_Cursor.row = 0;
}

/**************************************************************************/
/*!
    @brief    Takes in a string and places it at the cursor 
    @         location on the bitmap.  Function auto increments and 
    @         repositions cursor to keep text from falling off the screen. 
    @         This function should be used if you want a small font.   

    @size     Each charachter uses 1 row on 9 lines, 12 charachters per
              with around 10 lines available
*/
/**************************************************************************/  

void lcd_transferString(char * s){
    int i;
    for (i = 0; s[i] != '\0' ; i++)
    {
        if ((int)s[i] > 96)
            lcd_transferChar(s[i]);
        else
            lcd_transferSmallNumInt((int)s[i]-'0');
    }
}

/**************************************************************************/
/*!
    @brief    Takes in a lowercase letter and places it at the cursor 
    @         location on the bitmap.  Function auto increments and 
    @         repositions cursor to keep text from falling off the screen. 
    @         This function should be used if you want a small font.   

    @size     Uses 1 row on 9 lines 
*/
/**************************************************************************/  

void lcd_transferChar(char c){
    int i;
    uint8_t size;
    size = 9;
    for (i = 0; i < size; i++){
        lcd_transferToBitmap(chars[9*((int)c - 97)+i]);
        lcd_Cursor.line++;
    }
    lcd_Cursor.line -= size;
    lcd_Cursor.row++;
    if (lcd_Cursor.row >= 12)
    {
        lcd_Cursor.row = 0;
        lcd_Cursor.line = lcd_Cursor.line + size;
        if (lcd_Cursor.line >= 97)
        {
            lcd_Cursor.line = 1;
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

void lcd_transferSpecialChar(char c){
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
    if (c == '&')
    {
        offset = 29;
    }
    if (c == '.')
    {
        offset = 30;
    }
    for (i = 0; i < size; i++){
        lcd_transferToBitmap(chars[9*offset+i]);
        lcd_Cursor.line++;
    }
    lcd_Cursor.line -= size;
    lcd_Cursor.row++;
    if (lcd_Cursor.row >= 12)
    {
        lcd_Cursor.row = 0;
        lcd_Cursor.line = lcd_Cursor.line + size;
        if (lcd_Cursor.line >= 97)
        {
            lcd_Cursor.line = 1;
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

void lcd_transferSpecialBigChar(char c){
    uint8_t i;
    uint8_t offset;
    uint8_t size;
    size = 19;
    if (c == ':')
    {
        offset = 11;
    }
    for (i = 0; i < size; i++){
        lcd_transferToBitmap(bignums[19*2*offset+i]);
        lcd_Cursor.line++;
    }
    lcd_Cursor.line -= size;
    lcd_Cursor.row++;
    if (lcd_Cursor.row >= 12)
    {
        lcd_Cursor.row = 0;
        lcd_Cursor.line = lcd_Cursor.line + size;
        if (lcd_Cursor.line >= 97)
        {
            lcd_Cursor.line = 1;
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

void lcd_transferSmallNumInt(int num){
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
            lcd_transferToBitmap(smallnums[9*(realNum)+j]);
            lcd_Cursor.line++;
        }
        lcd_Cursor.line -= size;
        num = num % divisor;
        divisor = divisor / 10;
        lcd_Cursor.row++;
        if (lcd_Cursor.row >= 12)
        {
            lcd_Cursor.row = 0;
            lcd_Cursor.line = lcd_Cursor.line + size;
            if (lcd_Cursor.line >= 97)
            {
                lcd_Cursor.line = 1;
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

void lcd_transferBigNumInt(int num){
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
    if (num == 99999)
    {
        divisor = 1;
        numLength = 1;
        num = 10;
    }
    for (i = 0; i < numLength; i++){
        realNum = num/divisor;
        for (j = 0; j < size; j++)
        {
            lcd_transferToBitmap(bignums[19*2*(realNum)+(j*2)]);
            lcd_Cursor.row++;
            lcd_transferToBitmap(bignums[19*2*(realNum)+(j*2)+1]);
            lcd_Cursor.line++;
            lcd_Cursor.row--;
        }
        lcd_Cursor.line -= size;
        num = num % divisor;
        divisor = divisor / 10;
        lcd_Cursor.row += 2;
        if (lcd_Cursor.row >= 12)
        {
            lcd_Cursor.row = 0;
            lcd_Cursor.line = lcd_Cursor.line + size;
            if (lcd_Cursor.line >= 97)
            {
                lcd_Cursor.line = 1;
            }
        }
    }
}

/**************************************************************************/
/*!
    TODO fix comment

    @brief    Takes in an integer of size 0 - 4, inclusive, and places the
    @         corresponding battery level image at the cursor location.
    @         0 being for very low, and 4 being for full.

    @size     Uses 2 rows on 9 lines
*/
/**************************************************************************/ 

void lcd_transferBatteryLevel(uint8_t percentage){
    int divisor;
    int num = percentage / 20;
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
            lcd_transferToBitmap(battery[9*2*(realNum)+(j*2)]);
            lcd_Cursor.row++;
            lcd_transferToBitmap(battery[9*2*(realNum)+(j*2)+1]);
            lcd_Cursor.line++;
            lcd_Cursor.row--;
        }
        lcd_Cursor.line -= size;
        num = num % divisor;
        divisor = divisor / 10;
        lcd_Cursor.row += 2;
        if (lcd_Cursor.row >= 12)
        {
            lcd_Cursor.row = 0;
            lcd_Cursor.line = lcd_Cursor.line + size;
            if (lcd_Cursor.line >= 97)
            {
                lcd_Cursor.line = 1;
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

void lcd_transferToBitmap(uint8_t data){
    bitmap[lcd_Cursor.line*12+lcd_Cursor.row] = data;
}

/**************************************************************************/
/*!
    @brief    Sets the cursors row to 'x' and line to 'y'
*/
/**************************************************************************/ 

void lcd_setCursor(int x, int y){
    lcd_Cursor.line = y;
    lcd_Cursor.row = x;
}
 
/**************************************************************************/
/*!
    @brief    Returns reversed bit order with respect to the input "MSB".
*/
/**************************************************************************/

uint8_t lcd_reverseBitOrder(uint8_t MSB){
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
    @brief    Flips the bit values in uint8_t at position "row", and
              continues for "lines" number of lines.
*/
/**************************************************************************/

void lcd_invertBitMap(uint8_t row, uint8_t line, uint8_t numLines)
{
    int i;
    for (i = line; i < (line + numLines); i++)
    {
        bitmap[i*12+row] = ~bitmap[i*12+row];
    }
}

/**************************************************************************/
/*! 
    @brief    Clears the screen and bitmap.
*/
/**************************************************************************/

void lcd_clearDisplay() 
{
    int i;
    lcd_initCursor();
  	spi_set_slave_select();//nrf_gpio_pin_set(SPI_SS_PIN);
    spi_write(LCD_CLEAR);
    spi_write(0x00);
    spi_clear_slave_select();//nrf_gpio_pin_clear(SPI_SS_PIN);
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

void lcd_clearLines(uint8_t start, uint8_t end){
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

void lcd_refresh(void) 
{
  	spi_set_slave_select();//nrf_gpio_pin_set(SPI_SS_PIN);
    uint8_t addr;
    uint8_t byteNum;
    spi_write(DATA_WRITE);
    for(addr = 1; addr <= 96; addr++)
    {

        spi_write(lcd_reverseBitOrder(addr));
        for(byteNum = 0; byteNum < 12; byteNum++)
        {
            spi_write(~bitmap[(addr-1)*12+byteNum]);
        }
        spi_write(0x00);
    }
    spi_write(0x00);
    spi_clear_slave_select();//nrf_gpio_pin_clear(SPI_SS_PIN);
}

/**************************************************************************/
/*!
    @brief    Creates a line at the line number specified as input.

    @size     Uses all 12 rows on 1 line
*/
/**************************************************************************/ 

void lcd_drawLine(uint8_t line){
    uint8_t i;
    lcd_setCursor(0, line-1);
    for (i = 0; i < 12; i++)
    {
        lcd_transferToBitmap(0xFF);
        lcd_Cursor.row++;
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
