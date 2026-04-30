QT       += core gui bluetooth



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SettingWindow.cpp \
    aboutmainwindow.cpp \
    btmainwindow.cpp \
    colorslider.cpp \
    main.cpp \
    mainwindow.cpp \
    mypushbutton.cpp

HEADERS += \
    SettingWindow.h \
    aboutmainwindow.h \
    btmainwindow.h \
    colorslider.h \
    mainwindow.h \
    mypushbutton.h \
    update.h

FORMS += \
    btmainwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc

DISTFILES += \
    android_sources/AndroidManifest.xml \
    pic/btmenu.jpg

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android_sources
