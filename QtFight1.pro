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
    MyFileDeal.cpp \
    MyNewword.cpp \
    Dialogforqsystemtrayicon.cpp

HEADERS  += mainwindow.h \
    mdichild.h \
    addlabdialog.h \
    QSortTheStringList.h \
    mysignalmapper.h \
    mymdisubwindow.h \
    qsonaction.h \
    MyFileDeal.h \
    MyNewword.h \
    Dialogforqsystemtrayicon.h

FORMS    += mainwindow.ui \
    addlabdialog.ui \
    NewWord.ui \
    newword.ui \
    dialogforqsystemtrayicon.ui

RESOURCES += \
    myImage.qrc

OTHER_FILES += \
    readMe.txt \
    myapp.rc

RC_FILE = myapp.rc
