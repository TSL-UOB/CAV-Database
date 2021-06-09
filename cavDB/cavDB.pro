QT -= gui
QT += sql
#QT += core sql
QT += core
QT += core-private

#CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG += console
#CONFIG += staticlib c++11 #GC10/1/18

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    assertions.cpp \
    db_connect.cpp \
    db_tables.cpp \
    initmap.cpp \
    lanelettxt.cpp \
    laneletvector.cpp \
        main.cpp \
    readsimlog.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    lanelettxt.h \
    uepostgis.h \
    readsimlog.h

#TARGET = testQT

DISTFILES += \
    qt512.pro.user