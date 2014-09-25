//
//  pl_move_gen.h
//  Longshot
//
//  Created by Andrew D. Gibson on 3/13/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#ifndef __Shallow_Cyan__Pseudo_Legal_Move_Gen__
#define __Shallow_Cyan__Pseudo_Legal_Move_Gen__

#include <vector>
#include <cmath>

#include "Move.h"

// Utility Arrays
extern cBitboard fillUpAttacks[8][64];

extern cBitboard rotateRightBB[256];

extern cBitboard knightMovesFrom[64];

extern cBitboard kingMovesFrom[64];

extern cBitboard diagonalMask[64];

extern cBitboard antiDiagonalMask[64];


// The moveGen namespace holds all the functions for generating raw moves.
// The term raw moves delineates the fact that they are bitboards,
// not instances of the cMove class.
namespace moveGen {

    // Single Piece Move Generation
    cBitboard knight(int &knightSQ);
    
    cBitboard bishop(int &bishopSQ, cBitboard &occupied);
    
    cBitboard rook(int &rookSQ, cBitboard &occupied);

    
    // Piece Type Move Generation
    cBitboard pawnRightAtt(cBitboard &pawnLoc, bool color);
    
    cBitboard pawnLeftAtt(cBitboard &pawnLoc, bool color);
    
    cBitboard pawnForward(cBitboard &pawnLoc, cBitboard &occupied, bool color);
    
    cBitboard allKnight(cBitboard knightLoc);
    
    cBitboard allBishop(cBitboard bishopLoc, cBitboard &occupied);
    
    cBitboard allRook(cBitboard rookLoc, cBitboard &occupied);
    
    cBitboard allQueen(cBitboard queenLoc, cBitboard &occupied);

    cBitboard king(cBitboard &kingLoc);

}


#endif /* defined(__Shallow_Cyan__Pseudo_Legal_Move_Gen__) */
