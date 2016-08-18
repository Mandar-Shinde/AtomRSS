#-------------------------------------------------
#
# Project created by QtCreator 2016-08-12T22:10:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = untitled25
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    webworker.cpp

HEADERS  += mainwindow.h \
    webworker.h

FORMS    += mainwindow.ui
