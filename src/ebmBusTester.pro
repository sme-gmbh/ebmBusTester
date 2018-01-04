#-------------------------------------------------
#
# Project created by QtCreator 2018-01-04T03:26:10
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ebmBusTester
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ebmbus.cpp

HEADERS  += mainwindow.h \
    ebmbus.h

FORMS    += mainwindow.ui
