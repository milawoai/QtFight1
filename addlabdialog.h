#ifndef ADDLABDIALOG_H
#define ADDLABDIALOG_H

#include <QDialog>

namespace Ui {
class AddLabDialog;
}

class AddLabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddLabDialog(QWidget *parent = 0);
    ~AddLabDialog();

private:
    Ui::AddLabDialog *ui;
};

#endif // ADDLABDIALOG_H
