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
 * Interface for GPS module.
 * Note: much of this taken from Adafruit Ultimate GPS Arduino Library
 *
 * To generate your own sentences, check out the MTK command datasheet and use
 * a checksum calculator such as http://www.hhhh.org/wiml/proj/nmeaxor.html
 */

#ifndef GPS_H
#define GPS_H

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

#define PMTK_LOCUS_STARTLOG  "$PMTK185,0*22"
#define PMTK_LOCUS_STOPLOG "$PMTK185,1*23"
#define PMTK_LOCUS_ERASE_FLASH "$PMTK184,1*22"
#define PMTK_LOCUS_GET_FLASH "$PMTK622,1*29"
#define LOCUS_OVERLAP 0
#define LOCUS_FULLSTOP 1


// Sentence types we care about
#define GPS_TYPE_INVALID    0 
#define GPS_TYPE_NO_FIX     1
#define GPS_TYPE_GPRMC      2 // Has long, lat, time, speed
#define GPS_TYPE_GPGGA      3 // Has long, lat, time, altitude

#define KNOT_IN_METERS_PER_SECOND 0.514444F


#define GPS_LAT_LONG_LEN    20

// Information from the GPS module
typedef struct gps_info_struct {
    // Altitude (meters) relative to sea level
    int32_t altitude; 
    // Ground speed (Knots)
    float speed;
    // Coordinates formated like "083 44.1917 W" null terminated
    char latitude[14]; 
    char longitude[14]; 
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
 * Returns true if the GPS is not in standby mode.
 */
bool gps_is_enabled();

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
 * 
 * info_ptr -- pointer to the struct in which to place the data got.
 *
 * Returns the type sentence data contained in the struct. 
 */
extern int gps_get_info(gps_info_t * info_ptr);

/**
 * Send a message string (must be null terminated) to the GPS module.
 */
extern void gps_send_msg(char * msg);

/**
 * Erase the contents of the GPS log flash memory.
 */
extern void gps_erase_log();

/**
 * Enable logging on the GPS.
 */
extern void gps_start_logging();

/**
 * Disable logging on the GPS.
 */
extern void gps_stop_logging();

/**
 * Tell the GPS module to dump the contents of its log flash memory.
 *
 * Returns the number of sentences contained in the dump.
 */
extern uint16_t gps_flash_dump();

/**
 * Get the next chunk of the log dump. Call this in a loop until it returns 0
 * in order to get the full log dump.
 *
 * buf -- buffer in which to put the bytes from the log. 
 * num_bytes_desired -- how many bytes you want put into buf. 
 * 
 * Returns the number of bytes put into buf. This may be less than
 * num_bytes_desired. Returns 0 when there are no more bytes in the log.
 */
extern uint8_t gps_get_log_dump_bytes(uint8_t * buf, uint8_t num_bytes_desired);

#endif
