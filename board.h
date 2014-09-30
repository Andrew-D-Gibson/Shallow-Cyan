//
//  Board.h
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 11/26/13.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#ifndef __Shallow_Cyan__Board__
#define __Shallow_Cyan__Board__

#include "pl_move_gen.h"

// -- UTILITY FUNCTIONS -- \\

template <typename T>
T StringToNumber ( const std::string &Text );

bool is_number(const std::string& s);


// -- ZOBRIST HASHING FUNCTIONS -- \\

// These bitboards hold their randomly generated hash, which are then all combined
// to create a hash of any given position.
static cBitboard zobristPiece[12][64],
zobristCastling[16],
zobristEnPassant[8],     // 1 for each file.  Combined w/ zobristBlackTurn can reassemble the EPSquare
zobristBlackTurn;

void genZobrist();


// Class "cBoard" is the complete board representation used in this program.  
// It contains all information needed to fully define a chess position.
struct cBoard {

    // Character "move_CastlingRights" stores 5 bool values, move (1=white), white king side castling, 
    // white queen side castling, black king side castling, and black queen side castling. There are 3 unused bits.
    char move_CastlingRights = 255, 

    // Character "fiftyMoveDraw" stores the number of moves since the last capture or pawn move.  
    fiftyMoveDraw = 0; 
    

    // Integer "epSquare" holds the current en passant target square, with -1 being no en passant square.
    int epSquare, 

    // Array "currentPos" stores a version of the board that is easier to draw and manipulate for certain tasks.
    // It's elements can be any number from -1 to 11, with -1 being unoccupied, 1 being a white pawn, 
    // 2 being a white knight, etc.
    currentPos[64];
    

    // Array "Bitboards" stores the twelve needed bitboards for storing piece locations.
    cBitboard Bitboards[12];

    
    // Vector "prevPosHashes" stores all of the previously encountered position's hashes.
    // It is used to detect draws by repetition.
    std::vector<cBitboard> prevPosHashes;
    

    // Board Functions
    cBoard();
    
    void reset();

    void clear();

    bool setBoard(std::string fen);

    std::string getFEN();

    void move( cMove &m );
    
    void unMove (cMove &m, char previousMove_CastlingRights, int previousEPSquare, char prevFiftyMove );
    
    cBitboard genAttacks ( bool color );

    bool inCheck( bool color );
        
    bool checkDrawRep();

};


#endif /* defined(__Shallow_Cyan__Board__) */
