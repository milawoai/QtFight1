#ifndef QSONACTION_H
#define QSONACTION_H

#include <QAction>
#include <QString>
#include <QStringList>

class QSonAction : public QAction
{
    Q_OBJECT
public:
    //explicit QSonAction(QObject *parent = 0);
    explicit QSonAction(QAction *parent = 0);
    void setQStringList(QStringList Pathstr);

signals:
    void SonAction(QStringList);
public slots:
    void TimeToAction();
private:
        QAction *action;
        QStringList strList;

};

#endif // QSONACTION_H
