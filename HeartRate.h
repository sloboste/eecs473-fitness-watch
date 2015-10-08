#ifndef HR_H
#define HR_H

#include <stdint.h>

//Register Constants (all support R/W)
#define FIFO_DATA_REG 0x05
#define MODE_CONFIG_REG 0x06
#define SPO2_CONFIG_REG 0x07
#define LED_CONFIG_REG 0x09
#define TEMP_INT_REG 0x16
#define TEMP_FRAC_REG 0x17

	class HR
	{
		uint16_t buffer[16];
		uint16_t temperature_val;

		// Helper function for writing registers on the IC.
		// Will be used in constructor for configuration
		void HRI2CWrite(unsigned char slave_addr, unsigned char reg_addr, unsigned length, unsigned char * data);

		// Helper function for reading registers on the IC.
		// Will be used in getOneSample() and getTemp()
		uint8_t HRI2CRead(unsigned char slave_addr, unsigned char reg_addr, unsigned length);

		// Each I2C transaction will return 4 bits of useful info. Need to read 4 times
		// for one sample. Store each sample in buffer for takeReading to process
		uint16_t getOneSample();

		// Gets current temperature. Useful for adding precision to our measurements
		uint16_t getTemp();

	public:
		// This will setup the Heart Rate Monitor properly.
		// Need to configure the IC's mode of operation for our application
		HR::HR();

		// Takes a 10 second reading of the User's heart rate
		// Returns the number of beats in one minute
		uint8_t takeReading();

		// if en is true, then put device into power saving mode. If en is false,
		// put device into normal operation mode
		void powerSaveEnable(bool en);		
	};
	
#endif