QT       += core gui charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    RCSApp.cpp \
    createwindow.cpp \
    formwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    radoDevice.cpp \
    scanData.cpp \
    sensor.cpp \
    user.cpp

HEADERS += \
    RCS App \
    RCSApp.h \
    createwindow.h \
    formwindow.h \
    mainwindow.h \
    radoDevice.h \
    scanData.h \
    sensor.h \
    user.h

FORMS += \
    createwindow.ui \
    formwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += sql

RESOURCES += \
    assets.qrc
