/* MAX30100.h
 * Interface for the MAX30100 Heart Rate Monitor.
 */

#ifndef HR_H
#define HR_H

#include <stdint.h>
#include <stdbool.h>


//Register Constants (all support R/W)
#define FIFO_DATA_REG 0x05
#define MODE_CONFIG_REG 0x06
#define SPO2_CONFIG_REG 0x07
#define LED_CONFIG_REG 0x09
#define TEMP_INT_REG 0x16
#define TEMP_FRAC_REG 0x17


/* This will setup the Heart Rate Monitor properly.
 * Need to configure the IC's mode of operation for our application.
 */
void MAX30100_init();


/* Takes a 10 second reading of the user's heart rate
 * Returns the number of beats in one minute
 */
uint8_t MAX30100_takeReading();


/* If en is true, then put device into power saving mode. If en is false,
 * put device into normal operation mode
 */
void MAX30100_powerSaveEnable(bool en);		


uint16_t _MAX30100_buffer[16];
uint16_t _MAX30100_temperature_val;


/* Helper function for writing registers on the IC.
 * Will be used in constructor for configuration
 */
void _MAX30100_writeReg(uint8_t reg, uint8_t data);


/* Helper function for reading registers on the IC.
 * Will be used in getOneSample() and getTemp()
 */
uint8_t _MAX30100_readReg(uint8_t reg);


/* Each I2C transaction will return 4 bits of useful info. Need to read 4 times
 * for one sample. Store each sample in buffer for takeReading to process
 */
uint16_t _MAX30100_getOneSample();


/* Gets current temperature. Useful for adding precision to our measurements
 */
uint16_t _MAX30100_getTemp();

#endif
