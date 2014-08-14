#include "qsonaction.h"

//QSonAction::QSonAction(QObject *parent) :
//    QAction(parent)
//{
//}

QSonAction::QSonAction(QAction *parent):QAction(parent)
{
    action = parent;
}

void QSonAction::TimeToAction()
{
    QAction::trigger();
    emit SonAction(strList);
}

void QSonAction::setQStringList(QStringList Pathstr)
{
    strList = Pathstr;
}

