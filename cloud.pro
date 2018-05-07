#-------------------------------------------------
#
# Project created by QtCreator 2018-04-25T11:26:57
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cloud
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    udpclent.cpp \
    elink.cpp

HEADERS  += mainwindow.h \
    udpclent.h \
    elink.h

FORMS    += mainwindow.ui

LIBS += "E:/SoftwarePacket/ec4027/ec8000_eerom/ec8000_eeprom_cloud/USB_Driver.dll"

RESOURCES += \
    res.qrc
