/* MTK3339.h
 * Interface for the MTK3339 GPS module.
 */

#ifndef _MTK3339_H
#define _MTK3339_H

#include <stdint.h>


/* Struct to hold commonly used GPS data. 
 */
struct gpsDataStruct {
    float latitude;
    float longitude;
    float altitude;
    float utcTime;
};


/* Initialize the GPS module.
 */
void MTK3339_init(/* TODO add the MCU pins and UART handle */);


/* Turn on the GPS functionality. 
 */
void MTK3339_enable();


/* Turn off the GPS functionality and put the module into standby mode. 
 */
void MTK3339_disable();


/* Begin logging the GPS data using the logger on the GPS module. 
 */
void MTK3339_logStart();


/* End logging the GPS data using the logger on the GPS module. 
 */
void MTK3339_logStop();


/* Parse the NMEA sentence to extract GPS data.
 */
void MTK3339_parseSentence(char * sentence, uint8_t len,
                           struct gpsDataStruct * gpsDataPtr);

/* Send an ASCII command to the GPS module.
 */
void MTK3339_sendCommand(char * cmd, uint8_t len);

/* Wait for a sentence of data to be received from the GPS module, then return
 * it in buf;
 */
void MTK3339_waitForSentence(char * buf, uint8_t len);

#endif
