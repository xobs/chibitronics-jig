# Add more folders to ship with the application, here
folder_01.source = qml/chibitronics-jig
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    chibisequence.cpp \
    chibitest.cpp \
    setpower.cpp \
    setvoltage.cpp \
    setmicrodrive.cpp \
    unexportgpio.cpp \
    setgpio.cpp \
    delay.cpp \
    header.cpp \
    finished.cpp \
    programsticker.cpp \
    verifysticker.cpp \
    setstickerfuse.cpp \
    testaudio.cpp \
    testled.cpp \
    teststicker.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick1applicationviewer/qtquick1applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    chibisequence.h \
    chibitest.h \
    setpower.h \
    setvoltage.h \
    setmicrodrive.h \
    unexportgpio.h \
    setgpio.h \
    delay.h \
    header.h \
    finished.h \
    programsticker.h \
    verifysticker.h \
    setstickerfuse.h \
    testaudio.h \
    testled.h \
    teststicker.h
