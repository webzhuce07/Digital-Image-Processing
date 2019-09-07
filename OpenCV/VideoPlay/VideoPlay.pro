#**********************************************************************
#
#  Copyright (c) Mr.Bin. All rights reserved.
#  For more information visit: http://blog.csdn.net/webzhuce 
#
#**********************************************************************
TEMPLATE      = app

QT            += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH   += D:/opencv/build/include
CONFIG(debug, debug|release): {
LIBS += -LD:/opencv/build/x64/vc14/lib \
-lopencv_world340d
} else:CONFIG(release, debug|release): {
LIBS += -LD:/opencv/build/x64/vc14/lib \
-lopencv_world340
}
CONFIG       += console
SOURCES       = main.cpp 



