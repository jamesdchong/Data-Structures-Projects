/**
 * Implementation file to write bits into a buffer and flushes it to ostream
 *
 * Author: James Chon
 * Email: j2chong@ucsd.edu
 */
#include "BitOutputStream.hpp"

#define ZERO_HEX 0x00
#define ONE_HEX 0x01
#define SIZE_BYTE 8

/* Flush the buffer into the ostream */
void BitOutputStream::flush() {
    // Direct the buffer (1 byte) into the ostream
    out << buf;

    // Reset the buffer and counter
    buf = ZERO_HEX;
    nbits = 0;
}

/* Writes the bit given to the buffer using bitwise operations
 * i: bit to be put into the buffer
 */
void BitOutputStream::writeBit(int i) {
    // If bit buffer full, flush it
    if (nbits == SIZE_BYTE) {
        flush();
    }

    // Write the LSB of i into the buffer at the current index
    byte bufByte = buf;
    bufByte = bufByte & ~(ONE_HEX << SIZE_BYTE - 1 - nbits);
    buf = bufByte | (i << SIZE_BYTE - 1 - nbits);

    // Update nbits
    nbits++;
}

// More similar write functions for different sizes (byte, int)
