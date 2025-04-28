DEBUG_SUFFIX =
DEBUG_STR=
COMPILER_STR =
ARCH_STR = $${QT_ARCH}
PLATFORM_STR=

CONFIG(debug, debug|release) {
    DEBUG_STR = debug
    DEBUG_SUFFIX = d
} else {
    DEBUG_STR = release
    DEBUG_SUFFIX =
}

# COMPILER_STR = $$[QT_INSTALL_PREFIX]
# COMPILER_STR = $$basename(COMPILER_STR)-Qt$$[QT_VERSION]
COMPILER_STR = gcc  # örnek varsayım

PLATFORM_STR = $${COMPILER_STR}-$${ARCH_STR}

VER_MAJ = 1
VER_MIN = 0
VER_PAT = 0

VERSION = $$VER_MAJ"."$$VER_MIN"."$$VER_PAT

LIBVER = $$VERSION"-release"
DEFINES += LIBVER=\\\"$$LIBVER\\\"

DEFINES += VERSION=\\\"$$VERSION\\\"
DEFINES += VER_MAJ=\\\"$$VER_MAJ\\\"
DEFINES += VER_MIN=\\\"$$VER_MIN\\\"
DEFINES += VER_PAT=\\\"$$VER_PAT\\\"

PROJNAME = hidrolink

DESTROOT = $$OUT_PWD/../dist/$${PROJNAME}-$${VER_MAJ}.$${VER_MIN}-$${PLATFORM_STR}
DESTROOT_BIN = $$DESTROOT/bin
DESTROOT_LIB = $$DESTROOT/lib
