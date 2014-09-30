//
//  main.cpp
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 11/23/13.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#include "mainwindow.h"


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Shallow Cyan");
    w.show();

    return a.exec();
}
