#-------------------------------------------------
#
# Project created by QtCreator 2016-03-03T14:59:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Rewinder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    keyemitter.cpp \
    devicemanager.cpp \
    rewinder.cpp \
    acceleration.cpp \
    counter.cpp

HEADERS  += mainwindow.h \
    keyemitter.h \
    devicemanager.h \
    rewinder.h \
    acceleration.h \
    counter.h

FORMS    += mainwindow.ui

LIBS    += -lmodbus \
    -lwiringPi
