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

#include <stdint.h>

#include "charData.h"

const uint8_t battery[9*5*2] = {
	// 0/4
	0b00000000, 0b00000000,
	0b01111111, 0b11111000,
	0b01100000, 0b00000100,
	0b01100000, 0b00000110,
	0b01100000, 0b00000110,
	0b01100000, 0b00000110,
	0b01100000, 0b00000100,
	0b01111111, 0b11111000,
	0b00000000, 0b00000000,

	// 1/4
	0b00000000, 0b00000000,
	0b01111111, 0b11111000,
	0b01111100, 0b00000100,
	0b01111100, 0b00000110,
	0b01111100, 0b00000110,
	0b01111100, 0b00000110,
	0b01111100, 0b00000100,
	0b01111111, 0b11111000,
	0b00000000, 0b00000000,

	// 2/4
	0b00000000, 0b00000000,
	0b01111111, 0b11111000,
	0b01111111, 0b00000100,
	0b01111111, 0b00000110,
	0b01111111, 0b00000110,
	0b01111111, 0b00000110,
	0b01111111, 0b00000100,
	0b01111111, 0b11111000,
	0b00000000, 0b00000000,

	// 3/4
	0b00000000, 0b00000000,
	0b01111111, 0b11111000,
	0b01111111, 0b11000100,
	0b01111111, 0b11000110,
	0b01111111, 0b11000110,
	0b01111111, 0b11000110,
	0b01111111, 0b11000100,
	0b01111111, 0b11111000,
	0b00000000, 0b00000000,

	// 4/4
	0b00000000, 0b00000000,
	0b01111111, 0b11111000,
	0b01111111, 0b11111100,
	0b01111111, 0b11111110,
	0b01111111, 0b11111110,
	0b01111111, 0b11111110,
	0b01111111, 0b11111100,
	0b01111111, 0b11111000,
	0b00000000, 0b00000000,
};

const uint8_t bignums[19*2*12-19] = {
	//	0
	0b00001111, 0b11110000,
	0b00011111, 0b11111000,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00011111, 0b11111000,
	0b00001111, 0b11110000,

	//	1
	0b00000001, 0b10000000,
	0b00000111, 0b10000000,
	0b00001111, 0b10000000,
	0b00011101, 0b10000000,
	0b00011001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00111111, 0b11111100,
	0b00111111, 0b11111100,
	
	//	2
	0b00011111, 0b11110000,
	0b00111111, 0b11111000,
	0b00110000, 0b00011100,
	0b00110000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00011100,
	0b00000000, 0b00111000,
	0b00000000, 0b01110000,
	0b00000000, 0b11100000,
	0b00000001, 0b11000000,
	0b00000011, 0b10000000,
	0b00000111, 0b00000000,
	0b00001110, 0b00000000,
	0b00011100, 0b00000000,
	0b00111000, 0b00000000,
	0b00110000, 0b00000000,
	0b00110000, 0b00000000,
	0b00111111, 0b11111100,
	0b00111111, 0b11111100,
	
	//	3
	0b00001111, 0b11110000,
	0b00011111, 0b11111000,
	0b00111000, 0b00011100,
	0b00110000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00111000,
	0b00000001, 0b11110000,
	0b00000000, 0b00111000,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00110000, 0b00001100,
	0b00111000, 0b00011100,
	0b00011111, 0b11111000,
	0b00001111, 0b11110000,
	
	//	4
	0b00000000, 0b00111100,
	0b00000000, 0b01101100,
	0b00000000, 0b11001100,
	0b00000001, 0b10001100,
	0b00000011, 0b00001100,
	0b00000110, 0b00001100,
	0b00001100, 0b00001100,
	0b00011000, 0b00001100,
	0b00110000, 0b00001100,
	0b00111111, 0b11111100,
	0b00111111, 0b11111100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	
	//	5
	0b00111111, 0b11111100,
	0b00111111, 0b11111100,
	0b00110000, 0b00000000,
	0b00110000, 0b00000000,
	0b00110000, 0b00000000,
	0b00110000, 0b00000000,
	0b00110000, 0b00000000,
	0b00111111, 0b11100000,
	0b00111111, 0b11111000,
	0b00000000, 0b00011100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00110000, 0b00011100,
	0b00111000, 0b00111000,
	0b00011111, 0b11110000,
	0b00001111, 0b11100000,
	
	//	6
	0b00000111, 0b11110000,
	0b00011111, 0b11110000,
	0b00111000, 0b00000000,
	0b00110000, 0b00000000,
	0b00110000, 0b00000000,
	0b00110000, 0b00000000,
	0b00110000, 0b00000000,
	0b00110000, 0b00000000,
	0b00110000, 0b00000000,
	0b00110111, 0b11110000,
	0b00111111, 0b11111000,
	0b00111000, 0b00011100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00111000, 0b00011100,
	0b00011111, 0b11111000,
	0b00001111, 0b11110000,
	
	//	7
	0b00111111, 0b11111100,
	0b00111111, 0b11111100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00011100,
	0b00000000, 0b00111000,
	0b00000000, 0b01110000,
	0b00000000, 0b11100000,
	0b00000001, 0b11000000,
	0b00000011, 0b10000000,
	0b00000111, 0b00000000,
	0b00001110, 0b00000000,
	0b00011100, 0b00000000,
	0b00011000, 0b00000000,
	0b00110000, 0b00000000,
	0b00110000, 0b00000000,
	0b00110000, 0b00000000,
	
	//	8
	0b00001111, 0b11110000,
	0b00011111, 0b11111000,
	0b00111000, 0b00011100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00011100, 0b00111000,
	0b00001111, 0b11110000,
	0b00011100, 0b00111000,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00111000, 0b00011100,
	0b00011111, 0b11111000,
	0b00001111, 0b11110000,
	
	//	9
	0b00000111, 0b11110000,
	0b00011111, 0b11111000,
	0b00111000, 0b00011100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00110000, 0b00001100,
	0b00111000, 0b00011100,
	0b00011111, 0b11111100,
	0b00001111, 0b11101100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,
	0b00000000, 0b00001100,

	//	:
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000011, 0b11000000,
	0b00000011, 0b11000000,
	0b00000011, 0b11000000,
	0b00000011, 0b11000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000011, 0b11000000,
	0b00000011, 0b11000000,
	0b00000011, 0b11000000,
	0b00000011, 0b11000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	0b00000000, 0b00000000,
	
	//	:
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00111100,
	0b00111100,
	0b00111100,
	0b00111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00111100,
	0b00111100,
	0b00111100,
	0b00111100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
};

const uint8_t smallnums[9*11] = {
	//	0
	0b00111100,
	0b01000010,
	0b01001010,
	0b01001010,
	0b01011010,
	0b01010010,
	0b01010010,
	0b01000010,
	0b00111100,

	//	1
	0b00001000,
	0b00011000,
	0b00101000,
	0b00101000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00111110,

	//	2
	0b00111100,
	0b01000010,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b01111110,

	//	3
	0b01111100,
	0b00000010,
	0b00000010,
	0b00000010,
	0b00111100,
	0b00000010,
	0b00000010,
	0b00000010,
	0b01111100,

	//	4
	0b00000110,
	0b00001010,
	0b00010010,
	0b00100010,
	0b01000010,
	0b01111110,
	0b00000010,
	0b00000010,
	0b00000010,

	//	5
	0b01111110,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01111100,
	0b00000010,
	0b00000010,
	0b01000010,
	0b00111100,

	//	6
	0b00111100,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01011100,
	0b01100010,
	0b01000010,
	0b01000010,
	0b00111100,

	//	7
	0b01111110,
	0b00000010,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b01000000,

	//	8
	0b00111100,
	0b01000010,
	0b01000010,
	0b01000010,
	0b00111100,
	0b01000010,
	0b01000010,
	0b01000010,
	0b00111100,

	//	9
	0b00111100,
	0b01000010,
	0b01000010,
	0b01000010,
	0b00111110,
	0b00000010,
	0b00000010,
	0b00000100,
	0b00011000,

	//	:
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,

};

const uint8_t chars[9*31] = {
	//	A
	0b00011000,
	0b00100100,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01111110,
	0b01000010,
	0b01000010,
	0b01000010,
	
	//	B
	0b01111100,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01111100,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01111100,
	//	C
	0b00111100,
	0b01000010,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000010,
	0b00111100,
	//	D
	0b01111100,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01111100,
	//	E
	0b01111110,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01111110,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01111110,
	//	F
	0b01111110,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01111100,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000000,
	//	G
	0b00111100,
	0b01000010,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01001110,
	0b01000010,
	0b01000010,
	0b00111100,
	//	H
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01111110,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	//	I
	0b01111100,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b01111100,
	//	J
	0b01111110,
	0b00000100,
	0b00000100,
	0b00000100,
	0b00000100,
	0b00000100,
	0b00000100,
	0b01000100,
	0b00111000,
	//	K
	0b01000010,
	0b01000100,
	0b01001000,
	0b01010000,
	0b01100000,
	0b01010000,
	0b01001000,
	0b01000100,
	0b01000010,
	//	L
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01111110,
	//	M
	0b01000100,
	0b10101010,
	0b10101010,
	0b10010010,
	0b10000010,
	0b10000010,
	0b10000010,
	0b10000010,
	0b10000010,
	//	N
	0b01000010,
	0b01100010,
	0b01010010,
	0b01010010,
	0b01001010,
	0b01001010,
	0b01001010,
	0b01000110,
	0b01000010,
	//	O
	0b00111100,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b00111100,
	//	P
	0b01111100,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01111100,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000000,
	//	Q
	0b00111100,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01001010,
	0b01000100,
	0b00111010,
	//	R
	0b01111100,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01111100,
	0b01010000,
	0b01001000,
	0b01000100,
	0b01000010,
	//	S
	0b00111110,
	0b01000000,
	0b01000000,
	0b01000000,
	0b00111100,
	0b00000010,
	0b00000010,
	0b00000010,
	0b01111100,
	//	T
	0b00111110,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00001000,
	//	U
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b00111100,
	//	V
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b00100100,
	0b00011000,
	//	W
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01011010,
	0b01011010,
	0b00100100,
	//	X
	0b01000010,
	0b01000010,
	0b01000010,
	0b00100100,
	0b00011000,
	0b00100100,
	0b01000010,
	0b01000010,
	0b01000010,
	//	Y
	0b01000010,
	0b01000010,
	0b01000010,
	0b00100100,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	//	Z
	0b01111110,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b01111110,
	//	:
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	0b00000000,
	//	.
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00000000,
	//	/
	0b00000010,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b01000000,
	//	BLE Logo
	0b01001100,
	0b00101010,
	0b00011001,
	0b00001010,
	0b00001100,
	0b00001010,
	0b00011001,
	0b00101010,
	0b01001100,
	//	.
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
};
