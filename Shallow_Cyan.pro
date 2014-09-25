#-------------------------------------------------
#
# Project created by QtCreator 2014-05-11T22:28:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "Shallow Cyan"
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    board.cpp \
    evaluation_search.cpp \
    game_manager.cpp \
    move.cpp \
    pl_move_gen.cpp \
    legal_move_gen.cpp \
    gui.cpp

HEADERS  += mainwindow.h \
    bitboard.h \
    board.h \
    evaluation_search.h \
    game_manager.h \
    move.h \
    pl_move_gen.h \
    legal_move_gen.h \
    gui.h

FORMS    += mainwindow.ui

RESOURCES += \
    spriteImages.qrc
