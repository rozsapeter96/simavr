/*
	hd44780_cgrom.c

	Copyright 2020 Akos Kiss <akiss@inf.u-szeged.hu>

 	This file is part of simavr.

	simavr is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	simavr is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#define HD44780_CHAR_WIDTH 5
#define HD44780_CHAR_HEIGHT 8
#define HD44780_CHAR_NUM 256

// ROM Code A00
static const uint8_t hd44780_cgrom[HD44780_CHAR_NUM][HD44780_CHAR_HEIGHT] = {
  [0b00100000] = { // ' '
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  [0b00100001] = { // '!'
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
    0b00000,
    0b00100,
    0b00000,
  },
  [0b00100010] = { // '"'
    0b01010,
    0b01010,
    0b01010,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  [0b00100011] = { // '#'
    0b01010,
    0b01010,
    0b11111,
    0b01010,
    0b11111,
    0b01010,
    0b01010,
    0b00000,
  },
  [0b00100100] = { // '$'
    0b00100,
    0b01111,
    0b10100,
    0b01110,
    0b00101,
    0b11110,
    0b00100,
    0b00000,
  },
  [0b00100101] = { // '%'
    0b11000,
    0b11001,
    0b00010,
    0b00100,
    0b01000,
    0b10011,
    0b00011,
    0b00000,
  },
  [0b00100110] = { // '&'
    0b01100,
    0b10010,
    0b10100,
    0b01000,
    0b10101,
    0b10010,
    0b01101,
    0b00000,
  },
  [0b00100111] = { // '\''
    0b01100,
    0b00100,
    0b01000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  [0b00101000] = { // '('
    0b00010,
    0b00100,
    0b01000,
    0b01000,
    0b01000,
    0b00100,
    0b00010,
    0b00000,
  },
  [0b00101001] = { // ')'
    0b01000,
    0b00100,
    0b00010,
    0b00010,
    0b00010,
    0b00100,
    0b01000,
    0b00000,
  },
  [0b00101010] = { // '*'
    0b00000,
    0b00100,
    0b10101,
    0b01110,
    0b10101,
    0b00100,
    0b00000,
    0b00000,
  },
  [0b00101011] = { // '+'
    0b00000,
    0b00100,
    0b00100,
    0b11111,
    0b00100,
    0b00100,
    0b00000,
    0b00000,
  },
  [0b00101100] = { // ','
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b01100,
    0b00100,
    0b01000,
    0b00000,
  },
  [0b00101101] = { // '-'
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  [0b00101110] = { // '.'
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b01100,
    0b01100,
    0b00000,
  },
  [0b00101111] = { // '/'
    0b00000,
    0b00001,
    0b00010,
    0b00100,
    0b01000,
    0b10000,
    0b00000,
    0b00000,
  },

  [0b00110000] = { // '0'
    0b01110,
    0b10001,
    0b10011,
    0b10101,
    0b11001,
    0b10001,
    0b01110,
    0b00000,
  },
  [0b00110001] = { // '1'
    0b00100,
    0b01100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b01110,
    0b00000,
  },
  [0b00110010] = { // '2'
    0b01110,
    0b10001,
    0b00001,
    0b00010,
    0b00100,
    0b01000,
    0b11111,
    0b00000,
  },
  [0b00110011] = { // '3'
    0b11111,
    0b00010,
    0b00100,
    0b00010,
    0b00001,
    0b10001,
    0b01110,
    0b00000,
  },
  [0b00110100] = { // '4'
    0b00010,
    0b00110,
    0b01010,
    0b10010,
    0b11111,
    0b00010,
    0b00010,
    0b00000,
  },
  [0b00110101] = { // '5'
    0b11111,
    0b10000,
    0b11110,
    0b00001,
    0b00001,
    0b10001,
    0b01110,
    0b00000,
  },
  [0b00110110] = { // '6'
    0b00110,
    0b01000,
    0b10000,
    0b11110,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
  },
  [0b00110111] = { // '7'
    0b11111,
    0b10001,
    0b00001,
    0b00010,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
  },
  [0b00111000] = { // '8'
    0b01110,
    0b10001,
    0b10001,
    0b01110,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
  },
  [0b00111001] = { // '9'
    0b01110,
    0b10001,
    0b10001,
    0b01111,
    0b00001,
    0b00010,
    0b01100,
    0b00000,
  },
  [0b00111010] = { // ':'
    0b00000,
    0b01100,
    0b01100,
    0b00000,
    0b01100,
    0b01100,
    0b00000,
    0b00000,
  },
  [0b00111011] = { // ';'
    0b00000,
    0b01100,
    0b01100,
    0b00000,
    0b01100,
    0b00100,
    0b01000,
    0b00000,
  },
  [0b00111100] = { // '<'
    0b00010,
    0b00100,
    0b01000,
    0b10000,
    0b01000,
    0b00100,
    0b00010,
    0b00000,
  },
  [0b00111101] = { // '='
    0b00000,
    0b00000,
    0b11111,
    0b00000,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
  },
  [0b00111110] = { // '>'
    0b10000,
    0b01000,
    0b00100,
    0b00010,
    0b00100,
    0b01000,
    0b10000,
    0b00000,
  },
  [0b00111111] = { // '?'
    0b01110,
    0b10001,
    0b00001,
    0b00010,
    0b00100,
    0b00000,
    0b00100,
    0b00000,
  },

  [0b01000000] = { // '@'
    0b01110,
    0b10001,
    0b00001,
    0b01101,
    0b10101,
    0b10101,
    0b01110,
    0b00000,
  },
  [0b01000001] = { // 'A'
    0b01110,
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    0b10001,
    0b10001,
    0b00000,
  },
  [0b01000010] = { // 'B'
    0b11110,
    0b10001,
    0b10001,
    0b11110,
    0b10001,
    0b10001,
    0b11110,
    0b00000,
  },
  [0b01000011] = { // 'C'
    0b01110,
    0b10001,
    0b10000,
    0b10000,
    0b10000,
    0b10001,
    0b01110,
    0b00000,
  },
  [0b01000100] = { // 'D'
    0b11100,
    0b10010,
    0b10001,
    0b10001,
    0b10001,
    0b10010,
    0b11100,
    0b00000,
  },
  [0b01000101] = { // 'E'
    0b11111,
    0b10000,
    0b10000,
    0b11110,
    0b10000,
    0b10000,
    0b11111,
    0b00000,
  },
  [0b01000110] = { // 'F'
    0b11111,
    0b10000,
    0b10000,
    0b11110,
    0b10000,
    0b10000,
    0b10000,
    0b00000,
  },
  [0b01000111] = { // 'G'
    0b01110,
    0b10001,
    0b10000,
    0b10111,
    0b10001,
    0b10001,
    0b01111,
    0b00000,
  },

  [0b01001000] = { // 'H'
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    0b10001,
    0b10001,
    0b10001,
    0b00000,
  },
  [0b01001001] = { // 'I'
    0b01110,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b01110,
    0b00000,
  },
  [0b01001010] = { // 'J'
    0b00111,
    0b00010,
    0b00010,
    0b00010,
    0b00010,
    0b10010,
    0b01100,
    0b00000,
  },
  [0b01001011] = { // 'K'
    0b10001,
    0b10010,
    0b10100,
    0b11000,
    0b10100,
    0b10010,
    0b10001,
    0b00000,
  },
  [0b01001100] = { // 'L'
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11111,
    0b00000,
  },
  [0b01001101] = { // 'M'
    0b10001,
    0b11011,
    0b10101,
    0b10101,
    0b10001,
    0b10001,
    0b10001,
    0b00000,
  },
  [0b01001110] = { // 'N'
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  [0b01001111] = { // 'O'
    0b01110,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
  },

  [0b01010000] = { // 'P'
    0b11110,
    0b10001,
    0b10001,
    0b11110,
    0b10000,
    0b10000,
    0b10000,
    0b00000,
  },
  [0b01010001] = { // 'Q'
    0b01110,
    0b10001,
    0b10001,
    0b10001,
    0b10101,
    0b10010,
    0b01101,
    0b00000,
  },
  [0b01010010] = { // 'R'
    0b11110,
    0b10001,
    0b10001,
    0b11110,
    0b10100,
    0b10010,
    0b10001,
    0b00000,
  },
  [0b01010011] = { // 'S'
    0b01111,
    0b10000,
    0b10000,
    0b01110,
    0b00001,
    0b00001,
    0b11110,
    0b00000,
  },
  [0b01010100] = { // 'T'
    0b11111,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
  },
  [0b01010101] = { // 'U'
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
  },
  [0b01010110] = { // 'V'
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b01010,
    0b00100,
    0b00000,
  },
  [0b01010111] = { // 'W'
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10101,
    0b10101,
    0b01010,
    0b00000,
  },
  [0b01011000] = { // 'X'
    0b10001,
    0b10001,
    0b01010,
    0b00100,
    0b01010,
    0b10001,
    0b10001,
    0b00000,
  },
  [0b01011001] = { // 'Y'
    0b10001,
    0b10001,
    0b10001,
    0b01010,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
  },
  [0b01011010] = { // 'Z'
    0b11111,
    0b00001,
    0b00010,
    0b00100,
    0b01000,
    0b10000,
    0b11111,
    0b00000,
  },
  [0b01011011] = { // '['
    0b11100,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11100,
    0b00000,
  },
  [0b01011100] = { // Yen
    0b10001,
    0b01010,
    0b11111,
    0b00100,
    0b11111,
    0b00100,
    0b00100,
    0b00000,
  },
  [0b01011101] = { // ']'
    0b01110,
    0b00010,
    0b00010,
    0b00010,
    0b00010,
    0b00010,
    0b01110,
    0b00000,
  },
  [0b01011110] = { // '^'
    0b00100,
    0b01010,
    0b10001,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  [0b01011111] = { // '_'
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b00000,
  },

  [0b01100000] = { // '`'
    0b01000,
    0b00100,
    0b00010,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  [0b01100001] = { // 'a'
    0b00000,
    0b00000,
    0b01110,
    0b00001,
    0b01111,
    0b10001,
    0b01111,
    0b00000,
  },
  [0b01100010] = { // 'b'
    0b10000,
    0b10000,
    0b10110,
    0b11001,
    0b10001,
    0b10001,
    0b11110,
    0b00000,
  },
  [0b01100011] = { // 'c'
    0b00000,
    0b00000,
    0b01110,
    0b10000,
    0b10000,
    0b10001,
    0b01110,
    0b00000,
  },
  [0b01100100] = { // 'd'
    0b00001,
    0b00001,
    0b01101,
    0b10011,
    0b10001,
    0b10001,
    0b01111,
    0b00000,
  },
  [0b01100101] = { // 'e'
    0b00000,
    0b00000,
    0b01110,
    0b10001,
    0b11111,
    0b10000,
    0b01110,
    0b00000,
  },
  [0b01100110] = { // 'f'
    0b00110,
    0b01001,
    0b01000,
    0b11100,
    0b01000,
    0b01000,
    0b01000,
    0b00000,
  },
  [0b01100111] = { // 'g'
    0b00000,
    0b01111,
    0b10001,
    0b10001,
    0b01111,
    0b00001,
    0b01110,
    0b00000,
  },
  [0b01101000] = { // 'h'
    0b10000,
    0b10000,
    0b10110,
    0b11001,
    0b10001,
    0b10001,
    0b10001,
    0b00000,
  },
  [0b01101001] = { // 'i'
    0b00100,
    0b00000,
    0b01100,
    0b00100,
    0b00100,
    0b00100,
    0b01110,
    0b00000,
  },
  [0b01101010] = { // 'j'
    0b00010,
    0b00000,
    0b00110,
    0b00010,
    0b00010,
    0b10010,
    0b01100,
    0b00000,
  },
  [0b01101011] = { // 'k'
    0b10000,
    0b10000,
    0b10010,
    0b10100,
    0b11000,
    0b10100,
    0b10010,
    0b00000,
  },
  [0b01101100] = { // 'l'
    0b01100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b01110,
    0b00000,
  },
  [0b01101101] = { // 'm'
    0b00000,
    0b00000,
    0b11010,
    0b10101,
    0b10101,
    0b10001,
    0b10001,
    0b00000,
  },
  [0b01101110] = { // 'n'
    0b00000,
    0b00000,
    0b10110,
    0b11001,
    0b10001,
    0b10001,
    0b10001,
    0b00000,
  },
  [0b01101111] = { // 'o'
    0b00000,
    0b00000,
    0b01110,
    0b10001,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
  },

  [0b01110000] = { // 'p'
    0b00000,
    0b00000,
    0b11110,
    0b10001,
    0b11110,
    0b10000,
    0b10000,
    0b00000,
  },
  [0b01110001] = { // 'q'
    0b00000,
    0b00000,
    0b01101,
    0b10011,
    0b01111,
    0b00001,
    0b00001,
    0b00000,
  },
  [0b01110010] = { // 'r'
    0b00000,
    0b00000,
    0b10110,
    0b11001,
    0b10000,
    0b10000,
    0b10000,
    0b00000,
  },
  [0b01110011] = { // 's'
    0b00000,
    0b00000,
    0b01110,
    0b10000,
    0b01110,
    0b00001,
    0b11110,
    0b00000,
  },
  [0b01110100] = { // 't'
    0b01000,
    0b01000,
    0b11100,
    0b01000,
    0b01000,
    0b01001,
    0b00110,
    0b00000,
  },
  [0b01110101] = { // 'u'
    0b00000,
    0b00000,
    0b10001,
    0b10001,
    0b10001,
    0b10011,
    0b01101,
    0b00000,
  },
  [0b01110110] = { // 'v'
    0b00000,
    0b00000,
    0b10001,
    0b10001,
    0b10001,
    0b01010,
    0b00100,
    0b00000,
  },
  [0b01110111] = { // 'w'
    0b00000,
    0b00000,
    0b10001,
    0b10101,
    0b10101,
    0b10101,
    0b01010,
    0b00000,
  },
  [0b01111000] = { // 'x'
    0b00000,
    0b00000,
    0b10001,
    0b01010,
    0b00100,
    0b01010,
    0b10001,
    0b00000,
  },
  [0b01111001] = { // 'y'
    0b00000,
    0b00000,
    0b10001,
    0b10001,
    0b01111,
    0b00001,
    0b01110,
    0b00000,
  },
  [0b01111010] = { // 'z'
    0b00000,
    0b00000,
    0b11111,
    0b00010,
    0b00100,
    0b01000,
    0b11111,
    0b00000,
  },
  [0b01111011] = { // '{'
    0b00010,
    0b00100,
    0b00100,
    0b01000,
    0b00100,
    0b00100,
    0b00010,
    0b00000,
  },
  [0b01111100] = { // '|'
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
  },
  [0b01111101] = { // '}'
    0b01000,
    0b00100,
    0b00100,
    0b00010,
    0b00100,
    0b00100,
    0b01000,
    0b00000,
  },
  [0b01111110] = { // right arrow
    0b00000,
    0b00100,
    0b00010,
    0b11111,
    0b00010,
    0b00100,
    0b00000,
    0b00000,
  },
  [0b01111111] = { // left arrow
    0b00000,
    0b00100,
    0b01000,
    0b11111,
    0b01000,
    0b00100,
    0b00000,
    0b00000,
  },

  // TODO: [0b10100000...0b11011111]

  [0b11100000] = { // alpha
    0b00000,
    0b00000,
    0b01001,
    0b10101,
    0b10010,
    0b10010,
    0b01101,
    0b00000,
  },
  [0b11100001] = { // a:
    0b01010,
    0b00000,
    0b01110,
    0b00001,
    0b01111,
    0b10001,
    0b01111,
    0b00000,
  },
  [0b11100010] = { // beta (truncated from 5x10)
    0b00000,
    0b00000,
    0b01110,
    0b10001,
    0b11110,
    0b10001,
    0b11110,
    0b10000,
  },
  [0b11100011] = { // epsylon
    0b00000,
    0b00000,
    0b01110,
    0b10000,
    0b01100,
    0b10001,
    0b01110,
    0b00000,
  },
  [0b11100100] = { // mu (truncated from 5x10)
    0b00000,
    0b00000,
    0b10001,
    0b10001,
    0b10001,
    0b10011,
    0b11101,
    0b10000,
  },
  [0b11100101] = { // sigma
    0b00000,
    0b00000,
    0b01111,
    0b10100,
    0b10010,
    0b10001,
    0b01110,
    0b00000,
  },
  [0b11100110] = { // rho (truncated from 5x10)
    0b00000,
    0b00000,
    0b00110,
    0b01001,
    0b10001,
    0b10001,
    0b11110,
    0b10000,
  },
  [0b11100111] = { // g (truncated from 5x10)
    0b00000,
    0b00000,
    0b01111,
    0b10001,
    0b10001,
    0b10001,
    0b01111,
    0b00001,
  },
  [0b11101000] = { // square root
    0b00000,
    0b00000,
    0b00111,
    0b00100,
    0b00100,
    0b10100,
    0b01000,
    0b00000,
  },
  [0b11101001] = { // -1
    0b00000,
    0b00010,
    0b11010,
    0b00010,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  [0b11101010] = { // j (truncated from 5x10)
    0b00010,
    0b00000,
    0b00110,
    0b00010,
    0b00010,
    0b00010,
    0b00010,
    0b00010,
  },
  [0b11101011] = { //
    0b00000,
    0b10100,
    0b01000,
    0b10100,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  [0b11101100] = { // cent
    0b00000,
    0b00100,
    0b01110,
    0b10100,
    0b10101,
    0b01110,
    0b00100,
    0b00000,
  },
  [0b11101101] = { // Pound
    0b01000,
    0b01000,
    0b11100,
    0b01000,
    0b11100,
    0b01000,
    0b01111,
    0b00000,
  },
  [0b11101110] = { // n~
    0b01110,
    0b00000,
    0b10110,
    0b11001,
    0b10001,
    0b10001,
    0b10001,
    0b00000,
  },
  [0b11101111] = { // o:
    0b01010,
    0b00000,
    0b01110,
    0b10001,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
  },

  [0b11110000] = { // p (truncated from 5x10)
    0b00000,
    0b00000,
    0b10110,
    0b11001,
    0b10001,
    0b10001,
    0b11110,
    0b10000,
  },
  [0b11110001] = { // q (truncated from 5x10)
    0b00000,
    0b00000,
    0b01101,
    0b10011,
    0b10001,
    0b10001,
    0b01111,
    0b00001,
  },
  [0b11110010] = { // theta
    0b00000,
    0b01110,
    0b10001,
    0b11111,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
  },
  [0b11110011] = { // inf
    0b00000,
    0b00000,
    0b00000,
    0b01011,
    0b10101,
    0b11010,
    0b00000,
    0b00000,
  },
  [0b11110100] = { // Omega
    0b00000,
    0b00000,
    0b01110,
    0b10001,
    0b10001,
    0b01010,
    0b11011,
    0b00000,
  },
  [0b11110101] = { // U:
    0b01010,
    0b00000,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10011,
    0b01101,
  },
  [0b11110110] = { // Sigma
    0b11111,
    0b10000,
    0b01000,
    0b00100,
    0b01000,
    0b10000,
    0b11111,
    0b00000,
  },
  [0b11110111] = { // pi
    0b00000,
    0b00000,
    0b11111,
    0b01010,
    0b01010,
    0b01010,
    0b10011,
    0b00000,
  },
  [0b11111000] = { // x-
    0b11111,
    0b00000,
    0b10001,
    0b01010,
    0b00100,
    0b01010,
    0b10001,
    0b00000,
  },
  [0b11111001] = { // y (truncated from 5x10)
    0b00000,
    0b00000,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b01111,
    0b00001,
  },
  [0b11111010] = { //
    0b00000,
    0b00001,
    0b11110,
    0b00100,
    0b11111,
    0b00100,
    0b00100,
    0b00000,
  },
  [0b11111011] = { //
    0b00000,
    0b00000,
    0b11111,
    0b01000,
    0b01111,
    0b01001,
    0b10001,
    0b00000,
  },
  [0b11111100] = { //
    0b00000,
    0b00000,
    0b11111,
    0b10101,
    0b11111,
    0b10001,
    0b10001,
    0b00000,
  },
  [0b11111101] = { //
    0b00000,
    0b00100,
    0b00000,
    0b11111,
    0b00000,
    0b00100,
    0b00000,
    0b00000,
  },
  [0b11111110] = { // empty block
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  },
  [0b11111111] = { // full block (truncated from 5x10)
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
  },
};
