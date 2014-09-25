//
//  Bitboard.h
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 11/23/13.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#ifndef __Shallow_Cyan__Bitboard__
#define __Shallow_Cyan__Bitboard__

#include <iostream>

// This program uses Bitboards as the primary board representation,
// and uses the Little Endian (LE) coordinate system (a1 = 0, h8 = 63).
typedef unsigned long long cBitboard;


// Function "popCount" returns the number of bits set to 1 in a given bitboard.
template <class B>
int popCount (B x) {

    int count = 0;

    while (x) {  
        count++;
        x &= x - 1; 
    }

    return count;

}


// Function "checkBit" returns the state of the specified bit in a given variable.
// If the specified bit is a negative number, it functions as an untriggerable value without creating an error.
template <class B>
inline bool checkBit(B x, int bit) {

    return bit < 0 ? false : (x & (1LLU<<bit));

}


// Function "setBit0" turns the specified bit off.
template <class B>
inline void setBit0 (B &x, int bit) {

    x &= ~(1LLU<<bit);

}


// Function "setBit1" turns the specified bit on.
template <class B>
inline void setBit1 (B &x, int bit) {

    x |= 1LLU<<bit;

}


// Array "index64" is used for the bitscanForward function.
const int index64[64] = {           
    0, 47,  1, 56, 48, 27,  2, 60,
    57, 49, 41, 37, 28, 16,  3, 61,
    54, 58, 35, 52, 50, 42, 21, 44,
    38, 32, 29, 23, 17, 11,  4, 62,
    46, 55, 26, 59, 40, 36, 15, 53,
    34, 51, 20, 43, 31, 22, 10, 45,
    25, 39, 14, 33, 19, 30,  9, 24,
    13, 18,  8, 12,  7,  6,  5, 63
};

// Function "bitScanForward" returns the coordinate of the least significant bit set in a bitboard.
inline int bitScanForward(cBitboard B) {

    return index64[((B ^ (B-1)) * 0x03f79d71b4cb0a89) >> 58];

}


// Function "shift" allows me to << with negative numbers automatically.
template <class B>
inline B shift(B &x, int y) {   

    return y<0 ? (x >> -y) : (x << y);

}


#endif /* defined(__Shallow_Cyan__Bitboard__) */
