//
//  myview.h
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 9/25/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#include "myview.h"


void myView::move() {

    if (gameOver)
        return;

    gameManager.move();
    display->updateFromToIndicators(gameManager.moveHistory.prevMoves[gameManager.moveHistory.prevMoves.size()-1].getFrom(), gameManager.moveHistory.prevMoves[gameManager.moveHistory.prevMoves.size()-1].getTo());
    display->setPos(gameManager.board.currentPos);

    if (checkGameState())
        return;

}

void myView::mousePressEvent(QMouseEvent *event) {

    if (gameOver || !ready)
        return;

    QPointF pt = mapToScene(event->pos());

    if (pt.x() >= 0 && pt.x() <= 2048 && pt.y() >= 0 && pt.y() <= 2048) {

        int rank = display->flipped ? pt.y() / 256 : 8 - (pt.y() / 256),
        file =  display->flipped ? (2048 - pt.x()) / 256 : pt.x() / 256,
        square = (rank * 8) + file;

        bool pieceListLoc = display->isPieceAtLoc(square, (gameManager.board.move_CastlingRights & 1));

        if (pieceListLoc) {

            for (int i(0); i < display->pieces.size(); i++) {
                if (display->pieces[i]->location == square) {

                    display->updateSelected(-1);
                    display->setPos(gameManager.board.currentPos);

                    // This remove and add makes sure the selected piece is drawn last, and so is on top of everything else.
                    display->scene->removeItem(display->pieces[i]->image);
                    display->scene->addItem(display->pieces[i]->image);

                    pieceTracking = i;
                    break;
                }
            }

            tracking = 1;

            display->updateSelected(square);

        }

 }

 }

void myView::mouseMoveEvent(QMouseEvent *event) {

    if (gameOver || !ready)
        return;

    if (tracking) {
        QPointF pt = mapToScene(event->pos());

         int offsetX = pt.x() - 128, offsetY = pt.y() - 128;

        if (pt.x() - 128 < 0)
             offsetX = 0;
        else if (pt.x() + 128 > 2048)
            offsetX = 1792;

        if (pt.y() - 128 < 0)
             offsetY = 0;
        else if (pt.y() + 128 > 2048)
            offsetY = 1792;

        display->pieces[pieceTracking]->image->setOffset(offsetX, offsetY);
    }

}

void myView::mouseReleaseEvent(QMouseEvent *event) {

    if (gameOver || !ready)
        return;

    QPointF pt = mapToScene(event->pos());

    if (pt.x() >= 0 && pt.x() <= 2048 && pt.y() >= 0 && pt.y() <= 2048) {

        tracking = 0;

        int rank = display->flipped ? pt.y() / 256 : 8 - (pt.y() / 256),
        file =  display->flipped ? (2048 - pt.x()) / 256 : pt.x() / 256,
        square = (rank * 8) + file;

        bool pieceListLoc = display->isPieceAtLoc(square, (gameManager.board.move_CastlingRights & 1));

        if (pieceListLoc == 1) {
             display->updateSelected(square);
             display->setPos(gameManager.board.currentPos);
             return;
        }
        else if (gameManager.makePlayerMove(display->selected, square) == 1 ) {
                display->updateSelected(-1);
                display->updateFromToIndicators(-1, -1);
                display->setPos(gameManager.board.currentPos);

                emit updateToPlayLabel();

                if (checkGameState())
                    return;

                emit moveSearch();
                return;
        }
        else if (gameManager.makePlayerMove(display->selected, square) == 2 ) {        // Promo Move requires custom Input

              QStringList pieces;
              cMove promoMove;
              bool ok;

              pieces << "Queen" << "Rook" << "Bishop" << "Knight";
              QString item = QInputDialog::getItem(this, "Sanguine","Pick Promotion Piece", pieces, 0, false, &ok);
              if (!ok || item.isEmpty())
                  return;

              if (item == "Queen")
                  promoMove = cMove(gameManager.board.currentPos[display->selected], display->selected, square, 15);
              else if (item == "Rook")
                  promoMove = cMove(gameManager.board.currentPos[display->selected], display->selected, square, 14);
              else if (item == "Bishop")
                  promoMove = cMove(gameManager.board.currentPos[display->selected], display->selected, square, 13);
              else if (item == "Knight")
                  promoMove = cMove(gameManager.board.currentPos[display->selected], display->selected, square, 12);

              if (gameManager.board.currentPos[square] != -1)
                promoMove.setCapturedPiece(gameManager.board.currentPos[square]);

              gameManager.moveHistory.prevMoves.push_back(promoMove);
              gameManager.moveHistory.prevEpSquares.push_back(gameManager.board.epSquare);
              gameManager.moveHistory.prevMove_CastlingRights.push_back(gameManager.board.move_CastlingRights);
              gameManager.board.move(promoMove);

              display->updateSelected(-1);
              display->updateFromToIndicators(-1, -1);
              display->setPos(gameManager.board.currentPos);

              emit updateToPlayLabel();

              if (checkGameState())
                  return;

             emit moveSearch();
              return;

        }

    }

    display->updateSelected(-1);
    display->setPos(gameManager.board.currentPos);
    display->scene->update();

}

bool myView::checkGameState() {

    int gameState = gameManager.checkGameState();

    if (gameState != 0) {

        gameOver = 1;
        emit updateToPlayLabel();

        QMessageBox msgBox;

        switch (gameState) {
            case 1:
                msgBox.setText("White Won!");
                break;
            case -1:
                msgBox.setText("Black Won!");
                break;
            case 5:
                msgBox.setText("Draw.");
                break;
        }

        msgBox.setInformativeText("Reset Game?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Ok);

        int choice = msgBox.exec();

        if (choice == QMessageBox::Ok) {

            gameOver = 0;
            display->updateSelected(-1);
            display->updateFromToIndicators(-1,-1);
            gameManager.board.reset();
            gameManager.moveHistory.clear();
            display->setPos(gameManager.board.currentPos);

        }

        return 1;

    }

    return 0;

}
