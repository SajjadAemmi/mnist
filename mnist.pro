#-------------------------------------------------
#
# Project created by QtCreator 2019-08-20T17:46:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mnist
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

CONFIG += \
        c++11

HEADERS += \
        mainwindow.h

SOURCES += \
        main.cpp \
        mainwindow.cpp

FORMS += \
        mainwindow.ui

RESOURCES += \
        mnist.onnx \
        3.jpg


INCLUDEPATH += /usr/local/include/opencv4
LIBS += -L/usr/local/lib \
        -lopencv_imgcodecs \
        -lopencv_imgproc \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_shape \
        -lopencv_videoio \
        -lopencv_dnn \
#        -lopencv_video \
#        -lopencv_features2d \
#        -lopencv_calib3d \
#        -lopencv_objdetect \
#        -lopencv_stitching

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
