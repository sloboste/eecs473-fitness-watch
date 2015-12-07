/**
 * Interface for GPS module.
 * Note: much of this taken from Adafruit Ultimate GPS Arduino Library
 *
 * To generate your own sentences, check out the MTK command datasheet and use
 * a checksum calculator such as http://www.hhhh.org/wiml/proj/nmeaxor.html
 */

#ifndef __GPS_H
#define __GPS_H

// TODO Clean this up

// How fast the GPS module outputs sentences
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"

// How fast the GPS module updates its position fix.
#define PMTK_API_SET_FIX_CTL_1HZ  "$PMTK300,1000,0,0,0,0*1C"

// Baudrate the GPS module communicates with
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"

// Turn on GPRMC and GGA sentence output
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// Turn off output
#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

// standby command & boot successful message
#define PMTK_STANDBY "$PMTK161,0*28"
#define PMTK_AWAKE "$PMTK010,002*2D"

// Do not output antenna status updates
#define PGCMD_NOANTENNA "$PGCMD,33,0*6D" 

// TODO determine if we will use all of these ---
#define PMTK_LOCUS_STARTLOG  "$PMTK185,0*22"
#define PMTK_LOCUS_STOPLOG "$PMTK185,1*23"
#define PMTK_LOCUS_STARTSTOPACK "$PMTK001,185,3*3C"
#define PMTK_LOCUS_QUERY_STATUS "$PMTK183*38"
#define PMTK_LOCUS_ERASE_FLASH "$PMTK184,1*22"
#define PMTK_LOCUS_GET_FLASH "$PMTK622,1*29"
#define LOCUS_OVERLAP 0
#define LOCUS_FULLSTOP 1
// ---


// Sentence types we care about
#define GPS_TYPE_GPRMC 0 // Has long, lat, time, speed
#define GPS_TYPE_GPGGA 1 // Has long, lat, time, altitude

// TODO would be great to not use a float
//#define KNOT_IN_METERS_PER_SECOND 0.514444F


#define GPS_LAT_LONG_LEN    20

// Information from the GPS module
typedef struct gps_info_struct {
    // Altitude (meters) relative to sea level
    int32_t altitude; 
    // Ground speed (Knots)
    // FIXME change this tonight so that it is in some kind of meters unit as some kind of unsigned integer unit
    float speed;
    // Coordinates formated like "083 44.1917 W" null terminated
    char latitude[16]; 
    char longitude[16]; 
    // Time (UTC)
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} gps_info_t;


/**
 * Initialize the GPS module.
 */
extern void gps_init();

/**
 * Turn on the GPS module. 
 */
extern void gps_enable();

/**
 * Turn off the GPS module. 
 */
extern void gps_disable();

/**
 * Configure the GPS module with our preferred settings.
 */
extern void gps_config();

/** 
 * Get information from the GPS module. 
 */
extern void gps_get_info(gps_info_t * info_ptr, int type);


/**
 * Send a message string (must be null terminated) to the GPS module.
 */
extern void gps_send_msg(char * msg);

#endif
