#include <QPainter>
#include <QPalette>
#include "MyNewword.h"
#include "ui_newword.h"

NewWord::NewWord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewWord)
{
    setAttribute(Qt::WA_DeleteOnClose);// 这样可以在子窗口关闭时销毁这个类的对象
    ui->setupUi(this);
}

NewWord::NewWord(QPixmap pix, QWidget *parent):
        QWidget(parent),
        ui(new Ui::NewWord)
{
    ui->setupUi(this);
    //setAttribute(Qt::WA_TranslucentBackground);//;透明
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint|Qt::WindowStaysOnTopHint);
    move(QPoint(0, 0));
    resize(QSize(400,300));
    setAutoFillBackground(true);   // 这个属性一定要设置
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(pix.scaled(size(), Qt::IgnoreAspectRatio,
                            Qt::SmoothTransformation)));
    setPalette(pal);

    LanDir = new QDir("C:/Users/Administrator/Desktop/data/Lan");
}

NewWord::~NewWord()
{
    delete ui;
}
