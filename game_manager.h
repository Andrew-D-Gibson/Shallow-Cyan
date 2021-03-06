//
//  game_manager.h
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 4/16/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#ifndef __Shallow_Cyan__Game_Manager__
#define __Shallow_Cyan__Game_Manager__

#include "Evaluation_Search.h"


// Class "prevInfo" is used to store information about previous positions 
// that would otherwise be irretrievable without expensive calculations.
// It is used as a part of the gameManager class.
struct prevInfo {
    
    std::vector<char> prevMove_CastlingRights;
    
    std::vector<int> prevEpSquares, prevFiftyMoves;
    
    std::vector<cMove> prevMoves;

    
    void pop() {
        prevMove_CastlingRights.pop_back();
        prevEpSquares.pop_back();
        prevFiftyMoves.pop_back();
        prevMoves.pop_back();
    }
    
    void clear() {
        prevMove_CastlingRights.clear();
        prevEpSquares.clear();
        prevFiftyMoves.clear();
        prevMoves.clear();
    }
    
};


// Class "gameManager" is the main way this program interacts with a board object,
// containing all the methods commonly needed to do so properly and safely.
struct gameManager {

    // The "board" is the meat of this class, and everything 
    // else is designed to allow safe interaction with it.
    cBoard board;


    // Integer "searchTime" defines the number of seconds this program will 
    // think when told to search.  It can be adjusted by the slider in the GUI.
    int searchTime = 1;


    // The prevInfo "moveHistory" stores
    prevInfo moveHistory;


    gameManager();

    void makeMove(cMove &move);

    void move();

    int makePlayerMove(int from, int to, int promoFlag = 0);

    bool undo();

    int checkGameState();

    cMove iterativeSearch(cBoard board, int timeToSearch, int depth = std::numeric_limits<int>::max());

};


#endif /* defined(__Shallow_Cyan__Game_Manager__) */
