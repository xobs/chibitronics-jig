! include ( ../../common.pri ) {
  error("Unable to locate common.pri")
}

TEMPLATE += app
QT += gui qml quick widgets sql
CONFIG += debug

SOURCES += main.cpp \
    chibisequence.cpp \
    ../common/chibitestregistry.cpp \
    ../common/chibitest.cpp \
    chibilogger.cpp

# Include QML files when we run translation tools
lupdate_only{
SOURCES += TestWindow.qml \
           main.qml \
           TestItemStyle.qml \
           TestItemBackground.qml
}

TRANSLATIONS = app.ts

RESOURCES += qml.qrc

HEADERS += \
    chibisequence.h \
    ../common/chibitestregistry.h \
    ../common/chibitest.h \
    ../common/testmodule.h \
    chibilogger.h
