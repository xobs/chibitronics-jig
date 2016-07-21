! include ( ../../common.pri ) {
  error("Unable to locate common.pri")
}

TEMPLATE = lib
CONFIG += debug
HEADERS += ../common/testmodule.h \
           network_updater.h \
           disk_updater.h \
           update_controller.h

SOURCES += updater.cpp \
            network_updater.cpp \
            disk_updater.cpp \
            update_controller.cpp
