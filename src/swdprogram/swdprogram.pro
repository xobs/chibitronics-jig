! include ( ../../common.pri ) {
  error("Unable to locate common.pri")
}

TEMPLATE = lib
CONFIG += debug
SOURCES += swdprogram.cpp
HEADERS += ../common/testmodule.h
TRANSLATIONS = swdprogram.ts
