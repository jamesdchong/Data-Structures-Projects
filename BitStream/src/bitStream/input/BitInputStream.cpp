/**
 * Implementation file to fill a buffer of data from in, and read its bits
 *
 * Author: James Chong
 * Email: j2chong@ucsd.edu
 */
#include "BitInputStream.hpp"

#define ZERO_HEX 0x00
#define ONE_HEX 0x01
#define SIZE_BYTE 8

/* Fills a buffer with a byte of data from in */
void BitInputStream::fill() {
    // Read one byte into the buffer
    buf = in.get();

    // Reset counter
    nbits = 0;
}

/* Reads the bit at the nbits location of buf and returns 0 if the bit is 0, and
 * returns 1 if the bit is 1
 */
unsigned int BitInputStream::readBit() {
    // Fill buf with another byte of data if all bits have been read
    if (nbits == SIZE_BYTE) {
        fill();
    }

    // If the end of file is reached, return the EOF char
    if (in.eof()) {
        return EOF;
    }

    // Calculate the bit and the nbits location using bitwise operations
    byte bufByte = buf;
    byte bitValue = (bufByte >> SIZE_BYTE - 1 - nbits) & ONE_HEX;

    // Increment counter
    ++nbits;

    // Return 0 if bit is 0
    if (bitValue == ZERO_HEX) {
        return 0;
    }

    // Return 1 if bit is 1
    if (bitValue == ONE_HEX) {
        return 1;
    }
}