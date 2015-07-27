TEMPLATE = app

QT += core gui widgets printsupport serialport

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
    Events.cpp \
    pdpoller.cpp \
    portpoller.cpp \
    alglib/alglibinternal.cpp \
    alglib/alglibmisc.cpp \
    alglib/ap.cpp \
    alglib/dataanalysis.cpp \
    alglib/diffequations.cpp \
    alglib/fasttransforms.cpp \
    alglib/integration.cpp \
    alglib/interpolation.cpp \
    alglib/linalg.cpp \
    alglib/optimization.cpp \
    alglib/solvers.cpp \
    alglib/specialfunctions.cpp \
    alglib/statistics.cpp

CONFIG(debug, debug|release) {
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

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

RC_ICONS = logo.ico

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
    Events.h \
    pdpoller.h \
    portpoller.h \
    alglib/alglibinternal.h \
    alglib/alglibmisc.h \
    alglib/ap.h \
    alglib/dataanalysis.h \
    alglib/diffequations.h \
    alglib/fasttransforms.h \
    alglib/integration.h \
    alglib/interpolation.h \
    alglib/linalg.h \
    alglib/optimization.h \
    alglib/solvers.h \
    alglib/specialfunctions.h \
    alglib/statistics.h \
    alglib/stdafx.h

FORMS += \
    mainwindow.ui
TRANSLATIONS += ntc_ru.ts

DISTFILES += \
    logo.ico
