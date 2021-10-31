#-------------------------------------------------
#
# Project created by QtCreator 2021-06-28T09:23:56
#
#-------------------------------------------------

QT       += core gui
QT       += sql #加数据库模块
QT       += network #加网络模�?

CONFIG   += c++11 #支持C++11语法

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartLockSystem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    opencvtools.cpp \
    logindialog.cpp \
    userinfoentity.cpp \
    userinfodao.cpp \
    daofactory.cpp \
    personentity.cpp \
    persondao.cpp

HEADERS  += mainwindow.h \
    opencvtools.h \
    logindialog.h \
    userinfoentity.h \
    userinfodao.h \
    daofactory.h \
    personentity.h \
    persondao.h

FORMS    += mainwindow.ui \
    logindialog.ui

INCLUDEPATH += E:/opencv3.4-install/install/include #头文件路�?
INCLUDEPATH += E:/opencv3.4-install/install/include/opencv #头文件路�?
INCLUDEPATH += E:/opencv3.4-install/install/include/opencv2 #头文件路�?
LIBS += E:/opencv3.4-install/install/x86/mingw/lib/libopencv_*.a #库文件路�?
