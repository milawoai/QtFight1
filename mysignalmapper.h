#ifndef MYSIGNALMAPPER_H
#define MYSIGNALMAPPER_H

#include <QSignalMapper>

class MySignalMapper : public QSignalMapper
{
    Q_OBJECT
public:
    explicit MySignalMapper(QObject *parent = 0);
//    void setstrToDisplay(const QString&);

signals:
//    void SignaLabal(QString);
public slots:
//    void DefinalText(QString);
private:
//    QString strqToDisplay;
};

#endif // MYSIGNALMAPPER_H
