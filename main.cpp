#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QDebug>
#include <QCoreApplication>
#include <QTime>
#include "MyFileDeal.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); 
//    QDir dir=directoryOf("Resource");
//    QStringList filter;
//    filter<<"*.jpg";
//    dir.setNameFilters(filter);
//    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir.entryInfoList(filter));
//    QTime t;
//    t= QTime::currentTime();
//    qsrand(t.msec()+t.second()*1000);
//    int n = qrand()%(fileInfo->count());
//    QPixmap pix(directoryOf("Resource").absoluteFilePath(fileInfo->at(n).fileName()));
//    QSplashScreen splash(pix);
//    splash.resize(pix.size());
//    splash.show();
//    a.processEvents();
    //system("pause");
    MainWindow w;
    w.show();
    //splash.finish(&w);

    return a.exec();
}
