#-------------------------------------------------
#
# Project created by QtCreator 2016-09-29T09:21:12
#
#-------------------------------------------------

QT       += core sql
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG(release, debug|release)
{
    win32: DESTDIR = ../Win32/Release/bin
    unix:  DESTDIR = ../Unix/Release/bin
}
CONFIG(debug, debug|release)
{
    win32: DESTDIR = ../Win32/Debug/bin
    unix:  DESTDIR = ../Unix/Debug/bin

    DEFINES += _DEBUG
}

SOURCES += \
    main.cpp \
    Src/CDatabaseInit.cpp

HEADERS += \
    Src/CDatabaseInit.h

INCLUDEPATH += \
    $$PWD/../DbBaseLib
DEPENDPATH += \
    $$PWD/../DbBaseLib

win32 {
CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../Win32/release/lib/ -lDbBaseLib
else:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../Win32/Debug/lib/ -lDbBaseLib
}
unix {
CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../Unix/release/lib/ -lDbBaseLib
else:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../Unix/Debug/lib/ -lDbBaseLib
}
