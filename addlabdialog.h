#ifndef ADDLABDIALOG_H
#define ADDLABDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QSet>
#include <QModelIndex>

namespace Ui {
class AddLabDialog;
}

class AddLabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddLabDialog(QWidget *parent = 0);
    ~AddLabDialog();
private slots:
    void getLabelDisplay(QList<QString>);

    void on_LabeltreeView_clicked(const QModelIndex &index);

    void on_Add_clicked();

    void on_pushButton_clicked();

signals:
    void GoBack(QStringList);
    void EndIt();

private:
    Ui::AddLabDialog *ui;
    QString strOut;
    QSet<QString> haveBeenAdd;
    QModelIndex CurrentIndex;
    QStringList QChildList;
};

#endif // ADDLABDIALOG_H
