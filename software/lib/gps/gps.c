/*
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "nrf_gpio.h"

#include "gps.h"
#include "uart_adapter.h"

// FIXME we really don't want to have the gps module be dependent on the pins
// used for uart_init... Find a way to pass in the uart_adapter stuff
#include "blue_dev_board.h"
//#include "green_dev_board.h"

#define MAX_SENTENCE_LEN 128


//static uint32_t enable;
static char gps_buffer[MAX_SENTENCE_LEN];

//void gps_init(uint32_t pin_enable)
void gps_init()
{
    //enable = pin_enable; 

    // FIXME we really don't want to have the gps module be dependent on the pins
    // used for uart_init... Find a way to pass in the uart_adapter stuff
    uart_adapter_init(PIN_RXD, PIN_TXD, PIN_RTS, PIN_CTS);
    gps_disable();
}

void gps_enable()
{
    //nrf_gpio_pin_set(enable);
    gps_send_msg(PMTK_AWAKE);
}

void gps_disable()
{
    //nrf_gpio_pin_clear(enable);
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

static void get_sentence(char * buf, uint8_t buf_len)
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
