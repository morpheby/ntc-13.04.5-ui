TEMPLATE = app

QT += core gui widgets printsupport

CONFIG += c++11

SOURCES += main.cpp \
    serial/Atom.cpp \
    serial/Log.cpp \
    serial/Logger.cpp \
    serial/SerialComm.cpp \
    pdapi.cpp \
    modbusdriver.cpp \
    modbusconnection.cpp \
    qcustomplot.cpp \
    mainwindow.cpp \
    Events.cpp

CONFIG(debug) {
    DEFINES += DEBUG
}

unix {
    SOURCES += serial/CommHandler-unix.cpp
}

win32 {
    SOURCES += serial/CommHandler-windows.cpp
}

CONFIG += link_pkgconfig
PKGCONFIG += libmodbus

RESOURCES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    serial/Atom.h \
    serial/Atom.hpp \
    serial/CommHandler.h \
    serial/Log.h \
    serial/Logger.h \
    serial/platform.h \
    serial/SerialComm.h \
    serial/SerialComm.hpp \
    pdapi.h \
    modbusdriver.h \
    modbusconnection.h \
    modbusconnection.hpp \
    modbusshared.h \
    pdapi.hpp \
    qcustomplot.h \
    mainwindow.h \
    Events.h

FORMS += \
    mainwindow.ui
