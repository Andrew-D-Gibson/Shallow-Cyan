//
//  gui.h
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 5/10/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#ifndef GUI_H
#define GUI_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>


// Class "pieceImage" stores the actual image of a piece
// for the GUI, as well as the pieces location and color.
// Class "pieceImage" is used as part of the "GUI" class.
class pieceImage {
public:
    pieceImage (QPixmap i, int l, bool c, bool flipped);

    int location;

    bool color;

    QGraphicsPixmapItem *image = new QGraphicsPixmapItem;

};


// Class "GUI" draws the board during run time.
class GUI
{
public:

    bool flipped = 0;

    int lastFrom = -1, lastTo = -1, selected = -1;

    QGraphicsScene *scene;

    std::vector<pieceImage*> pieces;

    GUI();

    void setScene( QGraphicsScene *s);

    void setPos(int boardPos[64]);

    void updateFromToIndicators(int from, int to) {
        lastFrom = (flipped) ? 63-from : from;
        lastTo = (flipped) ? 63-to : to;
    }

    void updateSelected(int sel) {
        selected = sel;
    }

    bool isPieceAtLoc(int loc, bool color = 1);


private:
    QPixmap wPawn, wKnight, wBishop, wRook, wQueen, wKing,
        bPawn, bKnight, bBishop, bRook, bQueen, bKing;


};


#endif // GUI_H
