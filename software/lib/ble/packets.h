/*
 *
 */

#ifndef PACKETS_H
#define PACKETS_H

#include <stdint.h>
#include <stdbool.h>


#define PACKET_BUF_LEN                      20

#define PACKET_TYPE_INVALID                 0x00

#define PACKET_TYPE_REQUEST_PED_STEP_COUNT  0x01
#define PACKET_TYPE_REQUEST_GPS_DATA        0x02
#define PACKET_TYPE_REQUEST_GPS_LOG         0x03
#define PACKET_TYPE_REQUEST_BATTERY_LEVEL   0x04
#define PACKET_TYPE_REQUEST_HEART_RATE      0x05

#define PACKET_TYPE_REPLY_PED_STEP_COUNT    0x01
#define PACKET_TYPE_REPLY_GPS_LATITUDE      0x02
#define PACKET_TYPE_REPLY_GPS_LONGITUDE     0x03
#define PACKET_TYPE_REPLY_GPS_SPEED         0x04
#define PACKET_TYPE_REPLY_GPS_LOG           0x05
#define PACKET_TYPE_REPLY_BATTERY_LEVEL     0x06
#define PACKET_TYPE_REPLY_HEART_RATE        0x07


/**
 *
 */
extern uint8_t packets_decode_request_packet(
    uint8_t packet[PACKET_BUF_LEN],
    uint8_t len);

/**
 *
 */
extern bool packets_build_reply_packet(
    uint8_t packet[PACKET_BUF_LEN],
    uint8_t type,
    void * data,
    uint8_t data_len,
    bool terminal_packet); 

#endif
