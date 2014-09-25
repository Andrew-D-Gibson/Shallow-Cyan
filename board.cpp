//
//  Board.cpp
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 11/26/13.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#include "Board.h"
#include "sstream"

// -- UTILITY FUNCTIONS -- \\

// Function "StringToNumber" converts a given string to a number if
// it can be done feasibly.  If not, it returns 0.
template <typename T>
T StringToNumber ( const std::string &Text ) {
    std::istringstream ss(Text);
    T result;
    return ss >> result ? result : 0;
}


// Function "is_number" determines if a string
// can be converted into a number.
// It is useful for the setBoard funtion.
bool is_number(const std::string& s) {

    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();

}


// Function "flipValues" switches the values of two
// variables without the need for a holder variable.
template <typename T>
void flipValues ( T* a, T* b ) {
    if (a != b) {
        *a ^= *b;
        *b ^= *a;
        *a ^= *b;
    }
}



// -- ZOBRIST HASHING FUNCTIONS -- \\

// Function "randBB" generates a randomly populated bitboard.
cBitboard randBB() {
    cBitboard a = rand();
    a <<= 32;
    return a | rand();    
}


// Function "genZobrist" generates the zobrist keys
// at the start of the program.  These keys are used to
// detect draw by repetitions, and are stored as global variables.
void genZobrist() {
    
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    
    for (int i(0); i < 12; ++i)
        for (int j(0); j < 64; ++j)
            zobristPiece[i][j] =  randBB();
    
    for (int i(0); i < 8; i++) {
        zobristEnPassant[i] = randBB();
        zobristCastling[2*i] = randBB();
        zobristCastling[(2*i)+1] = randBB();
    }
    
    zobristBlackTurn = randBB();

}



// -- BOARD FUNCTIONS -- \\

cBoard::cBoard() {
    reset();
}


// Function "reset" sets the board to the starting position.
void cBoard::reset() {

    // Variables
    epSquare = -1;              // Clears the en passant square

    move_CastlingRights = 255;  // Sets it to white's move and allows all castling rights

    fiftyMoveDraw = 0;


    // Bitboards
    Bitboards[0] = 0xFF00;      // White Pawns

    Bitboards[1] = 0x42;        // White Knights

    Bitboards[2] = 0x24;        // White Bishops

    Bitboards[3] = 0x81;        // White Rooks

    Bitboards[4] = 0x8;         // White Queens

    Bitboards[5] = 0x10;        // White King


    Bitboards[6] = 0xFF000000000000;    // Black Pawns

    Bitboards[7] = 0x4200000000000000;  // Black Knights

    Bitboards[8] = 0x2400000000000000;  // Black Bishops

    Bitboards[9] = 0x8100000000000000;  // Black Rooks

    Bitboards[10] = 0x800000000000000;  // Black Queens

    Bitboards[11] = 0x1000000000000000; // Black King


    // Zobrist Hashing
    prevPosHashes.clear();

    cBitboard currentHash = 0;

    int resetPos[8][8] = {
        {3, 1, 2, 4, 5, 2, 1, 3},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {6, 6, 6, 6, 6, 6, 6, 6},
        {9, 7, 8, 10, 11, 8, 7, 9}
    };

    for (int i(0); i < 8; ++i)

        for (int j(0); j < 8; ++j) {

            currentPos[(i*8)+j] = resetPos[i][j];

            if (resetPos[i][j] != -1)
                currentHash ^= zobristPiece[resetPos[i][j]][(i*8)+j];
        }

    currentHash ^= zobristCastling[0xf];

    prevPosHashes.push_back(currentHash);

}


// Function "clear" completely clears the board and all its stored information.
// It is useful for the "setBoard" function.
void cBoard::clear() {

    move_CastlingRights = 1;

    epSquare = -1;

    for (int i(0); i < 12; ++i)
        Bitboards[i] = 0;

    for (int i(0); i < 64; ++i)
        currentPos[i] = -1;

    prevPosHashes.clear();

}


// Function "setBoard" sets the board object to the given FEN string.
// It returns success or failure, and if it failed,
// the object must be restored to a previous state outside the function.
bool cBoard::setBoard(std::string fen) {
    // position, color, castlingRights, enPassant, halfMoves, fullMove
    std::vector<std::string> arguments;

    long previousSpaceLocation(0);

    clear();

    for (int i(0); i < 6; ++i) {
        long spaceLocation = fen.find(" ", previousSpaceLocation);

        arguments.push_back(fen.substr(previousSpaceLocation, spaceLocation - previousSpaceLocation));

        previousSpaceLocation = spaceLocation+1;

    }


    int curCol(0), curRank(7);

    clear();

    for (int i(0); i < arguments[0].size(); ++i) {

        if (arguments[0].substr(i,1) == "p") {
            setBit1(Bitboards[6], (curRank*8)+curCol);
            currentPos[(curRank*8)+curCol] = 6;
        }
        else if (arguments[0].substr(i,1) == "P") {
            setBit1(Bitboards[0], (curRank*8)+curCol);
            currentPos[(curRank*8)+curCol] = 0;
        }
        else if (arguments[0].substr(i,1) == "n") {
            setBit1(Bitboards[7], (curRank*8)+curCol);
            currentPos[(curRank*8)+curCol] = 7;
        }
        else if (arguments[0].substr(i,1) == "N") {
            setBit1(Bitboards[1], (curRank*8)+curCol);
            currentPos[(curRank*8)+curCol] = 1;
        }
        else if (arguments[0].substr(i,1) == "b") {
            setBit1(Bitboards[8], (curRank*8)+curCol);
            currentPos[(curRank*8)+curCol] = 8;
        }
        else if (arguments[0].substr(i,1) == "B") {
            setBit1(Bitboards[2], (curRank*8)+curCol);
            currentPos[(curRank*8)+curCol] = 2;
        }
        else if (arguments[0].substr(i,1) == "r") {
            setBit1(Bitboards[9], (curRank*8)+curCol);
            currentPos[(curRank*8)+curCol] = 9;
        }
        else if (arguments[0].substr(i,1) == "R") {
            setBit1(Bitboards[3], (curRank*8)+curCol);
            currentPos[(curRank*8)+curCol] = 3;
        }
        else if (arguments[0].substr(i,1) == "q") {
            setBit1(Bitboards[10], (curRank*8)+curCol);
            currentPos[(curRank*8)+curCol] = 10;
        }
        else if (arguments[0].substr(i,1) == "Q") {
            setBit1(Bitboards[4], (curRank*8)+curCol);
            currentPos[(curRank*8)+curCol] = 4;
        }
        else if (arguments[0].substr(i,1) == "k") {
            setBit1(Bitboards[11], (curRank*8)+curCol);
            currentPos[(curRank*8)+curCol] = 11;
        }
        else if (arguments[0].substr(i,1) == "K") {
            setBit1(Bitboards[5], (curRank*8)+curCol);
            currentPos[(curRank*8)+curCol] = 5;
        }
        else if (arguments[0].substr(i,1) == "/") {
            curCol = 0;
            curRank--;
            continue;
        }
        else if (is_number(arguments[0].substr(i,1))) {
            curCol += StringToNumber<int>(arguments[0].substr(i,1));
            continue;
        }

        else
            return 0;

        curCol++;

    }

    if (arguments[1] == "w")
        setBit1(move_CastlingRights, 0);
    else if (arguments[1] == "b")
        setBit0(move_CastlingRights, 0);
    else
        return 0;

    for (int i(0); i < arguments[2].size(); ++i) {

        if (arguments[2].substr(i,1) == "-")
            break;
        if (arguments[2].substr(i,1) == "K")
            setBit1(move_CastlingRights, 1);
        else if (arguments[2].substr(i,1) == "Q")
            setBit1(move_CastlingRights, 2);
        else if (arguments[2].substr(i,1) == "k")
            setBit1(move_CastlingRights, 3);
        else if (arguments[2].substr(i,1) == "q")
            setBit1(move_CastlingRights, 4);
        else
            return 0;

    }

    if (arguments[3] != "-") {
        bool validEPSquare(0);
        for (int i(16); i < 24; i++) {
            if (arguments[3] == square[i]) {
                epSquare = i;
                validEPSquare = 1;
            }
        }
        for (int i(40); i < 48; i++) {
            if (arguments[3] == square[i]) {
                epSquare = i;
                validEPSquare = 1;
            }
        }
        if (!validEPSquare)
            return 0;
    }

    if (arguments[4] != "") {
        if (is_number(arguments[4]))
            fiftyMoveDraw = StringToNumber<int>(arguments[4]);
        else
            return 0;
    }


    cBitboard currentHash(0);

    for (int i(0); i < 8; ++i)

        for (int j(0); j < 8; ++j)
            if (currentPos[(i*8)+j] != -1)
                currentHash ^= zobristPiece[currentPos[(i*8)+j]][(i*8)+j];

    currentHash ^= checkBit(move_CastlingRights, 1) ? 0 : zobristBlackTurn;

    currentHash ^= zobristCastling[(move_CastlingRights >> 1)&0xf];

    if (epSquare != -1)
        currentHash ^= zobristEnPassant[epSquare & 7];

    prevPosHashes.push_back(currentHash);

    return 1;

}


// Function "getFen" returns the current FEN string of the board.
std::string cBoard::getFEN() {

    int offsetHolder(0), rankPos(0);

    std::ostringstream fenPos;

    for (int j(7); j >= 0; --j) {

        for (int k(0); k < 8; k++) {

            int i = (j*8)+k;

        if (rankPos > 7) {
            if (offsetHolder != 0)
                fenPos << offsetHolder;
            fenPos << '/';
            offsetHolder = 0;
            rankPos = 0;
        }

        if (currentPos[i] == 6) {
            if (offsetHolder > 0)
                fenPos << offsetHolder;
            fenPos << 'p';
            rankPos++;
        }
        else if (currentPos[i] == 0) {
            if (offsetHolder > 0)
                fenPos << offsetHolder;
            fenPos << 'P';
            rankPos++;
        }
        else if (currentPos[i] == 7) {
            if (offsetHolder > 0)
                fenPos << offsetHolder;
            fenPos << 'n';
            rankPos++;
        }
        else if (currentPos[i] == 1) {
            if (offsetHolder > 0)
                fenPos << offsetHolder;
            fenPos << 'N';
            rankPos++;
        }
        else if (currentPos[i] == 8) {
            if (offsetHolder > 0)
                fenPos << offsetHolder;
            fenPos << 'b';
            rankPos++;
        }
        else if (currentPos[i] == 2) {
            if (offsetHolder > 0)
                fenPos << offsetHolder;
            fenPos << 'B';
            rankPos++;
        }
        else if (currentPos[i] == 9) {
            if (offsetHolder > 0)
                fenPos << offsetHolder;
            fenPos << 'r';
            rankPos++;
        }
        else if (currentPos[i] == 3) {
            if (offsetHolder > 0)
                fenPos << offsetHolder;
            fenPos << 'R';
            rankPos++;
        }
        else if (currentPos[i] == 10) {
            if (offsetHolder > 0)
                fenPos << offsetHolder;
            fenPos << 'q';
            rankPos++;
        }
        else if (currentPos[i] == 4) {
            if (offsetHolder > 0)
                fenPos << offsetHolder;
            fenPos << 'Q';
            rankPos++;
        }
        else if (currentPos[i] == 11) {
            if (offsetHolder > 0)
                fenPos << offsetHolder;
            fenPos << 'k';
            rankPos++;
        }
        else if (currentPos[i] == 5) {
            if (offsetHolder > 0)
                fenPos << offsetHolder;
            fenPos << 'K';
            rankPos++;
        }
        else if (currentPos[i] == -1) {
            offsetHolder++;
            rankPos++;
            continue;
        }

        offsetHolder = 0;

        }

    }

    fenPos << " ";

    if (move_CastlingRights & 1)
        fenPos << "w ";
    else
        fenPos << "b ";

    bool anyCastlingRights(0);

    if (move_CastlingRights & 2) {
        fenPos << "K";
        anyCastlingRights = 1;
    }
    if (move_CastlingRights & 4) {
        fenPos << "Q";
        anyCastlingRights = 1;
    }
    if (move_CastlingRights & 8) {
        fenPos << "k";
        anyCastlingRights = 1;
    }
    if (move_CastlingRights & 16) {
        fenPos << "q";
        anyCastlingRights = 1;
    }

    if (!anyCastlingRights)
        fenPos << "-";

    fenPos << " ";

    if (epSquare == -1)
        fenPos << "-";
    else
        fenPos << square[epSquare];

    fenPos << " ";

    if (fiftyMoveDraw == '0')
        fenPos << "0 ";
    else
        fenPos << static_cast<int>(fiftyMoveDraw) << " ";

    fenPos << prevPosHashes.size();

    return fenPos.str();

}


// Function "move" makes the specified move in the board object,
// handling all flags and information that needs to be changed.
void cBoard::move( cMove &m ) {

    char from = m.getFrom(), to = m.getTo(), piece = m.getPiece(), flags = m.getFlags();
    
    bool color = piece < 6;

    cBitboard hashPosition = prevPosHashes[prevPosHashes.size()-1];

    
    move_CastlingRights ^= 1;         // Flips whose turn it is
    
    hashPosition ^= zobristBlackTurn;
    
    
    setBit0(Bitboards[piece], from);
    
    setBit1(Bitboards[piece], to);
    
    currentPos[from] = -1;
    
    currentPos[to] = piece;
    
    hashPosition ^= zobristPiece[piece][from];
    
    hashPosition ^= zobristPiece[piece][to];
    
    
    hashPosition ^= zobristCastling[move_CastlingRights >> 1 & 0xF];    // Clears the castling rights of the previous move
    
    fiftyMoveDraw++;
    
    
    epSquare = -1;                           // Sets the en passant square to an untriggerable value.
    
    if (!(flags & 8)) {        // Quiet Moves
        
        if (flags == QUIET) {
            if (piece == 0 || piece == 6)
                fiftyMoveDraw = 0;
        }
        
        else if (flags == DOUBLE_PAWN) {
            epSquare = to - (8 * (1 - ( !color * 2)));  // Sets the en passant square for later move generation.
            hashPosition ^= zobristEnPassant[epSquare & 7];
            fiftyMoveDraw = 0;
        }
        
        else if (move_CastlingRights & (0x6 << (!color * 2))) {
            
            if (flags == ROOK) {
                if (color) {             // White rook
                    
                    if (from == 7)   // King side
                        move_CastlingRights &= ~0x2;
                    else if (from == 0)       // Queen side
                        move_CastlingRights &= ~0x4;
                    
                }
                else {                          // Black rook
                    
                    if (from == 63)           // King side
                        move_CastlingRights &= ~0x8;
                    else if (from == 56)      // Queen side
                        move_CastlingRights &= ~0x10;
                    
                }
            }
            
            else if (flags == KING) {
                
                if (color)
                    move_CastlingRights &= ~0x6;
                else
                    move_CastlingRights &= ~0x18;
                
            }
            
            else if (flags == K_CASTLE) {
                
                if (color) {
                    Bitboards[3] ^= 0xA0;
                    currentPos[7] = -1;
                    currentPos[5] = 3;
                    hashPosition ^= zobristPiece[3][7];
                    hashPosition ^= zobristPiece[3][5];
                    move_CastlingRights &= 0xF9;
                }
                
                else {
                    Bitboards[9] ^= 0xA000000000000000;
                    currentPos[63] = -1;
                    currentPos[61] = 9;
                    hashPosition ^= zobristPiece[9][63];
                    hashPosition ^= zobristPiece[9][61];
                    move_CastlingRights &= 0xE7;
                }
                
            }
            
            else if (flags == Q_CASTLE) {
                
                if (color) {
                    Bitboards[3] ^= 0x9;
                    currentPos[0] = -1;
                    currentPos[3] = 3;
                    hashPosition ^= zobristPiece[3][0];
                    hashPosition ^= zobristPiece[3][3];
                    move_CastlingRights &= 0xF9;
                }
                
                else {
                    Bitboards[9] ^= 0x900000000000000;
                    currentPos[56] = -1;
                    currentPos[59] = 9;
                    hashPosition ^= zobristPiece[9][56];
                    hashPosition ^= zobristPiece[9][59];
                    move_CastlingRights &= 0xE7;
                }
                
            }
            
        }
        
    }
    else {                      // Capture Moves
        
        if (flags == CAPTURE) {
            
            char capturedPiece = m.getCapturedPiece();
            
            setBit0(Bitboards[capturedPiece], to);
            
            hashPosition ^= zobristPiece[capturedPiece][to];
            
            if (to == 7)
                setBit0(move_CastlingRights, 1);
            else if (to == 0)
                setBit0(move_CastlingRights, 2);
            else if (to == 63)
                setBit0(move_CastlingRights, 3);
            else if (to == 56)
                setBit0(move_CastlingRights, 4);
            
            fiftyMoveDraw = 0;
            
        }
        
        else if (flags == ROOK_CAPTURE) {
            
            char capturedPiece = m.getCapturedPiece();
            
            setBit0(Bitboards[capturedPiece], to);
            
            hashPosition ^= zobristPiece[capturedPiece][to];
            
            if (to == 7)
                setBit0(move_CastlingRights, 1);
            else if (to == 0)
                setBit0(move_CastlingRights, 2);
            else if (to == 63)
                setBit0(move_CastlingRights, 3);
            else if (to == 56)
                setBit0(move_CastlingRights, 4);
            
            fiftyMoveDraw = 0;
            
            if (color) {             // White rook
                
                if (from == 7)            // King side
                    move_CastlingRights &= ~0x2;
                else if (from == 0)       // Queen side
                    move_CastlingRights &= ~0x4;
                
            }
            else {                          // Black rook
                
                if (from == 63)           // King side
                    move_CastlingRights &= ~0x8;
                else if (from == 56)      // Queen side
                    move_CastlingRights &= ~0x10;
                
            }
            
        }
        
        else if (flags == KING_CAPTURE) {
            
            char capturedPiece = m.getCapturedPiece();
            
            setBit0(Bitboards[capturedPiece], to);
            
            hashPosition ^= zobristPiece[capturedPiece][to];
            
            if (to == 7)
                setBit0(move_CastlingRights, 1);
            else if (to == 0)
                setBit0(move_CastlingRights, 2);
            else if (to == 63)
                setBit0(move_CastlingRights, 3);
            else if (to == 56)
                setBit0(move_CastlingRights, 4);
            
            fiftyMoveDraw = 0;
            
            if (color)
                move_CastlingRights &= ~0x6;
            else
                move_CastlingRights &= ~0x18;
            
        }
        
        else if (flags == EP_CAPTURE) {
            
            setBit0(Bitboards[6 - (!color * 6)], to - 8 + (16*!color));
            hashPosition ^= zobristPiece[6 - (!color * 6)][to];
            currentPos[(to-8) + (!color * 16)] = {-1};
            
            fiftyMoveDraw = 0;
            
        }
        
    }
    
    if (flags > 11) {   // Checks for a Promotion flag to cut down on unnecessary if statements
        
        setBit0(Bitboards[piece], to);
        
        hashPosition ^= zobristPiece[piece][to];
        
        int promoPiece = piece + (flags - 11);
        
        setBit1(Bitboards[promoPiece], to);    // Sets the to bit to the promotion piece.
        
        currentPos[to] = promoPiece;
        
        hashPosition ^= zobristPiece[promoPiece][to];
        
        fiftyMoveDraw = 0;
        
        char capturedPiece = m.getCapturedPiece();
        
        if (capturedPiece != 0) {
        
            setBit0(Bitboards[capturedPiece], to);
            
            hashPosition ^= zobristPiece[capturedPiece][to];
        
            if (to == 7)
                setBit0(move_CastlingRights, 1);
            else if (to == 0)
                setBit0(move_CastlingRights, 2);
            else if (to == 63)
                setBit0(move_CastlingRights, 3);
            else if (to == 56)
                setBit0(move_CastlingRights, 4);
        
        }
    
    }
    
    hashPosition ^= zobristCastling[move_CastlingRights >> 1 & 0xF];
    
    prevPosHashes.push_back(hashPosition);
    
}


// Function "unMove" undoes the specified move, resetting the board's information
// to its previous state based on the given arguments.
void cBoard::unMove(cMove &m, char previousMove_CastlingRights, int previousEPSquare, char prevFiftyMove ) {
    
    char from = m.getFrom(), to = m.getTo(), piece = m.getPiece(), flags = m.getFlags();
    
    bool color = piece < 6;

    setBit1(Bitboards[piece], from);
    
    setBit0(Bitboards[piece], to);
    
    currentPos[from] = piece;
    
    currentPos[to] = -1;
    
    prevPosHashes.pop_back();
    
    
    epSquare = previousEPSquare;
    
    move_CastlingRights = previousMove_CastlingRights;
    
    fiftyMoveDraw = prevFiftyMove;

    if (!(flags & 8)) {       // Quiet Moves
        
        if (flags == QUIET || flags == DOUBLE_PAWN || flags == ROOK || flags == KING) {}
        
        else if (flags == K_CASTLE) {
            
            if (color) {
                Bitboards[3] ^= 0xA0;
                currentPos[7] = 3;
                currentPos[5] = -1;
            }
            
            else {
                Bitboards[9] ^= 0xA000000000000000;
                currentPos[63] = 9;
                currentPos[61] = -1;
            }
            
        }
        
        else if (flags == Q_CASTLE) {
            
            if (color) {
                Bitboards[3] ^= 0x9;
                currentPos[0] = 3;
                currentPos[3] = -1;
            }
            
            else {
                Bitboards[9] ^= 0x900000000000000;
                currentPos[56] = 9;
                currentPos[59] = -1;
            }
            
        }
        
    }
    else  {                 // Capture Moves
        
        if (flags == CAPTURE || flags == ROOK_CAPTURE || flags == KING_CAPTURE) {
            
            char capturedPiece = m.getCapturedPiece();
            
            setBit1(Bitboards[capturedPiece], to);
            
            currentPos[to] = capturedPiece;
            
        }
        
        else if (flags == EP_CAPTURE) {
            
            setBit1(Bitboards[6 - (!color * 6)], to - 8 + (16*!color));
            currentPos[(to-8) + (!color * 16)] = 6 - (!color * 6);
            
        }
        
    }
    
    if (flags > 11)   {  // Promotion Moves

        setBit0(Bitboards[ piece + (flags - 11) ], to);

        char capturedPiece = m.getCapturedPiece();

        if (capturedPiece > 0 && capturedPiece < 11) {
            setBit1(Bitboards[capturedPiece], to);
            currentPos[to] = capturedPiece;
        }
    }

}


// Function "genAttacks" returns a bitboard of
// all of the squares attacked by a given color's pieces.
cBitboard cBoard::genAttacks ( bool color ) {
    
    int colorOffset = (color) ? 6 : 0;
    
    cBitboard occupied = Bitboards[0] | Bitboards[1] | Bitboards[2] | Bitboards[3] | Bitboards[4] | Bitboards[5] | Bitboards[6] | Bitboards[7] | Bitboards[8] | Bitboards[9] | Bitboards[10] | Bitboards[11];

    return moveGen::pawnRightAtt(Bitboards[colorOffset], !color) | moveGen::pawnLeftAtt(Bitboards[colorOffset], !color)
    | moveGen::allKnight(Bitboards[1+colorOffset])
    | moveGen::allBishop(Bitboards[2+colorOffset], occupied)
    | moveGen::allRook(Bitboards[3+colorOffset], occupied)
    | moveGen::allQueen(Bitboards[4+colorOffset], occupied)
    | moveGen::king(Bitboards[5+colorOffset]);

}


// Function "inCheck" returns whether or not a given color
// is in check.
bool cBoard::inCheck ( bool color ) {

    return (genAttacks(color) & Bitboards[color ? 5 : 11]);

}


// Function "checkDrawRep" returns whether or not a position
// has been repeated three times, and is therefore a draw.
bool cBoard::checkDrawRep() {
    
    int repCounter = 1;
    
    for (int i(2); i < fiftyMoveDraw + 1; ++i) {        // No need to search farther back than the last pawn move or capture, which rule out repetition.
        if (prevPosHashes[prevPosHashes.size()-1] == prevPosHashes[prevPosHashes.size() - i])
            repCounter++;
    }
    
    if (repCounter >= 3 || fiftyMoveDraw >= 50)
        return 1;
    
    return 0;
    
}
