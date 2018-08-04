#-------------------------------------------------
#
# Project created by QtCreator 2018-04-18T21:22:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyOpenCV
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    utility.h

FORMS    += mainwindow.ui

INCLUDEPATH   += D:/opencv/build/include
CONFIG(debug, debug|release): {
LIBS += -LD:/opencv/build/x64/vc14/lib \
-lopencv_world340d
} else:CONFIG(release, debug|release): {
LIBS += -LD:/opencv/build/x64/vc14/lib \
-lopencv_world340
}


