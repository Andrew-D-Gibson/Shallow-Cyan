//
//  legal_move_gen.cpp
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 4/21/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#include "legal_move_gen.h"

// -- LEGAL MOVE GEN -- \\

// Pawns
void pawnBitboardToMoves ( cBitboard rawMoves, int offset, int flags, int piece, std::vector<cMove> &possibleMoves ) {  // Takes a move bitboard and turns it into a vector of cMove's
    
    while (rawMoves) {
        
        int LS1B = bitScanForward(rawMoves);    // Returns the int location of the Lowest raw move bit
        
        if (LS1B < 8 || LS1B >= 56)  // Promo handling
            for (int i(0); i < 4; ++i)
                possibleMoves.push_back(cMove( piece, LS1B+offset, LS1B, 12+i));
        
        else
            possibleMoves.push_back(cMove( piece, LS1B+offset, LS1B, flags ));
        
        rawMoves &= rawMoves - 1;   // Reset LS1B
        
    }
    
}


void genPawnMoves ( cBitboard &pawnLoc, cBitboard &occupied, cBitboard &enemy, bool color, int epSquare, std::vector<cMove> &possibleMoves ) {
    
    cBitboard tempGen;      // Temporarily holds the generated move bitboards
    
    int piece = (color) ? 0 : 6, direction = 1 - (2*!color);
    
    std::vector<int> captureOffsets = {-9 + (!color * 16), -7 + (!color * 16)};
    
    
    // One move forward gen
    
    tempGen = shift(pawnLoc, 8 * direction);   // Shifts pawn Locations up 1 rank
    
    tempGen &= ~occupied;   // Checks if each forward location is not occupied
    
    pawnBitboardToMoves(tempGen, -8 * direction, QUIET, piece, possibleMoves);
    
    
    // Two move forward gen
    
    if (color)
        tempGen &= 0xFF0000;
    else
        tempGen &= 0xFF0000000000;
    
    tempGen = shift(tempGen, 8 * direction);
    
    tempGen &= ~occupied;
    
    pawnBitboardToMoves(tempGen, -16 * direction, DOUBLE_PAWN, piece, possibleMoves);
    
    
    // Right Capture gen
    
    tempGen = pawnLoc & 0x7F7F7F7F7F7F7F7F;
    
    tempGen = shift(tempGen, (8 * direction)+1);
    
    if (checkBit(tempGen, epSquare))    // En Passant handling
        possibleMoves.push_back(cMove(piece, epSquare + captureOffsets[0], epSquare, EP_CAPTURE));
    
    tempGen &= enemy;
    
    pawnBitboardToMoves(tempGen, captureOffsets[0], CAPTURE, piece, possibleMoves);
    
    
    // Left Capture gen
    
    tempGen = pawnLoc & 0xFEFEFEFEFEFEFEFE;
    
    tempGen = shift(tempGen, (8 * direction)-1);
    
    if (checkBit(tempGen, epSquare)) // En Passant handling
        possibleMoves.push_back(cMove(piece, epSquare + captureOffsets[1], epSquare, EP_CAPTURE));
    
    tempGen &= enemy;
    
    pawnBitboardToMoves(tempGen, captureOffsets[1], CAPTURE, piece, possibleMoves);
    
}


// Knights
void genKnightMoves ( cBitboard knightLoc, cBitboard &friendly, cBitboard &enemy, bool color, std::vector<cMove> &possibleMoves ) {
    
    int piece = color ? 1 : 7;
    
    while (knightLoc) {
        
        int LS1B = bitScanForward(knightLoc);
        
        knightLoc &= knightLoc - 1; // Resets LS1B
        
        cBitboard tempMoves = knightMovesFrom[LS1B];
        
        tempMoves &= ~friendly;
        
        
        while (tempMoves) {
            
            int tempLS1B = bitScanForward(tempMoves);    // Returns the int location of the Lowest raw move bit.
            
            int flags = checkBit(enemy, tempLS1B) ? CAPTURE : QUIET;
            
            possibleMoves.push_back(cMove( piece, LS1B, tempLS1B, flags ));
            
            tempMoves &= tempMoves - 1;     // Reset LS1B
            
        }
        
    }
    
}


// Kings
void genKingMoves ( cBitboard &kingBit, cBitboard &friendly, cBitboard &enemy, cBitboard &enemyAttacks, bool color, char castlingRights, std::vector<cMove> &possibleMoves ) {
    
    int piece = color ? 5 : 11, LS1B, kingLoc = bitScanForward(kingBit);
    
    cBitboard rawMoves = kingMovesFrom[kingLoc];
    
    rawMoves &= ~friendly;
    
    while (rawMoves) {
        
        LS1B = bitScanForward(rawMoves);
        
        int flags = checkBit(enemy, LS1B) ? KING_CAPTURE : KING;
        
        possibleMoves.push_back(cMove( piece, kingLoc, LS1B, flags ));
        
        rawMoves &= rawMoves - 1;     // Reset LS1B
        
    }
    
    cBitboard occupied = friendly | enemy;
    
    if (  ( (castlingRights & 0x2 && color) || (castlingRights & 0x8 && !color) )  && !checkBit(occupied, kingLoc+1) && !checkBit(occupied, kingLoc+2))
        if (!checkBit(enemyAttacks, kingLoc) && !checkBit(enemyAttacks, kingLoc+1) && !checkBit(enemyAttacks, kingLoc+2))
            possibleMoves.push_back(cMove(piece, kingLoc, kingLoc+2, K_CASTLE));
    
    if ( ( (castlingRights & 0x4 && color) || (castlingRights & 0x10 && !color) ) && !checkBit(occupied, kingLoc-1) && !checkBit(occupied, kingLoc-2) && !checkBit(occupied, kingLoc-3))
        if (!checkBit(enemyAttacks, kingLoc) && !checkBit(enemyAttacks, kingLoc-1) && !checkBit(enemyAttacks, kingLoc-2))
            possibleMoves.push_back(cMove(piece, kingLoc, kingLoc-2, Q_CASTLE));
    
}


// Rooks
void genRookMoves ( cBitboard rookLoc, cBitboard &friendly, cBitboard &occupied, bool color, std::vector<cMove> &possibleMoves, int piece ) {
    
    bool isRook = (piece == 3) ? 1 : 0;
    
    cBitboard aFile = 0x0101010101010101;
    
    if (color != 1 && isRook)
        piece = 9;
    
    
    while (rookLoc) {
        
        int rookSquare = bitScanForward(rookLoc),
        
        rank = rookSquare >> 3,
        
        column = rookSquare & 7;
        
        rookLoc &= rookLoc-1;
        
        
        // Rank moves
        cBitboard holder = (occupied >> ((rank*8)+1) ) & 0x3F;
        
        holder = (fillUpAttacks[column][holder] & 0xFF) << (rank*8);
        
        
        // File moves
        cBitboard rawMoves = aFile & ( occupied >> column );
        
        rawMoves = ((0x8040201008040201 * rawMoves ) >> 57) & 0x3f;
        
        rawMoves = fillUpAttacks[7-rank][rawMoves] & 0xFF;
        
        rawMoves = (rotateRightBB[rawMoves] & aFile) << column;
        
        
        // Combine
        rawMoves |= holder;
        
        rawMoves &= ~friendly;
        
        
        // Serialize Moves
        
        while (rawMoves) {
            
            int rookMoveSquare = bitScanForward(rawMoves),
            
            flags = checkBit(occupied & ~friendly, rookMoveSquare) ? (isRook ? ROOK_CAPTURE : CAPTURE) : (isRook ? ROOK : QUIET);
            
            possibleMoves.push_back(cMove( piece, rookSquare, rookMoveSquare, flags ));
            
            rawMoves &= rawMoves - 1;     // Reset LS1B
            
        }
        
        
    }
    
}


// Bishops
void genBishopMoves ( cBitboard bishopLoc, cBitboard &friendly, cBitboard &occupied, bool color, std::vector<cMove> &possibleMoves, int piece ) {
    
    bool isBishop = (piece == 2) ? 1 : 0;
    
    cBitboard bFile = 0x0202020202020202;
    
    if (color != 1 && isBishop)
        piece = 8;
    
    while (bishopLoc) {
        
        int bishopSquare = bitScanForward(bishopLoc);
        
        bishopLoc &= bishopLoc-1;
        
        int column = bishopSquare & 7;
        
        // Diagonal
        cBitboard moves(0),
        holder = (diagonalMask[bishopSquare] & occupied) * bFile >> 58;
        
        moves |= diagonalMask[bishopSquare] & fillUpAttacks[column][holder];
        
        
        
        // Anti Diagonal
        holder = (antiDiagonalMask[bishopSquare] & occupied) * bFile >> 58;
        
        moves |= antiDiagonalMask[bishopSquare] & fillUpAttacks[column][holder];
        
        
        moves &= ~friendly;
        
        
        // Serialize
        
        while (moves) {
            
            int bishopMoveSquare = bitScanForward(moves);
            
            int flags = checkBit(occupied & ~friendly, bishopMoveSquare) ? CAPTURE : QUIET;
            
            possibleMoves.push_back(cMove( piece, bishopSquare, bishopMoveSquare, flags ));
            
            moves &= moves - 1;     // Reset LS1B
            
            
        }
        
    }
    
}


// Queens
void genQueenMoves ( cBitboard &queenLoc, cBitboard &friendly, cBitboard &occupied, bool color, std::vector<cMove> &possibleMoves ) {
    
    int piece = color ? 4 : 10;
    
    genRookMoves(queenLoc, friendly, occupied, color, possibleMoves, piece);
    
    genBishopMoves(queenLoc, friendly, occupied, color, possibleMoves, piece);
    
}


std::vector < cMove > genLegalMoves ( cBoard &board ) {

    bool color = board.move_CastlingRights & 1;
    
    char previousMove_CastlingRights = board.move_CastlingRights, prevFiftyMove = board.fiftyMoveDraw;
    
    int friendlyOffset = color ? 0 : 6, previousEPSquare = board.epSquare;
    
    cBitboard friendly(0), enemy(0), occupied, enemyAttacks = board.genAttacks(color);
    
    std::vector < cMove > legalMoves, possibleMoves;
    
    
    for (int i(0); i < 6; ++i) {
        
        friendly |= board.Bitboards[i+friendlyOffset];
        
        enemy |= board.Bitboards[i+(6-friendlyOffset)];
        
    }
    
    occupied = friendly | enemy;
    
    
    genPawnMoves(board.Bitboards[friendlyOffset], occupied, enemy, color, board.epSquare, possibleMoves);
    
    genKnightMoves(board.Bitboards[1+friendlyOffset], friendly, enemy, color, possibleMoves);
    
    genBishopMoves(board.Bitboards[2+friendlyOffset], friendly, occupied, color, possibleMoves);
    
    genRookMoves(board.Bitboards[3+friendlyOffset], friendly, occupied, color, possibleMoves);
    
    genQueenMoves(board.Bitboards[4+friendlyOffset], friendly, occupied, color, possibleMoves);
    
    genKingMoves(board.Bitboards[5+friendlyOffset], friendly, enemy, enemyAttacks, color, board.move_CastlingRights, possibleMoves );
    
    
    for (int i(0); i < possibleMoves.size(); ++i) {
        
        if (possibleMoves[i].getFlags() & 0x8)
            possibleMoves[i].setCapturedPiece(board.currentPos[possibleMoves[i].getTo()]);
        
        board.move(possibleMoves[i]);
        
        if (!board.inCheck(color))
            legalMoves.push_back(possibleMoves[i]);
        
        board.unMove(possibleMoves[i], previousMove_CastlingRights, previousEPSquare, prevFiftyMove);
        
    }
    
    return legalMoves;
    
}
