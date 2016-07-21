#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T16:04:06
#
#-------------------------------------------------

QT       -= core
QT       -= gui

CONFIG += c++11

TARGET = final
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DEFINES *= DEBUG
DEFINES *= WITHOUT_DEMONIZATION

#LIBS *= -fopenmp
#QMAKE_CXXFLAGS *= -fopenmp

SOURCES += \
    main.cpp \
    tapplication.cpp \
    thttpserver.cpp \
    tworker.cpp

HEADERS += \
    tapplication.h \
    thttpserver.h \
    tworker.h
