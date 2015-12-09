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
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "gps.h"
#include "uart_adapter.h"

#define MAX_SENTENCE_LEN    130
#define LOG_DUMP_LEN        8192

#include "nrf_gpio.h" // FIXME remove
#include "boards.h" // FIXME remove


static char gps_buffer[LOG_DUMP_LEN];
static uint32_t gps_log_dump_len;


void gps_init()
{

}

void gps_enable()
{
    gps_send_msg(PMTK_AWAKE);
}

void gps_disable()
{
    gps_send_msg(PMTK_STANDBY);
}

void gps_config()
{
    gps_send_msg(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps_send_msg(PMTK_SET_NMEA_UPDATE_1HZ);
    gps_send_msg(PMTK_API_SET_FIX_CTL_1HZ);
    gps_send_msg(PGCMD_NOANTENNA);
}

void gps_send_msg(char * msg)
{
    uart_adapter_write(msg, strlen(msg));
    uart_adapter_write("\r\n", 2);
}

static int sentence_type(char * buf)
{
    // Sentences begin with "$GPXXX" which indicates the type
    if ( (buf[0] == '$') && (buf[1] == 'G') && (buf[2] == 'P') &&   
         (buf[3] == 'R') && (buf[4] == 'M') && (buf[5] == 'C') ) {     
        return GPS_TYPE_GPRMC;                                                           
    }   
    if ( (buf[0] == '$') && (buf[1] == 'G') && (buf[2] == 'P') &&   
         (buf[3] == 'G') && (buf[4] == 'G') && (buf[5] == 'A') ) {     
        return GPS_TYPE_GPGGA;                                                           
    }   
    return -1;
}

static uint8_t get_sentence(char * buf, uint8_t buf_len)
{
    memset(buf, 0, buf_len);
    // Find start    
    do {
        uart_adapter_read(&buf[0], 1);
    } while (buf[0] != '$');

    // Read until checksum
    uint8_t idx = 1;
    while (true) {
        uart_adapter_read(&buf[idx], 1);
        if (buf[idx] == '*') {
            ++idx; 
            break;
        }
        ++idx;
    }

    // Checksum
    uart_adapter_read(&buf[idx], 1);
    ++idx;
    uart_adapter_read(&buf[idx], 1);
    ++idx;

    nrf_gpio_pin_toggle(PIN_LED_3);    

    return idx;
}

static void parse_time(char * buf, uint8_t * hours_ptr, uint8_t * minutes_ptr,
                       uint8_t * seconds_ptr)
{
    // Format is like "HHMMSS.SS"
    char temp;

    buf[6] = '\0';
    *seconds_ptr = (uint8_t) atoi(buf + 4);
    buf[6] = '.';
    temp = buf[4];
    buf[4] = '\0';
    *minutes_ptr = (uint8_t) atoi(buf + 2);
    buf[4] = temp;
    temp = buf[2];
    buf[2] = '\0';
    *hours_ptr = (uint8_t) atoi(buf);
    buf[2] = temp;
}

static void parse_coordinate(char * buf, char * coord)
{
    // Format comes in like "1234.56,N"
    // Get coordinate and format like "083 42.8600 W"
    // This format works in Google Maps, e.g., "42 17.5400 N, 083 42.8600 W"
    char * end;
    uint8_t size;

    end = strchr(buf, '.') - 2; 
    size = (uint8_t) (end - buf);
    strncpy(coord, buf, size);
    (coord + size)[0] = ' ';
    buf += size;
    end = strchr(buf, ',');
    end[0] = ' ';
    ++end;
    end = strchr(end, ',');
    strncpy(coord + size + 1, buf, (end - buf));
    ++end; 
    buf = end;
}

static bool parse_gpgga(char * buf, gps_info_t * gps_info_ptr)
{
    // $GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx
    //        time      lat       long       fix      alt 

    char * start = buf;
    char * end;
    
    // Identifier
    start = strchr(start, ',') + 1;

    // Time
    parse_time(start, &gps_info_ptr->hours, &gps_info_ptr->minutes,
               &gps_info_ptr->seconds);
    start = strchr(start, ',') + 1;

    // Get lattitude 
    parse_coordinate(start, gps_info_ptr->latitude); 
    start = strchr(start, ',') + 1;
    
    // Get longitude 
    parse_coordinate(start, gps_info_ptr->longitude); 
    start = strchr(start, ',') + 1;

    // Skip over stuff
    start = strchr(start, ',') + 1;
    start = strchr(start, ',') + 1;
    start = strchr(start, ',') + 1;

    // Altitude (truncate to 1 meter resolution)
    end = strchr(start, '.');
    end[0]  = '\0';
    gps_info_ptr->altitude = atoi(start);
    end[0]  = '.';

    return true;
}

static bool parse_gprcm(char * buf, gps_info_t * gps_info_ptr)
{
    // $GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a*hh
    //                    lat       long

    char * start = buf;
    char * end;
    
    // Identifier
    start = strchr(start, ',') + 1;

    // Time
    parse_time(start, &gps_info_ptr->hours, &gps_info_ptr->minutes,
               &gps_info_ptr->seconds);
    start = strchr(start, ',') + 1;

    // Active?
    if (start[0] != 'A') {
        return false; 
    }
    start = strchr(start, ',') + 1;

    // Get lattitude 
    parse_coordinate(start, gps_info_ptr->latitude); 
    start = strchr(start, ',') + 1;
    
    // Get longitude and format like "12 34.56 N" 
    parse_coordinate(start, gps_info_ptr->longitude); 
    start = strchr(start, ',') + 1;

    // Ground speed in Knots
    end = strchr(start, ',') + 1;
    end[0] = '\0'; 
    gps_info_ptr->speed = atof(start);

    return true;
}

void gps_get_info(gps_info_t * info_ptr, int type)
{
    memset(info_ptr, 0, sizeof(*info_ptr));

    do {
        get_sentence(gps_buffer, 128); 
    } while (sentence_type(gps_buffer) != type);

    switch (type) {
    case GPS_TYPE_GPRMC:
        parse_gprcm(gps_buffer, info_ptr);
        break;
    case GPS_TYPE_GPGGA:
        parse_gpgga(gps_buffer, info_ptr);
        break;
    }
}

uint16_t gps_flash_dump()
{
    gps_log_dump_len = 0;
    char * gps_buffer_position = gps_buffer;

    uint16_t num_sentences = 0;
    uint8_t bytes_read; 

    // used in for loops
    int i = 0;
    int j = 0;

    // send dump message
    gps_send_msg(PMTK_LOCUS_GET_FLASH);

    // keeps track of sentence size
    uint16_t multiplier = 1;

    //Read in byte from rx_buff and place in buffer (This eats through the buffer until start of dump)
    do {
        bytes_read = get_sentence(gps_buffer, MAX_SENTENCE_LEN);
    } while (gps_buffer[5] != 'L' || gps_buffer[6] != 'O' ||
             gps_buffer[7] != 'X' || gps_buffer[8] != ',' ||
             gps_buffer[9] != '0');

    gps_buffer_position += bytes_read;

    // TODO use atoi
    //Read in the num sentences that will be sent
    for (i = 0; gps_buffer[i+11] != '*' ;i++)
    {
        multiplier = multiplier*10;
    }
    for (j = 0; j < i ;j++)
    {
        multiplier = multiplier/10;
        num_sentences += multiplier*(((int)gps_buffer[j+11])-48);
    }
    // -----

    // Get the dump data
    uint16_t k; 
    for (k = 0; k < num_sentences; ++k) {
        // Sanity check
        if ((gps_buffer_position + MAX_SENTENCE_LEN) >=
            (gps_buffer + LOG_DUMP_LEN)) {
            // We don't have any more room!
            gps_log_dump_len = gps_buffer_position - gps_buffer;
            return k + 1; 
        }

        // Read in sentence
        bytes_read = get_sentence(gps_buffer_position, MAX_SENTENCE_LEN);
        gps_buffer_position += bytes_read;
    }

    // Get the terminating sentence
    bytes_read = get_sentence(gps_buffer_position, MAX_SENTENCE_LEN);
    gps_buffer_position += bytes_read;

    gps_log_dump_len = gps_buffer_position - gps_buffer;

    return num_sentences + 2;
}

uint8_t gps_get_log_dump_bytes(uint8_t * buf, uint8_t num_bytes_desired)
{
    static uint32_t bytes_sent = 0; 
    uint32_t bytes_remaining = gps_log_dump_len - bytes_sent; 
    uint8_t * gps_buffer_position = gps_buffer + bytes_sent;
    
    if (bytes_remaining == 0) {
        bytes_sent = 0; // Reset after whole log transfered
        return 0;
    } else if (bytes_remaining < num_bytes_desired) {
        memcpy(buf, gps_buffer_position, bytes_remaining); 
        bytes_sent += bytes_remaining;
        return bytes_remaining;
    } else {
        memcpy(buf, gps_buffer_position, num_bytes_desired); 
        bytes_sent += num_bytes_desired;
        return num_bytes_desired;
    }
}
