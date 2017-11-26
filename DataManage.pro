#-------------------------------------------------
#
# Project created by QtCreator 2017-11-23T19:38:58
#
#-------------------------------------------------

QT       += core gui sql
RC_ICONS = DataManage.ico
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataManage
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mainwindow.cpp

HEADERS  += widget.h \
    mainwindow.h

FORMS    += widget.ui \
    mainwindow.ui

RESOURCES += \
    datamanageresource.qrc

DISTFILES +=
