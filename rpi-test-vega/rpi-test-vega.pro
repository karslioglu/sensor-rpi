QT = core

CONFIG += c++17 cmdline

include(../common.pri)

DESTDIR = $$DESTROOT_BIN

INCLUDEPATH = ../../../sensorreaderlib/
LIBS += -L$$DESTROOT_LIB -lsensorreaderlib

SOURCES += \
        main.cpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

HEADERS +=
