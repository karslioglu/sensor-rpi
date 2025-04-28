QT       += core gui widgets quick

CONFIG += c++17 qt quick qml release

include(../common.pri)

DESTDIR = $$DESTROOT_BIN

INCLUDEPATH = ../../../sensorreaderlib/
LIBS += -L$$DESTROOT_LIB -lsensorreaderlib

SOURCES += \
    main.cpp \
    sensordatamodel.cpp

HEADERS += \
    sensordatamodel.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc
