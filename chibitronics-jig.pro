TEMPLATE += app
QT += gui qml quick widgets
CONFIG += debug

QMAKE_LFLAGS += -L/opt/etnaviv/lib

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    chibisequence.cpp \
    chibitestregistry.cpp \
    delay.cpp \
    header.cpp \
    setpower.cpp \
    chibitest.cpp

RESOURCES += qml.qrc

HEADERS += \
    chibisequence.h \
    chibitestregistry.h \
    chibitest.h

