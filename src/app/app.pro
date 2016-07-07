! include ( ../../common.pri ) {
  error("Unable to locate common.pri")
}

TEMPLATE += app
QT += gui qml quick widgets
CONFIG += debug

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    chibisequence.cpp \
    ../common/chibitestregistry.cpp \
    ../common/chibitest.cpp

RESOURCES += qml.qrc

HEADERS += \
    chibisequence.h \
    ../common/chibitestregistry.h \
    ../common/chibitest.h

