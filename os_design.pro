#-------------------------------------------------
#
# Project created by QtCreator 2015-12-30T10:46:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = os_design
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    process.cpp \
    dynamicallocation.cpp

HEADERS  += mainwindow.h \
    process.h \
    dynamicallocation.h

FORMS    += mainwindow.ui

# to support C++11
QMAKE_CXXFLAGS += -std=c++11
