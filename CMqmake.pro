QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    config.cpp \
    delegate.cpp \
    main.cpp \
    widget.cpp \
    glwidget.cpp \
    model/train.cpp \
    model/robject.cpp \
    model/manager.cpp \
    model/position.cpp \
    model/platform.cpp \
    model/animation.cpp \
    model/transborder.cpp \
    model/math/vecmath.cpp

HEADERS += \
    config.h \
    delegate.h \
    widget.h \
    glwidget.h \
    model/train.h \
    model/robject.h \
    model/manager.h \
    model/position.h \
    model/platform.h \
    model/animation.h \
    model/transborder.h \
    model/math/vecmath.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
