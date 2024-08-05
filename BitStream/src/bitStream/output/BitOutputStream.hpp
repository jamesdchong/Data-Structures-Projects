/**
 * Header file that defines all public and private variables and methods to be
 * implemented in BitOutputStream.cpp
 *
 * Author: James Chong
 * Email: j2chong@ucsd.edu
 */
#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>

#define ZERO_HEX 0x00

typedef unsigned char byte;

using namespace std;

/* Class that defines a BitOutputStream that writes bits to a buffer, and
 * flushes the buffer to ostream */
class BitOutputStream {
  private:
    char buf;      // one byte buffer of bits
    int nbits;     // number of bits have been writen to buf
    ostream& out;  // reference to the output stream to use

  public:
    /* Initializes a new BitOutputStream
     * os: ostream to be used as a reference
     */
    explicit BitOutputStream(ostream& os) : out(os) {
        buf = ZERO_HEX;
        nbits = 0;
    };

    /* Flush the buffer into the ostream */
    void flush();

    /* Writes the bit given to the buffer using bitwise operations
     * i: bit to be put into the buffer
     */
    void writeBit(int i);
};

#endif
