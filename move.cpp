//
//  Move.cpp
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 11/23/13.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#include "Move.h"

std::string square[64] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};


// Usual constructor for a cMove.
// Takes in the neccessary information for a move and stores them in the 32 bits of an integer.
cMove::cMove(char piece, char from, char to, char flags) {
    
    moveInfo |= from | to << 6 | piece << 12 | flags << 16;
    
}

// A move initialized with this blank constructor cannot be executed on the board.
cMove::cMove() {}


char cMove::getFrom() {
    return moveInfo & 0x3F;
}

char cMove::getTo() {
    return (moveInfo >> 6) & 0x3F;
}

char cMove::getPiece() {
    return (moveInfo >> 12) & 0xF;
}

int cMove::getFlags() {
    return (moveInfo >> 16) & 0xF;
}

char cMove::getCapturedPiece() {
    return (moveInfo >> 20) & 0xF;
}


// The Captured Piece information is never available on initializaton,
// and is provided later if the move is a capture.
void cMove::setCapturedPiece(char capturedPiece) {
    moveInfo |= capturedPiece << 20;
}


// Useful for debugging and the log.
void cMove::draw() {
    std::cout << square[getFrom()] << "-" << square[getTo()];
}
