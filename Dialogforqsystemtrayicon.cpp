#include "Dialogforqsystemtrayicon.h"
#include "ui_dialogforqsystemtrayicon.h"

DialogForQSystemTrayIcon::DialogForQSystemTrayIcon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogForQSystemTrayIcon)
{
    ui->setupUi(this);
}

DialogForQSystemTrayIcon::~DialogForQSystemTrayIcon()
{
    delete ui;
}

void DialogForQSystemTrayIcon::on_pushButton_clicked()
{
    emit SendTrayResult(true);
    this->destroy();
}

void DialogForQSystemTrayIcon::on_pushButton_2_clicked()
{
    emit SendTrayResult(false);
    this->destroy();
}
