QT -= gui

TEMPLATE = lib
DEFINES += SENSORREADER_LIBRARY

CONFIG += c++17

include(../common.pri)

DESTDIR = $$DESTROOT_LIB

SOURCES += \
    mcp3008reader.cpp \
    sensorlogger.cpp \
    sensorreader.cpp

HEADERS += \
    adcfilter.h \
    mcp3008reader.h \
    sensorlogger.h \
    sensorreader_global.h \
    sensorreader.h

unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
