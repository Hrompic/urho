TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#DEFINES += __ANDROID_
CONFIG -= qt
INCLUDEPATH += /usr/local/include/Urho3D/ThirdParty/
SOURCES += \
        main.cpp
LIBS += -lUrho3D
