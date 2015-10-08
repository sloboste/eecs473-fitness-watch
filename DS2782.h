/* DS2782.h
 * Interface for the Maxim DS2782 Stand Alone Fuel Gauge IC.
 */


/* The modes of operation that the DS2782 can be in.
 */
enum DS2782_modeEnum = {
    ACTIVE, SLEEP
};


/* Initialize the DS2782 unit to ready it for communication. 
 */
void DS2782_init(/* TODO pass in the MCU pins and I2C handle */);


/* Set the parameters for the DS2782 that allow it to accurately track the
 * battery functionality. This only needs to be called ONCE for the lifetime of
 * the battery and should be called after DS2782_init.
 */
void DS2782_configParams(/* TODO determine exactly which parameters will be set
                            for our use case */);


/* Put the DS2782 into a specified mode of operation.
 */
void DS2782_setMode(enum DS2782_modeEnum mode);


/* Get an instantaneous voltage reading from the battery.
 */
uint16_t DS2782_getInstVoltage(); 


/* Get an instantaneous current reading from the battery.
 */
uint16_t DS2782_getInstCurrent(); 


/* Get an average current reading from the battery.
 */
uint16_t DS2782_getAvgCurrent(); 


/* Get an instantaneous temperature reading from the battery.
 */
uint16_t DS2782_getInstTemp(); 


/* Get the battery's full capacity.
 */
uint16_t DS2782_getFullCapacity(); 


/* Get the battery's remaining active capacity.
 */
uint16_t DS2782_getActiveCapacity(); 


/* Get the battery's remaining standby capacity.
 */
uint16_t DS2782_getStandbyCapacity(); 


/* The registers and corresponding addresses in the DS2782.
 */
enum DS2782_regsisterEnum = {
    STATUS = 0x01,      // Status (R/W)
    RAAC_MSB = 0x02,    // Remaining Active Absolute Capacity MSB (R)
    RAAC_LSB = 0x03,    // Remaining Active Absolute Capacity LSB (R) 
    RSAC_MSB = 0x04,    // Remaining Standby Absolute Capacity MSB (R) 
    RSAC_LSB = 0x05,    // Remaining Standby Absolute Capacity LSB (R) 
    RARC = 0x06,        // Remaining Active Relative Capacity (R)
    RSRC = 0x07,        // Remaining Standby Relative Capacity (R)
    IAVG_MSB = 0x08,    // Average Current Register MSB (R)
    IAVG_LSB = 0x09,    // Average Current Register LSB (R)
    TEMP_MSB = 0x0A,    // Temperature Register MSB (R)
    TEMP_LSB = 0x0B,    // Temperature Register LSB (R)
    VOLT_MSB = 0x0C,    // Voltage Register MSB (R)
    VOLT_LSB = 0x0D,    // Voltage Register LSB (R)
    CURRENT_MSB = 0x0E, // Current Register MSB (R)
    CURRENT_LSB = 0x0F, // Current Register LSB (R)
    ACR_MSB = 0x10,     // Accumulated Current Register MSB (R/W)
    ACR_LSB = 0x11,     // Accumulated Current Register LSB (R/W)
    ACRL_MSB = 0x12,    // Low Accumulated Current Register MSB (R)
    ACRL_LSB = 0x13,    // Low Accumulated Current Register LSB (R)
    AS = 0x14,          // Age Scalar (R/W)
    SFR = 0x15,         // Special Feature Register (R/W)
    FULL_MSB = 0x16,    // Full Capacity MSB (R)
    FULL_LSB = 0x17,    // Full Capacity LSB (R)
    AE_MSB = 0x18,      // Active Empty MSB (R)
    AE_LSB = 0x19,      // Active Empty LSB (R)
    SE_MSB = 0x1A,      // Standby Empty MSB (R)
    SE_LSB = 0x1B,      // Standby Empty LSB (R)
    EEPROM = 0x1F,      // EEPROM Register (R/W)
    /* TODO: add these if necessary later on */
    // User EEPROM block 0 from 0x20 to 0x2F. Lockable. (R/W)
    // Additional user EEPROM block 0 from 0x30 to 0x37. Lockable. (R/W)
    // Parameter EEPROM from 0x60 to 0x7F. Lockable. Block 1. (R/W)
    // Unique ID from 0xF0 to 0xF7. (R)
    FXN_CMD = 0xFE      // Function Command Register (R)
};


/* Low-level write interface to the DS2782. This is exposed to support any
 * functionality without a specific function in this interface.
 */
void _DS2782_writeReg(enum DS2782_regEnum, uint8_t data);


/* Low-level read interface to the DS2782. This is exposed to support any
 * functionality without a specific function in this interface.
 */
uint8_t _DS2782_readReg(enum DS2782_regEnum);
