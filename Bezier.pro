#-------------------------------------------------
#
# Project created by QtCreator 2018-03-10T18:45:56
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Bezier
TEMPLATE = app
CONFIG += c++11

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
        main.cpp \
        mainwindow.cpp \
    renderwindow.cpp \
    shader.cpp \
    vertex.cpp \
    sceneobject.cpp \
    axis.cpp \
    camera.cpp \
    objectinstance.cpp \
    beziercurve.cpp \
    groundplane.cpp

HEADERS += \
        mainwindow.h \
    renderwindow.h \
    shader.h \
    constants.h \
    vertex.h \
    sceneobject.h \
    axis.h \
    camera.h \
    objectinstance.h \
    beziercurve.h \
    groundplane.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    vertexcolorshader.vert \
    fragmentcolorshader.frag