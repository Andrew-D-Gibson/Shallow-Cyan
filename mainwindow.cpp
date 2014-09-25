//
//  mainwindow.cpp
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 4/23/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Brush
    QBrush backgroundBrush(Qt::cyan, Qt::Dense6Pattern);


    // Scene and View
    scene = new QGraphicsScene(this);

    view = new myView(scene);
    view->setParent(this);
    view->setBackgroundBrush(backgroundBrush);

    view->display = new GUI;
    view->display->scene = scene;
    view->display->setPos(view->gameManager.board.currentPos);

    ui->layout->addWidget(view);
    view->show();


    // Label Font
    QFont f( "Helvetica Neue", 16);
    ui->strengthLabel->setFont(f);
    ui->toPlayLabel->setFont(f);
    updateToPlayLabel();


    // Search Thread
    search = new searchThread();
    search->view = view;
    connect(view, SIGNAL(moveSearch()), this, SLOT(moveSearch()));
    connect(view, SIGNAL(updateToPlayLabel()), this, SLOT(updateToPlayLabel()));
    connect(search, SIGNAL(finished()), this, SLOT(finishSearch()));

    searchLeftTimer = new QTimer(this);
    searchLeftTimer->setInterval(1000);
    connect(searchLeftTimer, SIGNAL(timeout()), this, SLOT(updateSearchLeft()) );

    view->ready = 1;

}

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

              promoMove.draw();

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

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::moveSearch() {

    view->ready = 0;

    ui->strengthLabel->setText(ui->strengthLabel->text() + "\nThinking...");
    view->display->updateSelected(-1);
    view->display->updateFromToIndicators(-1,-1);

    searchLeft = view->gameManager.searchTime;
    searchLeftTimer->start();

    search->start();

}

void MainWindow::finishSearch() {
    view->ready = 1;

    searchLeftTimer->stop();

    if (!view->gameOver) {
        view->display->updateFromToIndicators(view->gameManager.moveHistory.prevMoves[view->gameManager.moveHistory.prevMoves.size()-1].getFrom(), view->gameManager.moveHistory.prevMoves[view->gameManager.moveHistory.prevMoves.size()-1].getTo());
        view->display->setPos(view->gameManager.board.currentPos);
    }

    view->checkGameState();

    on_strengthSlider_valueChanged(ui->strengthSlider->value());

    updateToPlayLabel();

}

void MainWindow::on_MoveButton_clicked() {
    if (!view->ready)
        return;

    moveSearch();
}

void MainWindow::on_ResetButton_clicked() {

    if (!view->ready)
        return;

    view->gameOver = 0;
    view->display->updateSelected(-1);
    view->display->updateFromToIndicators(-1,-1);
    view->gameManager.board.reset();
    view->gameManager.moveHistory.clear();
    view->display->setPos(view->gameManager.board.currentPos);
    updateToPlayLabel();

    view->scene()->update();
    view->update();
}

void MainWindow::on_QuitButton_clicked() {
    QApplication::quit();
}

void MainWindow::on_UndoButton_clicked() {
    if (!view->ready)
        return;

    if (view->gameManager.moveHistory.prevMoves.size() >= 1)
        view->display->updateFromToIndicators(view->gameManager.moveHistory.prevMoves[view->gameManager.moveHistory.prevMoves.size()-1].getFrom(), view->gameManager.moveHistory.prevMoves[view->gameManager.moveHistory.prevMoves.size()-1].getTo());
    else
        view->display->updateFromToIndicators(-1,-1);

    if (view->gameManager.undo())
        view->gameOver = 0;

    view->display->updateSelected(-1);
    view->display->setPos(view->gameManager.board.currentPos);
    updateToPlayLabel();

    view->scene()->update();
    view->update();
}

void MainWindow::on_actionPaste_FEN_triggered() {
    if (!view->ready)
        return;

    QClipboard *clipboard = QApplication::clipboard();
    std::string originalText = clipboard->text().toUtf8().constData();

    cBoard boardCopy = view->gameManager.board;

    if (!(view->gameManager.board.setBoard(originalText))) {
        QMessageBox msgBox;
        msgBox.setText("Invalid FEN");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        view->gameManager.board = boardCopy;
    }
    else {
        view->display->updateSelected(-1);
        view->display->updateFromToIndicators(-1,-1);
        view->gameManager.moveHistory.clear();
    }

    view->display->setPos(view->gameManager.board.currentPos);
    updateToPlayLabel();

    if (view->checkGameState())
        return;
}

void MainWindow::on_actionCopy_FEN_triggered() {
    QApplication::clipboard()->setText(QString::fromStdString(view->gameManager.board.getFEN()));
}

void MainWindow::on_flipButton_clicked() {
    view->display->flipped = !view->display->flipped;

    view->display->lastFrom = 63 - view->display->lastFrom;
    view->display->lastTo = 63 - view->display->lastTo;
    view->display->selected = view->display->selected;

    view->display->setPos(view->gameManager.board.currentPos);
    view->display->scene->update();
}

void MainWindow::on_strengthSlider_valueChanged(int value) {
    if (view->ready) {

        int searchTimes[10] = {0, 1, 3, 5, 10, 30, 60, 180, 300, 1200};

        value = searchTimes[value];

        view->gameManager.searchTime = value;

        QString output;
        if (value == 0)
            output = QString(" < 1 s");
        else if (value < 60)
            output = (QString::number(value) + " sec");
        else {
            output.setNum(value/60);
            output += ":00 min";
        }

        ui->strengthLabel->setText("Search: " + output);
    }
}


void MainWindow::updateToPlayLabel() {
    if (view->gameManager.board.move_CastlingRights & 0x1)
        ui->toPlayLabel->setText("<b>White to Play</b>");
    else
        ui->toPlayLabel->setText("<b>Black to Play</b>");
}

void MainWindow::updateSearchLeft() {
    if (searchLeft > 0)
        searchLeft--;

    int minutes = searchLeft / 60, seconds = searchLeft % 60;
    QString output;

    if (minutes > 0) {
        if (seconds >= 10)
            output = "Search: " + QString::number(minutes) + ":" + QString::number(seconds) + " min\nThinking...";
        else
            output = "Search: " + QString::number(minutes) + ":0" + QString::number(seconds) + " min\nThinking...";
    }
    else {
         output = "Search: " + QString::number(seconds) + " sec\nThinking...";
    }

    ui->strengthLabel->setText(output);
}
