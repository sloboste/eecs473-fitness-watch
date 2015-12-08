#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "nrf_gpio.h"

#include "gps.h"
#include "uart_adapter.h"

#define MAX_SENTENCE_LEN 130


static char gps_buffer[MAX_SENTENCE_LEN];

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

void get_sentence(char * buf, uint8_t buf_len)
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
    //return idx;
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

// uint16_t gps_flash_dump_partial(uint8_t * buf, uint8_t * buf_len)
// {
//     static uint16_t numSentences = 0;
//     static uint8_t sentencePos = 0;
//     static uint8_t sentenceSize = 0;
//     // Initiate flash dump
//     // Init packetByteCount to 0;

//     // used in for loops
//     int i = 0;
//     int j = 0;

//     // Checks to see if its mid dump, if not then begin dump
//     if (numSentences == 0)
//     {   
//         // send dump message
//         //nrf_gpio_pin_clear(PIN_LED_1);
//         gps_send_msg(PMTK_LOCUS_GET_FLASH);
//         // keeps track of sentence size
//         sentenceSize = 0;
//         uint16_t multiplier = 1;
//         //Read in byte from rx_buff and place in buffer (This eats through the buffer until start of dump)
//         sentenceSize = get_sentence(gps_buffer, 128);
//         while (gps_buffer[5] != 'L' || gps_buffer[6] != 'O' || gps_buffer[7] != 'X' || gps_buffer[8] != ',' || gps_buffer[9] != '0')
//         {
//             sentenceSize = get_sentence(gps_buffer, 128);
//         }
//         //Read in the num sentences that will be sent
//         for (i = 0; gps_buffer[i+12] != '*' ;i++)
//         {
//             multiplier = multiplier*10;
//         }
//         for (j = 0; j < i ;j++)
//         {
//             //multiplier = multiplier/10;
//             numSentences += multiplier*(((int)gps_buffer[j+11])-48);
//         }
//         // resets position in sentence
//         sentencePos = 0;
//         // sets end of dump flag to false
//         sentenceSize = get_sentence(gps_buffer, 128);

//     }


//     for (i = 0; i < *buf_len; i++)
//     {
//         // nrf_gpio_pin_toggle(PIN_LED_3);
//         // nrf_delay_ms(500);
//         if(sentencePos + i == sentenceSize)
//         {
//             // nrf_gpio_pin_toggle(PIN_LED_4);
//             // nrf_delay_ms(500);
//             numSentences--;
//             if (numSentences != 0)
//             {
//                 // get a new sentence
//                 sentenceSize = get_sentence(gps_buffer, 128);
//                 // -i to make up for +i a few lines below.. init to 0
//                 sentencePos = 0 - i;
//             }
//             else
//             {
//                 nrf_gpio_pin_toggle(PIN_LED_1);
//                 break;
//             }
//         }
//         else
//         {
//             // copy over buffer
//             buf[i] = gps_buffer[sentencePos+i];
//         }
//     }
//     *buf_len = i;
//     sentencePos += i;
//     return numSentences;
// }

uint16_t gps_flash_dump_partial(char * buf)
{
    uint16_t numSentences = 0;
    // Initiate flash dump
    // Init packetByteCount to 0;

    // used in for loops
    int i = 0;
    int j = 0;

    // send dump message
    //nrf_gpio_pin_clear(PIN_LED_1);
    //nrf_gpio_pin_toggle(PIN_LED_1);
    gps_send_msg(PMTK_LOCUS_GET_FLASH);
    //nrf_gpio_pin_toggle(PIN_LED_2);
    // keeps track of sentence size
    uint16_t multiplier = 1;
    //Read in byte from rx_buff and place in buffer (This eats through the buffer until start of dump)
    get_sentence(gps_buffer, 128);
    while (gps_buffer[5] != 'L' || gps_buffer[6] != 'O' || gps_buffer[7] != 'X' || gps_buffer[8] != ',' || gps_buffer[9] != '0')
    {
        get_sentence(gps_buffer, 128);
    }
    //Read in the num sentences that will be sent
    for (i = 0; gps_buffer[i+11] != '*' ;i++)
    {
        multiplier = multiplier*10;
    }
    for (j = 0; j < i ;j++)
    {
        multiplier = multiplier/10;
        numSentences += multiplier*(((int)gps_buffer[j+11])-48);
    }
    memcpy(buf, gps_buffer, 17);

    return (numSentences*8);
}
