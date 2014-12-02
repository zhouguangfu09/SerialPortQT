#-------------------------------------------------
#
# Project created by QtCreator 2012-01-07T20:14:26


QT       += core gui

TARGET = serialPort
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    win_qextserialport.cpp \
    qextserialport.cpp \
    qextserialbase.cpp \
    showwave.cpp \
    autosenddata.cpp \
    handledata.cpp \
    aboutform.cpp

HEADERS  += mainwindow.h \
    win_qextserialport.h \
    qextserialport.h \
    qextserialbase.h \
    showwave.h \
    autosenddata.h \
    handledata.h \
    aboutform.h

FORMS    += mainwindow.ui \
    showwave.ui \
    autosenddata.ui \
    aboutform.ui

RESOURCES += \
    picture.qrc

OTHER_FILES +=
