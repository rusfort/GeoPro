#-------------------------------------------------
#
# Project created by QtCreator 2022-01-09T11:55:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeoPro
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    angle.cpp \
    circle.cpp \
    line.cpp \
        main.cpp \
        geopro.cpp \
    obj_menu.cpp \
    point.cpp \
    ray.cpp \
    segment.cpp \
    triangle.cpp \
    widget.cpp \
    geometry_main.cpp

HEADERS += \
    angle.h \
    circle.h \
        geopro.h \
    line.h \
    obj_menu.h \
    point.h \
    ray.h \
    segment.h \
    triangle.h \
    widget.h \
    geometry_main.h \
    service.h

FORMS += \
        geopro.ui \
        obj_menu.ui

RESOURCES += \
    res.qrc
