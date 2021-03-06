#-------------------------------------------------
#
# SimComp GUI - V 0.30
# Author: Ole Bjørn Eithun Pedersen
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimComp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        runwidget.cpp \
    idewidget.cpp \
    performancechart.cpp \
    memorymap.cpp \
    dropdownwidget.cpp \
    fileviewer.cpp \
    memorywindow.cpp

HEADERS += \
        mainwindow.h \
        globals.h \
        runwidget.h \
    idewidget.h \
    performancechart.h \
    memorymap.h \
    dropdownwidget.h \
    fileviewer.h \
    memorywindow.h

RESOURCES += \
    images.qrc \
    styles.qrc

win32:RC_ICONS = SimComp_icon.ico
macx:ICON = SimComp_icon.icns

include(../../_Simulator/_src/Simulator_core.pri)
