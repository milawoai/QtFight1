#-------------------------------------------------
#
# Project created by QtCreator 2014-07-09T18:57:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtFight1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mdichild.cpp \
    addlabdialog.cpp \
    QSortTheString.cpp \
    mysignalmapper.cpp \
    mymdisubwindow.cpp \
    qsonaction.cpp \
    MyFileDeal.cpp

HEADERS  += mainwindow.h \
    mdichild.h \
    addlabdialog.h \
    QSortTheStringList.h \
    mysignalmapper.h \
    mymdisubwindow.h \
    qsonaction.h \
    MyFileDeal.h

FORMS    += mainwindow.ui \
    addlabdialog.ui

RESOURCES += \
    myImage.qrc

OTHER_FILES += \
    readMe.txt
