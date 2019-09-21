#**********************************************************************

#Copyright (c) Mr.Bin. All rights reserved.
#For more information visit: http://blog.csdn.net/webzhuce

#**********************************************************************

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyOpenCV
TEMPLATE = app

SOURCES += main.cpp \
	levelset.cpp
HEADERS += levelset.h

INCLUDEPATH   += H:/OpenCV/opencv/build/include
CONFIG(debug, debug|release): {
LIBS += -LH:/OpenCV/opencv/build/x64/vc14/lib \
-lopencv_world340d
} else:CONFIG(release, debug|release): {
LIBS += -LH:/OpenCV/opencv/build/x64/vc14/lib \
-lopencv_world340
}
