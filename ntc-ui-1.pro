TEMPLATE = app

QT += qml quick

CONFIG += c++11

SOURCES += main.cpp \
    serial/Atom.cpp \
    serial/Log.cpp \
    serial/Logger.cpp \
    serial/SerialComm.cpp

unix {
    SOURCES += serial/CommHandler-unix.cpp
}

win32 {
    SOURCES += serial/CommHandler-windows.cpp
}

RESOURCES += qml.qrc

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
    serial/SerialComm.hpp
