! include ( ../../common.pri ) {
  error("Unable to locate common.pri")
}

CONFIG += debug

SOURCES += main.cpp \
    ../common/chibitestregistry.cpp \
    ../common/chibitest.cpp

HEADERS += \
    ../common/chibitestregistry.h \
    ../common/chibitest.h \
    ../common/testmodule.h
