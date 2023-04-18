QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += "ABS=\"\\\"$$PWD\\\"\""

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cc \
    model/cave.cc \
    view/cave_window.cc \
    view/mainwindow.cc \
    model/matrix/operators.cc \
    model/matrix/s21_matrix_oop.cc \
    model/parser.cc

HEADERS += \
    controller/controller.h \
    model/cave.h \
    model/model.h \
    view/cave_window.h \
    view/mainwindow.h \
    model/matrix/s21_matrix_oop.h \
    model/maze.h \
    model/parser.h

FORMS += \
    view/cave.ui \
    view/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
