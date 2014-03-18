#-------------------------------------------------
#
# Project created by QtCreator 2014-03-14T17:58:28
#
#-------------------------------------------------

QT       += widgets core gui declarative

TARGET = test
TEMPLATE = lib

DEFINES += TEST_LIBRARY

SOURCES += test.cpp \
    mainwindow.cpp \
    myQtWinManager.cpp

OrbiterSDKDir = C:/SSUToolboxSVN/Orbitersdk

HEADERS += test.h\
        test_global.h \
    mainwindow.h \
    myQtWinManager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$OrbiterSDKDir/include
INCLUDEPATH += $$OrbiterSDKDir/lib
DEPENDPATH += $$OrbiterSDKDir/include
DEPENDPATH += $$OrbiterSDKDir/lib

LIBS += -L$$OrbiterSDKDir/lib/ -lorbiter -lOrbitersdk

FORMS += \
    mainwindow.ui
