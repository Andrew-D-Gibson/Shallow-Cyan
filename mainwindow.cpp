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
