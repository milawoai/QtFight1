#ifndef DIALOGFORQSYSTEMTRAYICON_H
#define DIALOGFORQSYSTEMTRAYICON_H

#include <QDialog>

namespace Ui {
class DialogForQSystemTrayIcon;
}

class DialogForQSystemTrayIcon : public QDialog
{
    Q_OBJECT

public:
    explicit DialogForQSystemTrayIcon(QWidget *parent = 0);
    ~DialogForQSystemTrayIcon();
signals:
    void SendTrayResult(bool);
private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

private:
    Ui::DialogForQSystemTrayIcon *ui;
};

#endif // DIALOGFORQSYSTEMTRAYICON_H
