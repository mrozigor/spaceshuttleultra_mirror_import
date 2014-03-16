#-------------------------------------------------
#
# Project created by QtCreator 2014-03-14T17:34:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SSUToolbox
TEMPLATE = app


SOURCES += main.cpp\
        mainWindow.cpp \
    frameMECO.cpp \
    frameKits.cpp \
    frameLanding.cpp \
    frameCrew.cpp \
    frameFlightData.cpp \
    frameMission.cpp

HEADERS  += mainWindow.h \
    frameMECO.h \
    frameKits.h \
    frameLanding.h \
    frameCrew.h \
    frameFlightData.h \
    frameMission.h

FORMS    += mainWindow.ui \
    frameMECO.ui \
    frameKits.ui \
    frameLanding.ui \
    frameCrew.ui \
    frameFlightData.ui \
    frameMission.ui
