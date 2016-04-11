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
    dynamicallocation.cpp \
    findfirstfit.cpp \
    processinmemmodel.cpp \
    processfinmodel.cpp \
    jobmodel.cpp \
    resourcemodel.cpp \
    MemFitInterface.cpp

HEADERS  += mainwindow.h \
    process.h \
    dynamicallocation.h \
    findfirstfit.h \
    processinmemmodel.h \
    processfinmodel.h \
    jobmodel.h \
    resourcemodel.h \
    MemFitInterface.h

FORMS    += mainwindow.ui
UI_DIR = ./UI
# to support C++11
QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    qdarkstyle/style.qrc
