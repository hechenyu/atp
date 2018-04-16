QT += core
QT += network
QT -= gui

TARGET = ATP_Igsmr_Capture
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    posix_qextserialport.cpp \
    qextserialbase.cpp \
    qextserialport.cpp \
    collectiondata.cpp \
    igsmrmonitor.cpp \
    igsmrmonitorapp.cpp

HEADERS += \
    posix_qextserialport.h \
    qextserialbase.h \
    qextserialport.h \
    collectiondata.h \
    igsmrmonitor.h \
    igsmrmonitorapp.h

