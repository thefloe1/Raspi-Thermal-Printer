#-------------------------------------------------
#
# Project created by QtCreator 2012-08-27T13:40:09
#
#-------------------------------------------------

QT       += core

QT       += gui

TARGET = printertest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += extserialport

TEMPLATE = app


SOURCES += printertest.cpp \
    ../../printer.cpp

HEADERS += \
    ../../printer.h
