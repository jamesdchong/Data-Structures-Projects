/**
 * Header file that defines all public and private variables and methods to be
 * implemented in BitInputStream.cpp
 *
 * Author: James Chong
 * Email: j2chong@ucsd.edu
 */
#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>
#define ZERO_HEX 0x00
#define ONE_HEX 0x01
#define SIZE_BYTE 8

typedef unsigned char byte;

using namespace std;

/* Class that defines a BitInputStream that reads bits and fills a buffer with a
 * byte of data */
class BitInputStream {
  private:
    char buf;     // one byte buffer of bits
    int nbits;    // number of bits have been writen to buf
    istream& in;  // reference to the input stream to use

  public:
    /* Constructor that initializes a BitInputStream
     * is: istream to be used as a reference
     */
    explicit BitInputStream(istream& is) : in(is) {
        buf = ZERO_HEX;
        nbits = SIZE_BYTE;
    };

    /* Fills a buffer with a byte of data from in */
    void fill();

    /* Reads the bit at the nbits location of buf and returns 0 if the bit is 0,
     * and returns 1 if the bit is 1
     */
    unsigned int readBit();
};

#endif