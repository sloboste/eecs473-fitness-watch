#include <stdint.h>

#include "packets.h"


uint8_t packets_decode_request_packet(
    uint8_t packet[PACKET_BUF_LEN],
    uint8_t len)
{
    return (0x7F & packet[0]); 
}

bool packets_build_reply_packet(
    uint8_t packet[PACKET_BUF_LEN],
    uint8_t type,
    void * data,
    uint8_t data_len,
    bool terminal_packet) 
{   
    if (data_len > 18) {
        return false;
    }

    packet[0] = ((terminal_packet ? 1 : 0) << 7) + type; 
    packet[1] = data_len;
    memcpy(&packet[2], data, data_len);

    return true;
}
