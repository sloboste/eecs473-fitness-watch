/* Interface for GPS module.
 * Note: much of this taken from Adafruit Ultimate GPS Arduino Library
 */

#ifndef __GPS_H
#define __GPS_H

// TODO Clean this up

// different commands to set the update rate from once a second (1 Hz) to 10 times a second (10Hz)
// Note that these only control the rate at which the position is echoed, to actually speed up the
// position fix you must also send one of the position fix rate commands below too.
//#define PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ  "$PMTK220,10000*2F" // Once every 10 seconds, 100 millihertz.
//#define PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ  "$PMTK220,5000*1B"  // Once every 5 seconds, 200 millihertz.
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
//#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
//#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"
// Position fix update rate commands.
//#define PMTK_API_SET_FIX_CTL_100_MILLIHERTZ  "$PMTK300,10000,0,0,0,0*2C" // Once every 10 seconds, 100 millihertz.
//#define PMTK_API_SET_FIX_CTL_200_MILLIHERTZ  "$PMTK300,5000,0,0,0,0*18"  // Once every 5 seconds, 200 millihertz.
#define PMTK_API_SET_FIX_CTL_1HZ  "$PMTK300,1000,0,0,0,0*1C"
//#define PMTK_API_SET_FIX_CTL_5HZ  "$PMTK300,200,0,0,0,0*2F"
// Can't fix position faster than 5 times a second!


//#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C"
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"

// turn on only the second sentence (GPRMC)
//#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
// turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn on ALL THE DATA
//#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn off output
//#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

// to generate your own sentences, check out the MTK command datasheet and use a checksum calculator
// such as the awesome http://www.hhhh.org/wiml/proj/nmeaxor.html

#define PMTK_LOCUS_STARTLOG  "$PMTK185,0*22"
#define PMTK_LOCUS_STOPLOG "$PMTK185,1*23"
#define PMTK_LOCUS_STARTSTOPACK "$PMTK001,185,3*3C"
#define PMTK_LOCUS_QUERY_STATUS "$PMTK183*38"
#define PMTK_LOCUS_ERASE_FLASH "$PMTK184,1*22"
#define LOCUS_OVERLAP 0
#define LOCUS_FULLSTOP 1

//#define PMTK_ENABLE_SBAS "$PMTK313,1*2E"
//#define PMTK_ENABLE_WAAS "$PMTK301,2*2E"

// standby command & boot successful message
#define PMTK_STANDBY "$PMTK161,0*28"
//#define PMTK_STANDBY_SUCCESS "$PMTK001,161,3*36"  // Not needed currently
#define PMTK_AWAKE "$PMTK010,002*2D"

// ask for the release and version
//#define PMTK_Q_RELEASE "$PMTK605*31"

// request for updates on antenna status 
//#define PGCMD_ANTENNA "$PGCMD,33,1*6C" 
#define PGCMD_NOANTENNA "$PGCMD,33,0*6D" 


// Sentence types we care about
#define GPS_TYPE_GPRMC 0 // Has long, lat, time, speed
#define GPS_TYPE_GPGGA 1 // Has long, lat, time, altitude

// TODO would be great to not use a float
//#define KNOT_IN_METERS_PER_SECOND 0.514444F


// Information from the GPS module
typedef struct gps_info_struct {
    // Altitude (meters) relative to sea level
    int32_t altitude; 
    // Ground speed (Knots)
    // TODO would be great to not use a float
    float speed;
    // Coordinates formated like "083 44.1917 W" null terminated
    char latitude[16]; 
    char longitude[16]; 
    // Time (UTC)
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} gps_info_t;


// Initialize the GPS module
//extern void gps_init(uint32_t pin_enable);
extern void gps_init();

// Turn on the GPS module 
extern void gps_enable();

// Turn off the GPS module 
extern void gps_disable();

// Configure the GPS module with our preferred settings
extern void gps_config();

// Get information from the GPS module 
extern void gps_get_info(gps_info_t * info_ptr, int type);


// Send a message string (must be null terminated) to the GPS module
extern void gps_send_msg(char * msg);

#endif