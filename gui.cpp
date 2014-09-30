//
//  gui.h
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 5/10/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#include "gui.h"
#include <QPainter>

// Array "squareOffsets" holds all the locations of the
// upper left corner of the squares in Little Endian coordinates.
QPointF squareOffsets[64] = {
    QPointF(0,1792), QPointF(256, 1792), QPointF(512, 1792), QPointF(768, 1792), QPointF(1024, 1792), QPointF(1280, 1792), QPointF(1536, 1792), QPointF(1792, 1792),
    QPointF(0,1536), QPointF(256, 1536), QPointF(512, 1536), QPointF(768, 1536), QPointF(1024, 1536), QPointF(1280, 1536), QPointF(1536, 1536), QPointF(1792, 1536),
    QPointF(0,1280), QPointF(256, 1280), QPointF(512, 1280), QPointF(768, 1280), QPointF(1024, 1280), QPointF(1280, 1280), QPointF(1536, 1280), QPointF(1792, 1280),
    QPointF(0,1024), QPointF(256, 1024), QPointF(512, 1024), QPointF(768, 1024), QPointF(1024, 1024), QPointF(1280, 1024), QPointF(1536, 1024), QPointF(1792, 1024),
    QPointF(0,768), QPointF(256, 768), QPointF(512, 768), QPointF(768, 768), QPointF(1024, 768), QPointF(1280, 768), QPointF(1536, 768), QPointF(1792, 768),
    QPointF(0,512), QPointF(256, 512), QPointF(512, 512), QPointF(768, 512), QPointF(1024, 512), QPointF(1280, 512), QPointF(1536, 512), QPointF(1792, 512),
    QPointF(0,256), QPointF(256, 256), QPointF(512, 256), QPointF(768, 256), QPointF(1024, 256), QPointF(1280, 256), QPointF(1536, 256), QPointF(1792, 256),
    QPointF(0,0), QPointF(256, 0), QPointF(512, 0), QPointF(768, 0), QPointF(1024, 0), QPointF(1280, 0), QPointF(1536, 0), QPointF(1792, 0),
};


pieceImage::pieceImage(QPixmap i, int l, bool c, bool flipped) : location(l), color(c) {
    image->setPixmap(i);

    if (location >= 0 && location <= 63) {
        if (flipped)
            image->setOffset(1792 - squareOffsets[location].x(), 1792 - squareOffsets[location].y());
        else
            image->setOffset(squareOffsets[location]);
    }
}


GUI::GUI() {

    // White Pieces
    wPawn = QPixmap("://GUI Images/wp.png").scaled(QSize(256,256));
    wBishop = QPixmap("://GUI Images/wb.png").scaled(QSize(256,256));
    wKnight = QPixmap("://GUI Images/wn.png").scaled(QSize(256,256));
    wRook = QPixmap("://GUI Images/wr.png").scaled(QSize(256,256));
    wQueen = QPixmap("://GUI Images/wq.png").scaled(QSize(256,256));
    wKing = QPixmap("://GUI Images/wk.png").scaled(QSize(256,256));

    // Black Pieces
    bPawn = QPixmap("://GUI Images/bp.png").scaled(QSize(256,256));
    bBishop = QPixmap("://GUI Images/bb.png").scaled(QSize(256,256));
    bKnight = QPixmap("://GUI Images/bn.png").scaled(QSize(256,256));
    bRook = QPixmap("://GUI Images/br.png").scaled(QSize(256,256));
    bQueen = QPixmap("://GUI Images/bq.png").scaled(QSize(256,256));
    bKing = QPixmap("://GUI Images/bk.png").scaled(QSize(256,256));

}


// Function "setPos" takes in a array of integers that represent the state
// of the board, and draws the GUI to match it.
void GUI::setPos(int boardPos[64]) {

    pieces.clear();
    scene->clear();
    scene->addPixmap(QPixmap("://GUI Images/Chess Board.png").scaled(QSize(2048, 2048)));

    QPen boardBorder(QColor(0, 0, 0));
    boardBorder.setWidth(16);

    scene->addRect(-8, -8, 2056, 2056, boardBorder);



    //  Last move and selected indicators
    QPen toFromOutline(QColor(0,0,0));
    toFromOutline.setWidth(6);

    if (lastTo >= 0 && lastTo < 64 && lastFrom >= 0 && lastFrom < 64) {
        scene->addRect(squareOffsets[lastTo].x(), squareOffsets[lastTo].y(), 256, 256, toFromOutline, QColor(0,0,0,0));
        scene->addRect(squareOffsets[lastFrom].x(), squareOffsets[lastFrom].y(), 256, 256, toFromOutline, QColor(0,0,0,0));
    }

    if (selected >= 0 && selected < 64) {
        if (flipped)
            scene->addRect(1792-squareOffsets[selected].x(), 1792-squareOffsets[selected].y(), 256, 256, toFromOutline, QColor(0,0,0,0));
        else
            scene->addRect(squareOffsets[selected].x(), squareOffsets[selected].y(), 256, 256, toFromOutline, QColor(0,0,0,0));
    }


    // Pieces
    for(int i(0); i < 64; ++i) {

            if (boardPos[i] == -1) {}
            else if (boardPos[i] == 0) {
                pieces.push_back(new pieceImage(wPawn, i, 1, flipped));
            }
            else if (boardPos[i] == 1) {
                pieces.push_back(new pieceImage(wKnight, i, 1, flipped));
            }
            else if (boardPos[i] == 2) {
                pieces.push_back(new pieceImage(wBishop, i, 1, flipped));
            }
            else if (boardPos[i] == 3) {
                pieces.push_back(new pieceImage(wRook, i, 1, flipped));
            }
            else if (boardPos[i] == 4) {
                pieces.push_back(new pieceImage(wQueen, i, 1, flipped));
            }
            else if (boardPos[i] == 5) {
                pieces.push_back(new pieceImage(wKing, i, 1, flipped));
            }

            else if (boardPos[i] == 6) {
                pieces.push_back(new pieceImage(bPawn, i, 0, flipped));
            }
            else if (boardPos[i] == 7) {
                pieces.push_back(new pieceImage(bKnight, i, 0, flipped));
            }
            else if (boardPos[i] == 8) {
                pieces.push_back(new pieceImage(bBishop, i, 0, flipped));
            }
            else if (boardPos[i] == 9) {
                pieces.push_back(new pieceImage(bRook, i, 0, flipped));
            }
            else if (boardPos[i] == 10) {
                pieces.push_back(new pieceImage(bQueen, i, 0, flipped));
            }
            else if (boardPos[i] == 11) {
                pieces.push_back(new pieceImage(bKing, i, 0, flipped));
            }

        }

    for (int i(0); i < pieces.size(); ++i)
        scene->addItem(pieces[i]->image);

}


// Function "isPieceAtLoc" returns true if a piece of the right color is at
// a given location, and false if otherwise.
bool GUI::isPieceAtLoc(int loc, bool color) {

    for (int i(0); i < pieces.size(); ++i) {
        if (pieces[i]->location == loc && pieces[i]->color == color) {
            return 1;
        }
    }

    return 0;

}
