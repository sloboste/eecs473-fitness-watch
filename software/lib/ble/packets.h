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
 * The request reply BLE packet structure for the Eir watch app. This uses the
 * watch_service for the transport mechanism. Well formed packets consist are
 * 1 to 20 bytes inclusive in length. A well formed packet will always begin
 * with a byte containing the PACKET_TYPE in the bits 0-6 and bit 7 as a flag
 * indicating when set that this packet is the last of the transmission
 * sequence. The next byte is the data length byte. The remaining bytes are the
 * data bytes.
 *
 * Data buffer (20 bytes)
 * +------------------------+-------------+-----------+ 
 * | Byte 0                 | Byte 1      | Byte 2-19 | 
 * +------------------------+-------------+-----------+
 * | bit 7: terminal packet | data length | data      | 
 * | bits 0-6: packet type  |             |           | 
 * +------------------------+-------------+-----------+
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
 * Decode the type of a request packet.
 * 
 * packet -- a buffer containing the request packet. 
 * len -- the length of the buffer.
 *
 * Returns the type of the request packet.
 */
extern uint8_t packets_decode_request_packet(
    uint8_t packet[PACKET_BUF_LEN],
    uint8_t len);

/**
 * Encode data into a request packet.
 * 
 * packet -- a buffer that will store the encoded packet.
 * data -- a buffer holding the data to be encoded. 
 * data_len -- then length of the data buffer.
 * terminal_packet -- is this the last packet in the transmission sequence?
 *
 * Returns true if the data was of valid length. Returns false otherwise.
 */
extern bool packets_build_reply_packet(
    uint8_t packet[PACKET_BUF_LEN],
    uint8_t type,
    void * data,
    uint8_t data_len,
    bool terminal_packet); 

#endif
