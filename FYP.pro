#-------------------------------------------------
#
# Project created by QtCreator 2011-01-07T14:50:58
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = FYP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    geometry.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    geometry.h

FORMS    += mainwindow.ui

LIBS    += /usr/lib/libPtex.so
