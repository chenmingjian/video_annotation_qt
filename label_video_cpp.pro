#-------------------------------------------------
#
# Project created by QtCreator 2019-07-24T17:52:33
#
#-------------------------------------------------

QT       += core testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = video_label
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

CONFIG += c++11

SOURCES += \
        easyseek.cpp \
        main.cpp \
        mainwindow.cpp \
        tools.cpp

HEADERS += \
        easyseek.h \
        mainwindow.h \
    tools.hpp

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


LIBS       += -lVLCQtCore -lVLCQtWidgets


# Edit below for custom library location
LIBS       += -LC:/Users/chen/Desktop/VLC-Qt_1.1.0_win64_msvc2015/lib -lVLCQtCore -lVLCQtWidgets
INCLUDEPATH += C:/Users/chen/Desktop/VLC-Qt_1.1.0_win64_msvc2015/include

RC_ICONS = "logo.ico"

RESOURCES += \
    image.qrc
