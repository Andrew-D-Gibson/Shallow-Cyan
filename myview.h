//
//  myview.h
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 9/25/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#ifndef MYVIEW_H
#define MYVIEW_H

#include <QtWidgets>
#include "gui.h"
#include "game_manager.h"


// Class "myView" is responsible for managing interaction between the GUI
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


#endif // MYVIEW_H
