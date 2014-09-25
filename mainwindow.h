//
//  mainwindow.h
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 4/23/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include "gui.h"
#include "game_manager.h"


// Class "myView" is responsible for managine interaction between the GUI
// and the gameManager.  It also handles mouse events.
class myView : public QGraphicsView
{
    Q_OBJECT

public:
    myView(QGraphicsScene *scene) : QGraphicsView(scene) {}

    // Sub Classes
    GUI *display;
    gameManager gameManager;

    // Functions
    bool checkGameState();
    void move();

    // Info
    bool gameOver = 0, ready = 0, tracking = 0;
    int pieceTracking;

public slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void moveSearch();
    void updateToPlayLabel();


protected:
    void resizeEvent(QResizeEvent *event)
    {
        QGraphicsView::resizeEvent(event);
        fitInView(sceneRect(), Qt::KeepAspectRatio);
    }

};


// Class "searchThread" is a custom thread used to run the search while keeping the
// GUI running and without locking the computer.
class searchThread : public QThread {

    Q_OBJECT

signals:
    void finished();

public:

    bool stop = 0;

    myView *view;

    searchThread(QObject *parent = 0) : QThread(parent) {};

    void run() {

        if (view->gameOver) {
            emit finished();
            return;
        }

        view->gameManager.move();

        emit finished();

    }

};


namespace Ui {
    class MainWindow;
}


// Class "mainWindow" is responsible for managing the interaction
// between the view and the searchThread,
// and for managing the UI Elements outside the chess board.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    myView *view;

    searchThread *search;

    int searchLeft;

    QTimer *searchLeftTimer;

public slots:

    void moveSearch();

    void finishSearch();

    void updateToPlayLabel();

    void updateSearchLeft();


private slots:

    void on_MoveButton_clicked();

    void on_ResetButton_clicked();

    void on_QuitButton_clicked();

    void on_UndoButton_clicked();

    void on_actionPaste_FEN_triggered();

    void on_flipButton_clicked();

    void on_strengthSlider_valueChanged(int value);

    void on_actionCopy_FEN_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

};

#endif // MAINWINDOW_H
