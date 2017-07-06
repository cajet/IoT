#-------------------------------------------------
#
# Project created by QtCreator 2017-06-29T09:38:44
#
#-------------------------------------------------

QT       += core gui network

TARGET = IOT_SENSOR
TEMPLATE = app

SOURCES += main.cpp\
        widget.cpp \
    rain.cpp \
    secure.cpp

HEADERS  += widget.h \
    include/libwsncomm.h \
    include/node_config.h \
    rain.h \
    secure.h

FORMS    += widget.ui \
    rain.ui \
    secure.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    pic/Thumbs.db \
    pic/human.png \
    pic/rainUmbrella.png \
    pic/safe.png \
    pic/sun.png

LIBS += -L../IOT_SENSOR/lib -lwsncomm
INCLUDEPATH += ../IOT_SENSOR/include

OTHER_FILES += \
    pic/human.png \
    pic/rainUmbrella.png \
    pic/safe.png \
    pic/sun.png \
    pic/Thumbs.db \
    lib/libwsncomm.so \
    IOT_SENSOR.pro.user
