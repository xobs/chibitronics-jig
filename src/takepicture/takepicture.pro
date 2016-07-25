! include ( ../../common.pri ) {
  error("Unable to locate common.pri")
}

TEMPLATE = lib
CONFIG += debug
SOURCES += takepicture.cpp
HEADERS += ../common/testmodule.h
TRANSLATIONS = takepicture.ts
