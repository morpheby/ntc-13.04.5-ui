TEMPLATE = app

QT += qml quick

SOURCES += main.cpp \
    serialcomm.cpp \
    serial/Atom.cpp \
    serial/CommHandler-unix.cpp \
    serial/CommHandler-windows.cpp \
    serial/Log.cpp \
    serial/Logger.cpp \
    serial/SerialComm.cpp \
    serial/SerialComm_test.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    serialcomm.h \
    serial/Atom.h \
    serial/Atom.hpp \
    serial/CommHandler.h \
    serial/Log.h \
    serial/Logger.h \
    serial/platform.h \
    serial/SerialComm.h \
    serial/SerialComm.hpp
