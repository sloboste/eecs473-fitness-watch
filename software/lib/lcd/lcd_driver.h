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
 *
 * 
 * TODO Add proper comments for functions.
 */

#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <stdint.h>


// LCD dimensions
#define LCD_WIDTH       (96)
#define LCD_HEIGHT      (96) 

struct lcd_cursor_struct {
	uint8_t line;
	uint8_t row;
} lcd_Cursor;

/* ************** */
/* Init Functions */
/* ************** */

extern void lcd_initCursor();

/* *********************************** */
/* Transfer, Draw, and Erase Functions */
/* *********************************** */

extern void lcd_drawLine(uint8_t line);
extern void lcd_transferToBitmap(uint8_t data);
extern void lcd_clearLines(uint8_t start, uint8_t end);

extern void lcd_transferString(char * s);
extern void lcd_transferChar(char c);
extern void lcd_transferSmallNumInt(int num);
extern void lcd_transferBigNumInt(int num);
extern void lcd_transferSpecialChar(char c);
extern void lcd_transferSpecialBigChar(char c);

extern void lcd_transferBatteryLevel(uint8_t percentage);

extern void lcd_invertBitMap(uint8_t row, uint8_t line, uint8_t numLines);

/* ***************************** */
/* LCD Transfer Helper Functions */
/* ***************************** */

extern uint8_t lcd_reverseBitOrder(uint8_t MSB);

/* ******************************** */
/* Macro LCD Manipulation Functions */
/* ******************************** */

extern void lcd_refresh(void);
extern void lcd_clearDisplay();

/* ********************** */
/* LCD Mangment Functions */
/* ********************** */

extern void lcd_setCursor(int x, int y);

/* ************** */
/* Test Functions */
/* ************** */

//extern void lcd_charTest();

#endif
