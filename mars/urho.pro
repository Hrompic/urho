TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#DEFINES += __ANDROID_
CONFIG -= qt

SOURCES += \
        main.cpp
LIBS += -lUrho3D
