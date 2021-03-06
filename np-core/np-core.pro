#-------------------------------------------------
#
# Project created by QtCreator 2019-03-22T16:04:58
#
#-------------------------------------------------
include(../qmake-target-platform.pri)
include(../qmake-destination-path.pri)

DESTDIR = $$PWD/../binaries/$$DESTINATION_PATH
OBJECTS_DIR = $$PWD/build/$$DESTINATION_PATH/.obj
MOC_DIR = $$PWD/build/$$DESTINATION_PATH/.moc
RCC_DIR = $$PWD/build/$$DESTINATION_PATH/.qrc
UI_DIR = $$PWD/build/$$DESTINATION_PATH/.ui

QT       += sql qml
QT       -= gui

TARGET = np-core
TEMPLATE = lib
CONFIG += c++14

DEFINES += NPCORE_LIBRARY

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
        source/controllers/mastercontroller.cpp \
    source/models/day.cpp \
    source/models/diet.cpp \
    source/models/foodgrp.cpp \
    source/models/foodwgt.cpp \
    source/models/foodnutr.cpp \
    source/models/foodid.cpp \
    source/models/foodsearch.cpp \
    source/models/fooditem.cpp \
    source/models/fooditemlist.cpp \
    source/models/meal.cpp \
    source/database/databasemanager.cpp

HEADERS += \
        source/controllers/mastercontroller.h \
    source/models/day.h \
    source/models/diet.h \
    source/models/foodgrp.h \
    source/models/foodwgt.h \
    source/models/foodnutr.h \
    source/models/foodid.h \
    source/models/meal.h \
    source/np-core_global.h \
    source/models/foodsearch.h \
    source/models/fooditem.h \
    source/models/fooditemlist.h \
    source/database/databasemanager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

message(np-core project dir: $${PWD})
message(np-core output dir: $${DESTDIR})
