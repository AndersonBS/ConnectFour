#-------------------------------------------------
#
# Project created by QtCreator 2016-08-24T01:04:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AI_FourInARow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gameboard.cpp \
    aiplayer.cpp \
    treenode.cpp

HEADERS  += mainwindow.h \
    gameboard.h \
    aiplayer.h \
    treenode.h

RESOURCES += \
    images.qrc
