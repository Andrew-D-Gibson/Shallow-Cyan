//
//  Move.h
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 11/23/13.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#ifndef __Shallow_Cyan__Move__
#define __Shallow_Cyan__Move__

#include "Bitboard.h"

// Array "square" returns a string of the chess coordinates of a Little Endian coordinate.  e.g square[0]  = "a1", and square[63] = "h8"
extern std::string square[64];


// Enum states "moveFlags" are used to determine special actions to take in the move/unmove functions.
// They are specially assigned values, so that any flag involving a possible capture or promotion has its 4th bit set to 1. e.g 0001
enum moveFlags {
    KING = 0, QUIET = 1, DOUBLE_PAWN = 2, ROOK = 3, K_CASTLE = 4, Q_CASTLE = 5,     // Fourth bit is 0

    CAPTURE = 8, ROOK_CAPTURE = 9, KING_CAPTURE = 10, EP_CAPTURE = 11,               // Fourth bit is 1
    PROMO_N = 12, PROMO_B = 13, PROMO_R = 14, PROMO_Q = 15
};


// Class "cMove" is the way this program stores a chess move that can later be executed on the board.
struct cMove {
    
    // moveInfo stores all the required information in different parts of an int.  
    int moveInfo = 0;
    

    cMove(char piece, char from, char to, char flags);
    
    cMove();

    
    char getPiece();

    char getFrom();
    
    char getTo();

    int getFlags();
    
    char getCapturedPiece();
    

    void setCapturedPiece(char capturedPiece);

    
    void draw();
    
};


#endif /* defined(__Shallow_Cyan__Move__) */
