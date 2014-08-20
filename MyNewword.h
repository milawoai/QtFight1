#ifndef MYNEWWORD_H
#define MYNEWWORD_H

#include <QWidget>
#include <QPixmap>
#include <QDir>

namespace Ui {
class NewWord;
}

class NewWord : public QWidget
{
    Q_OBJECT

public:
    explicit NewWord(QWidget *parent = 0);
    explicit NewWord(QPixmap pix,QWidget *parent = 0);
    ~NewWord();

private:
    Ui::NewWord *ui;
    QDir *LanDir;
};

#endif // MYNEWWORD_H
