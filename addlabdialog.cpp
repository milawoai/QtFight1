#include "addlabdialog.h"
#include "ui_addlabdialog.h"
#include <QStandardItemModel>
#include <QDebug>

AddLabDialog::AddLabDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLabDialog)
{
    ui->setupUi(this);
    QStandardItemModel * model  = new QStandardItemModel(ui->LabeltreeView);
    ui->LabeltreeView -> setModel( model );
    model->setHorizontalHeaderLabels(QStringList()<<tr("LabelName"));

}

AddLabDialog::~AddLabDialog()
{
    delete ui;
}

void AddLabDialog::getLabelDisplay(QList<QString> str)
{
    QString strTemp;
    QStandardItemModel * model   = new QStandardItemModel(ui->LabeltreeView);
    QMap<QString, QStandardItem*> map;
    foreach (strTemp, str)
    {
        QStringList temp1 = strTemp.split("\\");
        QString strT;
        bool isfirst = true;
        QStandardItem* regist = NULL;
        QStandardItem* itemProject = NULL;
        foreach(strT,temp1)
        {
            int len = strTemp.indexOf(strT);
            QString strIn = strTemp.left(len+strT.length());

            if(!map.contains(strIn))
            {
                itemProject = new QStandardItem(strT);
                map.insert(strIn,itemProject);
            }
            else
            {
                itemProject = map[strIn];
            }
            if(isfirst)
            {
                model->appendRow(itemProject);
                isfirst = false;
            }
            else
            {
                regist->appendRow(itemProject);
            }
            regist = itemProject;
        }
    }
    ui->LabeltreeView -> setModel( model );
}


void AddLabDialog::on_LabeltreeView_clicked(const QModelIndex &index)
{
    strOut.clear();
    QString str;
    QModelIndex indexP = index;
    QModelIndex indexRegiste = index.parent();


    while(indexP.parent()!= QModelIndex())
    {
        str += QStringLiteral("%1").arg(indexP.data().toString());
        str += "\\";
        indexP = indexP.parent();
    }
    str += QStringLiteral("%1").arg(indexP.data().toString());
    str += "\\";
    qDebug()<<str;
    QStringList temp1 = str.split("\\");
    QStringList::const_iterator constIterator;
    for (constIterator = temp1.constEnd()-1 ; constIterator != temp1.constBegin()-1;
              --constIterator)
    {
        strOut += (*constIterator);
        strOut +="\\";
    }
    ui->textBrowser->setText(strOut);
}

void AddLabDialog::on_Add_clicked()
{
    if(ui->lineEdit->displayText() == QString())
    {
        return;
    }
    else
    {
        QString STRTEM = strOut;
        strOut += ui->lineEdit->displayText();
        if(!haveBeenAdd.contains(strOut))
        {
            QStringList ListStr = strOut.split("\\", QString::SkipEmptyParts);
            emit GoBack(ListStr);
            haveBeenAdd.insert(strOut);
        }
        strOut = STRTEM;

    }
}


void AddLabDialog::on_pushButton_clicked()
{
    emit EndIt();
    this->destroy();
}
