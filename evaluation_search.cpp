//
//  evaluation_search.cpp
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 4/8/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#include "Evaluation_Search.h"


// Function "eval" is the evaluation function for this program.
// It takes in a board object and the side to evaluate for (1 for white, -1 for black).
// Currently, it only assesses material and piece locations.
int eval(cBoard &board, int color) {

    int evaluation = 0,  pieceValues[5] = {100, 320, 330, 500, 900};

    // -- Material -- \\

    for (int i = 0; i < 5; ++i)
            evaluation += (popCount(board.Bitboards[i]) - popCount(board.Bitboards[i + 6])) * pieceValues[i];


    // -- Square Values -- \\

    int pieceSQValues [7][64] = {
        { 0,  0,  0,  0,  0,  0,  0,  0,        // Pawns
            50, 50, 50, 50, 50, 50, 50, 50,
            10, 10, 20, 30, 30, 20, 10, 10,
            5,  5, 10, 25, 25, 10,  5,  5,
            0,  0,  0, 25, 25,  0,  0,  0,
            5, -5,-10,  -2, -2,-10, -5,  5,
            5, 10, 10,-20,-20, 10, 10,  5,
            0,  0,  0,  0,  0,  0,  0,  0
        },
        {                                   // Knights
            -50,-40,-30,-30,-30,-30,-40,-50,
            -40,-20,  0,  0,  0,  0,-20,-40,
            -30,  0, 5, 15, 15, 5,  0,-30,
            -30,  5, 15, 20, 20, 15,  5,-30,
            -30,  0, 15, 20, 20, 15,  0,-30,
            -30,  5, 5, 15, 15, 5,  5,-30,
            -40,-20,  0,  3,  3,  0,-20,-40,
            -50,-40,-30,-30,-30,-30,-40,-50,
        },
        {                                       // Bishops
            -20,-10,-10,-10,-10,-10,-10,-20,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -10,  0,  5, 10, 10,  5,  0,-10,
            -10,  5,  5, 10, 10,  5,  5,-10,
            -10,  0, 12, 10, 10, 12,  0,-10,
            -10, 10, 10, 5, 5, 10, 10,-10,
            -10,  5,  0,  0,  0,  0,  5,-10,
            -20,-10,-10,-10,-10,-10,-10,-20,
        },
        {                                       // Rooks
            0,  0,  0,  0,  0,  0,  0,  0,
            5, 10, 10, 10, 10, 10, 10,  5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            0,  0,  0,  5,  5,  0,  0,  0
        },
        {                                       // Queens
            -20,-10,-10, -5, -5,-10,-10,-20,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -10,  0,  5,  5,  5,  5,  0,-10,
            -5,  0,  5,  5,  5,  5,  0, -5,
            0,  0,  5,  5,  5,  5,  0, -5,
            -10,  5,  5,  5,  5,  5,  0,-10,
            -10,  0,  5,  0,  0,  0,  0,-10,
            -20,-10,-10, -5, -5,-10,-10,-20
        },
        {                                       // Middle game King
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -20,-30,-30,-40,-40,-30,-30,-20,
            -10,-20,-20,-20,-20,-20,-20,-10,
            20, 20,  0,  0,  0,  0, 20, 20,
            20, 30, 10,  0,  0, 10, 30, 20
        },
        {                                       // End game King
            -50,-40,-30,-20,-20,-30,-40,-50,
            -30,-20,-10,  0,  0,-10,-20,-30,
            -30,-10, 20, 30, 30, 20,-10,-30,
            -30,-10, 30, 40, 40, 30,-10,-30,
            -30,-10, 30, 40, 40, 30,-10,-30,
            -30,-10, 20, 30, 30, 20,-10,-30,
            -30,-30,  0,  0,  0,  0,-30,-30,
            -50,-30,-30,-30,-30,-30,-30,-50
        }
    };

    for (int i(0); i < 12; ++i) {

        cBitboard pieceLoc = board.Bitboards[i];

        while (pieceLoc) {

            int singlePieceLoc = bitScanForward(pieceLoc);
            pieceLoc &= pieceLoc - 1;

            if (i < 6) {    // White eval
                singlePieceLoc = 63 - singlePieceLoc;

                if (i == 5) {
                    evaluation += (popCount(board.Bitboards[10] == 0)) ? pieceSQValues[6][singlePieceLoc] : pieceSQValues[5][singlePieceLoc];
                    // Uses the endgame square values if the enemy's queen is gone
                }
                else
                    evaluation += pieceSQValues[i][singlePieceLoc];
            }

            else {      // Black eval
                if (i == 11) {
                    evaluation -= (popCount(board.Bitboards[4] == 0)) ? pieceSQValues[6][singlePieceLoc] : pieceSQValues[5][singlePieceLoc];
                }
                else
                    evaluation -= pieceSQValues[i-6][singlePieceLoc];
            }

        }

    }

   return color == 1 ? evaluation : -evaluation;

}


// Function "roughMoveStrength" is used to quickly and roughly decide
// which of two possible moves is more likely to be good.
// It is used in the search functions to facilitate good move ordering
// for the alpha-beta algorithm.
bool roughMoveStrength (cMove &a, cMove &b) {

    int score = a.getFlags() - b.getFlags();   
    score += (a.getPiece() < b.getPiece()) ? 2 : -2;

    return (score > 0);   
}


// Function "quiescentSearch" is called at every node for evaluation.
// By checking possible captures and recaptures, it prevents the horizon effect,
// checking for static evaluation only once no more captures exist.
int quiescentSearch (cBoard &board, int alpha, int beta, int color) {
    
    int standPat = eval(board, color);
    
    if (standPat >= beta)
        return beta;
    if (alpha < standPat)
        alpha = standPat;

    bool currentPlayer = board.move_CastlingRights & 0x1;
    
    int colorOffset = currentPlayer ? 0 : 6;

    cBitboard enemyPieces = board.Bitboards[6-colorOffset] | board.Bitboards[7-colorOffset] | board.Bitboards[8-colorOffset] | board.Bitboards[9-colorOffset] | board.Bitboards[10-colorOffset] | board.Bitboards[11-colorOffset],
    friendlyPieces = board.Bitboards[colorOffset] | board.Bitboards[1+colorOffset] | board.Bitboards[2+colorOffset] | board.Bitboards[3+colorOffset] | board.Bitboards[4+colorOffset] | board.Bitboards[5+colorOffset],
    
    occupied = enemyPieces | friendlyPieces;
    
    std::vector<cMove> possibleMoves, possibleCaptures;

    
    if ((moveGen::pawnLeftAtt(board.Bitboards[colorOffset], currentPlayer) | moveGen::pawnRightAtt(board.Bitboards[colorOffset], currentPlayer)) & enemyPieces)
        genPawnMoves(board.Bitboards[colorOffset], occupied, enemyPieces, currentPlayer, board.epSquare, possibleMoves);

    if (moveGen::allKnight(board.Bitboards[1+colorOffset]) & enemyPieces)
        genKnightMoves(board.Bitboards[1+colorOffset], friendlyPieces, enemyPieces, currentPlayer, possibleMoves);

    if (moveGen::allBishop(board.Bitboards[2+colorOffset], occupied) & enemyPieces)
        genBishopMoves(board.Bitboards[2+colorOffset], friendlyPieces, occupied, currentPlayer, possibleMoves);

    if (moveGen::allRook(board.Bitboards[3+colorOffset], occupied) & enemyPieces)
        genRookMoves(board.Bitboards[3+colorOffset], friendlyPieces, occupied, currentPlayer, possibleMoves);

    if (moveGen::allQueen(board.Bitboards[4+colorOffset], occupied) & enemyPieces)
        genQueenMoves(board.Bitboards[4+colorOffset], friendlyPieces, occupied, currentPlayer, possibleMoves);

    if (moveGen::king(board.Bitboards[5+colorOffset]) & enemyPieces) {
        cBitboard enemyAttacks = board.genAttacks(!currentPlayer);
        genKingMoves(board.Bitboards[5+colorOffset], friendlyPieces, enemyPieces, enemyAttacks, currentPlayer, (board.move_CastlingRights >> 1) & 0xF, possibleMoves);
    }
    
    for (int i(0); i < possibleMoves.size(); ++i)
        if (possibleMoves[i].getFlags() >= 8)
            possibleCaptures.push_back(possibleMoves[i]);
    
    if (possibleCaptures.size() == 0)
        return eval(board, color);

    std::sort(possibleCaptures.begin(), possibleCaptures.end(), roughMoveStrength);
    
    
    int bestValue = -std::numeric_limits<int>::max(),
    previousEPSquare = board.epSquare,
    previousMove_CastlingRights = board.move_CastlingRights,
    prevFiftyMove = board.fiftyMoveDraw;

    
    for (int i(0); i < possibleCaptures.size(); i++) {

        possibleCaptures[i].setCapturedPiece(board.currentPos[possibleCaptures[i].getTo()]);
        
        board.move(possibleCaptures[i]);
        
        if (board.inCheck(!(board.move_CastlingRights & 0x1))) {
            board.unMove(possibleCaptures[i], previousMove_CastlingRights, previousEPSquare, prevFiftyMove);
            continue;
        }
        
        
        int nodeScore = board.checkDrawRep() ? 0 : -quiescentSearch(board, -beta, -alpha, -color);
        
        board.unMove(possibleCaptures[i], previousMove_CastlingRights, previousEPSquare, prevFiftyMove);
        
        bestValue = std::max(bestValue, nodeScore);
        
        alpha = std::max(alpha, nodeScore);
        
        if (alpha >= beta)
            break;
        
    }
    
    if (bestValue == -std::numeric_limits<int>::max())
        return eval(board, color);
    
    return bestValue;

}


// Function "negamaxAB" is the main search algorithm used in this program.
// It is the normal negamax algorithm with alpha beta cut offs.
// The "endTime" argument is used for iterative deepening, and the function will
// cut off the search once that time has passed.
int negamaxAB (cBoard &board, int depth, int alpha, int beta, int color, time_t &endTime) {
    
    if (depth <= 0)
        return quiescentSearch(board, alpha, beta, color);
    
    
    char previousMove_CastlingRights = board.move_CastlingRights, prevFiftyMove = board.fiftyMoveDraw;
    
    int bestValue = -std::numeric_limits<int>::max(), previousEPSquare = board.epSquare;
    
    
    std::vector<cMove> possibleMoves = genLegalMoves(board);
    
    if (possibleMoves.size() == 0)
        return (board.inCheck(bool(color==1 ? 1 : 0))) ? bestValue+(100-depth) : 0;
        // The '(100-depth)' makes it so that sooner mates are chosen, e.g. mates in 1 are chosen over mates in 6.
    
    std::sort(possibleMoves.begin(), possibleMoves.end(), roughMoveStrength);

    
    for (int i(0); i < possibleMoves.size(); i++) {

        if (endTime != 0) {
         if (difftime(endTime, time(0)) < 0)
             return (bestValue == -std::numeric_limits<int>::max()) ? 20000 : bestValue;
        }
        
        board.move(possibleMoves[i]);
        
        int nodeScore = board.checkDrawRep() ? 0 : -negamaxAB(board, depth-1, -beta, -alpha, -color, endTime);
        
        board.unMove(possibleMoves[i], previousMove_CastlingRights, previousEPSquare, prevFiftyMove);
        
        bestValue = (bestValue > nodeScore) ? bestValue : nodeScore;
        
        alpha = (alpha > nodeScore) ? alpha : nodeScore;
        
        if (alpha >= beta)
            break;
        
    }
    
    return bestValue;
    
}


// Function "topNegamaxAB" is the way a search is started.
// This function returns a cMove object, not just a score for a particular move,
// unlike the function "negamaxAB."
cMove topNegamaxAB(cBoard board, int depth, time_t &endTime, cMove topMove) {
    
    char previousMove_CastlingRights = board.move_CastlingRights, prevFiftyMove = board.fiftyMoveDraw;
    
    int bestValue = -std::numeric_limits<int>::max(), previousEPSquare = board.epSquare, color = checkBit(board.move_CastlingRights, 0) ? 1 : -1;
    
    std::vector<cMove> possibleMoves = genLegalMoves(board);

    std::sort (possibleMoves.begin(), possibleMoves.end(), roughMoveStrength);

    if (topMove.getFrom() != topMove.getTo()) {

        for (int i(1); i < possibleMoves.size(); ++i)
            if (possibleMoves[i].getFrom() == topMove.getFrom() && possibleMoves[i].getTo() == topMove.getTo()) {
                std::move (possibleMoves.begin()+i, possibleMoves.begin()+i+1, possibleMoves.begin());
                break;
            }
    }

    
    cMove bestMove;
    
    int nodeScore;
    
    for (int i(0); i < possibleMoves.size(); i++) {

        if (endTime != 0) {
        time_t currentTime;
        time(&currentTime);
        if (difftime(endTime, currentTime) < 0)
            return cMove();
        }
        
        board.move(possibleMoves[i]);

        nodeScore = board.checkDrawRep() ? 0 : -negamaxAB(board, depth-1, -std::numeric_limits<int>::max(), -bestValue, -color, endTime);

        board.unMove(possibleMoves[i], previousMove_CastlingRights, previousEPSquare, prevFiftyMove);

        
        if (nodeScore > bestValue) {
            bestMove = possibleMoves[i];
            bestValue = nodeScore;
        }

    }

    return bestMove;
    
}
