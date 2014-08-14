#include "addlabdialog.h"
#include "ui_addlabdialog.h"

AddLabDialog::AddLabDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLabDialog)
{
    ui->setupUi(this);
}

AddLabDialog::~AddLabDialog()
{
    delete ui;
}
