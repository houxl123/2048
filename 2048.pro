#-------------------------------------------------
#
# Project created by QtCreator 2016-06-17T10:28:12
#
#-------------------------------------------------

QT       += core gui
CONFIG      += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2048
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp

HEADERS  += mainwindow.h \
    constdefine.h \
    cell.h

RC_ICONS = 2048.ico

RESOURCES += \
    2048.qrc
