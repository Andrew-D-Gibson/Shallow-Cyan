//
//  legal_move_gen.h
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 4/21/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#ifndef __Longshot__Legal_Move_Gen__
#define __Longshot__Legal_Move_Gen__

#include "Board.h"


// The functions in this file generate the legal moves for a given set of pieces.
// Function specific explanations have been left.
// Note: Each function is given the vector their moves will be added to.

void genPawnMoves ( cBitboard &pawnLoc, cBitboard &occupied, cBitboard &enemy, bool color, int epSquare, std::vector<cMove> &possibleMoves );

void genKnightMoves ( cBitboard knightLoc, cBitboard &friendly, cBitboard &enemy, bool color, std::vector<cMove> &possibleMoves );

void genKingMoves ( cBitboard &kingBit, cBitboard &friendly, cBitboard &enemy, cBitboard &enemyAttacks, bool color, char castlingRights, std::vector<cMove> &possibleMoves );

void genRookMoves ( cBitboard rookLoc, cBitboard &friendly, cBitboard &occupied, bool color, std::vector<cMove> &possibleMoves, int piece = 3 );

void genBishopMoves ( cBitboard bishopLoc, cBitboard &friendly, cBitboard &occupied, bool color, std::vector<cMove> &possibleMoves, int piece = 2 );

void genQueenMoves ( cBitboard &queenLoc, cBitboard &friendly, cBitboard &occupied, bool color, std::vector<cMove> &possibleMoves ) ;


// Function "genLegalMoves" is given a board object
// and returns all legal moves from that position.
std::vector < cMove > genLegalMoves ( cBoard &board );


#endif /* defined(__Longshot__Legal_Move_Gen__) */
