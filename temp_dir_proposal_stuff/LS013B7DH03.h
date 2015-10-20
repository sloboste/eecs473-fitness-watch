/* LS013B7DH03.h
 * Interface for the LS013B7DH03 LCD.
 */

#ifndef _LS013B7DH03_H
#define _LS013B7DH03_H

#include <stdint>
#include <stdbool.h>


/* Initialize the LCD.
 */
void LS013B7DH03_init(/* TODO pass in the MCU pins and SPI handle */);


/* Stores the bit map of the current contents of the screen as a 1-D array.
 * When a row is edited in this array, the correpsonding bit in
 * LS013B7DH03_editedRows should be set. Afterward, LS013B7DH03_pushImage should be
 * called.
 */
bool LS013B7DH03_bitMap[16384];


/* Keeps track of which lines of the image have been edited so that only the
 * sections of the screen requiring edits will be written to. This is to save 
 * CPU time, and refresh rate
 */
bool LS013B7DH03_editedRows[128];
 

/* Write a row of the bit map using the corresponding row in the
 * LS013B7DH03_bitMap
 * array, to the LCD.
 */
void LS013B7DH03_writeRow(uint8_t row);


/* Calls LS013B7DH03_writeRow() for all the lines of the LCD that require
 * updating and clears the LS013B7DH03_editedRows bits for those rows.
 */
void LS013B7DH03_pushImage();


/* Low-level write interface to the LS013B7DH03. This is exposed to support any
 * functionality without a specific function in this interface.
 */
void _LS013B7DH03_writeReg(uint8_t reg, uint8_t data, bool EXTCOMIN);

#endif
