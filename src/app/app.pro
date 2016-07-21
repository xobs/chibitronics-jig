! include ( ../../common.pri ) {
  error("Unable to locate common.pri")
}

TEMPLATE += app
QT += gui qml quick widgets
CONFIG += debug

SOURCES += main.cpp \
    chibisequence.cpp \
    logger.cpp \
    ../common/chibitestregistry.cpp \
    ../common/chibitest.cpp

# Include QML files when we run translation tools
lupdate_only{
SOURCES += TestWindow.qml \
           main.qml
}

TRANSLATIONS = app.ts

RESOURCES += qml.qrc

HEADERS += \
    chibisequence.h \
    logger.h \
    ../common/chibitestregistry.h \
    ../common/chibitest.h \
    ../common/testmodule.h
