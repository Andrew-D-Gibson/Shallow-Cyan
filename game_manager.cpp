//
//  game_manager.cpp
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 4/16/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#include "game_manager.h"
#include <sstream>

gameManager::gameManager() {
    genZobrist();
    board.reset();
}


// Function "makeMove" takes in an intended move,
// saves any information needed to undo the move,
// and then plays the move on the board.
void gameManager::makeMove(cMove &move) {
    moveHistory.prevMove_CastlingRights.push_back(board.move_CastlingRights);
    moveHistory.prevEpSquares.push_back(board.epSquare);
    moveHistory. prevFiftyMoves.push_back(board.fiftyMoveDraw);
    moveHistory.prevMoves.push_back(move);

    board.move(move);
}


// Function "move" makes the computer search for the 
// time defined by the variable "searchTime," and make the best move it finds.
void gameManager::move() {

    cMove tempMove;

    if (searchTime == 0)
        tempMove = iterativeSearch(board, 86400, 4);
    else
        tempMove = iterativeSearch(board, searchTime);
    
    makeMove(tempMove);

}


// Function "makePlayerMove" is given input from the user,
// and checks if a legal move matches that input.
// It returns 1 for success, 2 if it needs promotion information,
// and 0 if no legal move matches the input.
int gameManager::makePlayerMove(int from, int to, int promoFlag) {

    std::vector<cMove> possibleMoves = genLegalMoves(board);

    for (int i(0); i < possibleMoves.size(); ++i) {
        if (possibleMoves[i].getFrom() == from && possibleMoves[i].getTo() == to) { // Legal move match

            if (possibleMoves[i].getFlags() >= 12) {        // A promotion flag
                 if ( promoFlag == 0 && possibleMoves[i].getFlags() != promoFlag)
                 return 2;   // Needs more promotion information to identify the intended move.
            }

            makeMove(possibleMoves[i]);

            return 1;
        }
    }

    return 0;

}


// Function "checkGameState" is used to check for checkmates and 
// stalemates at the end of every computer and player move.
// It returns 0 for a normal state, -1 for a black win, 1 for a white win, and 5 for a draw.
int gameManager::checkGameState() {
    
    if (board.checkDrawRep())  // This call checks for draw by repetition.
        return 5;

    if (genLegalMoves(board).size() == 0) {

        bool currentPlayer = checkBit(board.move_CastlingRights, 0);

        if (board.inCheck(currentPlayer))
            return currentPlayer ? -1 : 1;
        else
            return 5;
    }

    return 0;

}


// Function "undo" undoes the last move played, using 
// the information stored in its vectors to replace the board's state.
// It returns its success.
bool gameManager::undo() {

    if (moveHistory.prevMoves.size() >= 1) {
           board.unMove(moveHistory.prevMoves[moveHistory.prevMoves.size()-1], moveHistory.prevMove_CastlingRights[moveHistory.prevMove_CastlingRights.size()-1], moveHistory.prevEpSquares[moveHistory.prevEpSquares.size()-1], moveHistory.prevFiftyMoves[moveHistory.prevFiftyMoves.size()-1]);
           moveHistory.prevMoves[moveHistory.prevMoves.size()-1].draw();
           //std::cout << " " << static_cast<int>(moveHistory.prevMoves[moveHistory.prevMoves.size()-1].getFlags()) << std::endl;
           moveHistory.pop();

           return 1;
     }

    return 0;
}

// Function "iterativeSearch" 
cMove gameManager::iterativeSearch(cBoard board, int timeToSearch, int depth) {

    time_t startTime, endTime;

    time(&startTime);
    endTime = startTime+timeToSearch;

    cMove bestMove;

    for (int i(2); i < depth; ++i) {

        cMove newBestMove = topNegamaxAB(board, i, endTime, bestMove);

        if (newBestMove.getFrom() != newBestMove.getTo())
            bestMove = newBestMove;

        time_t currentTime;
        time(&currentTime);
        if (difftime(endTime, currentTime) < 0)
            return bestMove;

    }

}
