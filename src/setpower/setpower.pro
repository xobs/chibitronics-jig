! include ( ../../common.pri ) {
  error("Unable to locate common.pri")
}

TEMPLATE = lib
CONFIG += debug
SOURCES += setpower.cpp
HEADERS += ../common/testmodule.h
