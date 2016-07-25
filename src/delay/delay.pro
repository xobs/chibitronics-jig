! include ( ../../common.pri ) {
  error("Unable to locate common.pri")
}

TEMPLATE = lib
CONFIG += debug
SOURCES += delay.cpp
HEADERS += ../common/testmodule.h
TRANSLATIONS = delay.ts
