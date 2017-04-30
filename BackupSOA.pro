#-------------------------------------------------
#
# Project created by QtCreator 2017-03-28T13:18:15
#
#-------------------------------------------------

QT       += core gui network
PROTOS = protocolbuffer.proto
include(protobuf.pri)
LIBS += -L/usr/local/lib -lprotobuf
INCLUDEPATH += /usr/local/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BackupSOA
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        backupwindow.cpp

HEADERS  += backupwindow.h \
    protocolbuffer.proto \
    magicobject.h \
    backupserver.h \
    backupuser.h

FORMS    += backupwindow.ui

STATECHARTS +=
