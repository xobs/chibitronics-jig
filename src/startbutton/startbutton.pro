! include ( ../../common.pri ) {
  error("Unable to locate common.pri")
}

TEMPLATE = lib
CONFIG += debug
SOURCES += startbutton.c \
           wiringpi.c
HEADERS += ../common/testmodule.h
