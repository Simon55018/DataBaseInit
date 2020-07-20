#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T10:00:44
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = DbBaseLib
TEMPLATE = lib
CONFIG  += staticlib

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#begin 输出路径
CONFIG(debug, debug|release){
    MOC_DIR = ./GeneratedFiles/debug
    OBJECTS_DIR = ./debug
    win32:DESTDIR = $$PWD/../Win32/Debug/lib
    unix:DESTDIR = $$PWD/../Unix/Debug/lib

    DEFINES += _DEBUG

    LIBS += -L$$DESTDIR -lDbCipherDriver
}
else: CONFIG(release, debug|release){
    MOC_DIR = ./GeneratedFiles/release
    OBJECTS_DIR = ./release
    win32:DESTDIR = $$PWD/../Win32/Release/lib
    unix:DESTDIR = $$PWD/../Unix/Release/lib

    LIBS += -L$$DESTDIR -lDbCipherDriver
}
#end

INCLUDEPATH += \
    ./include

OTHER_FILES += \
    readme.txt

HEADERS += \
    CSqlQuery.h \
    src/CDbMain.h \
    src/CDbManager.h

SOURCES += \
    CSqlQuery.cpp \
    src/CDbMain.cpp
