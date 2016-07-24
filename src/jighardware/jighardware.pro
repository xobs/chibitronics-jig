! include ( ../../common.pri ) {
  error("Unable to locate common.pri")
}

TEMPLATE = lib
CONFIG += debug
SOURCES += \
           wiringpi.c \
    jighardware.c
HEADERS += ../common/testmodule.h
