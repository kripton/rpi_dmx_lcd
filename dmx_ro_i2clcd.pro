#-------------------------------------------------
#
# Project created by QtCreator 2012-08-15T17:48:07
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = dmx_ro_i2clcd
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    universetolcd.cpp \
    hd44780_i2c.cpp

HEADERS += \
    universetolcd.h \
    hd44780_i2c.h

LIBS += -lqjson -lola
