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
