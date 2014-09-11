#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QSystemTrayIcon>
#include "Dialogforqsystemtrayicon.h"
#include "mysignalmapper.h"
#include "qsonaction.h"
#include "addlabdialog.h"

namespace Ui {
class MainWindow;
}

class MdiChild;
class QMdiSubWindow;
class QSignalMapper;
class NewWord;
class QDir;
class QAction;
class QComboBox;
class QFontComboBox;
class QTextEdit;
class QTextCharFormat;
class QMenu;
class QPrinter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event);
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *event);  // 关闭事件
    void ChangReadRecio(QList<QString> &StringList,QMenu* Menu,QString str);
    QAction* MainWindow::WriterRecio(QStringList::const_iterator& iter, QStringList::const_iterator iterend, QAction* &Parent);
    QMenu* AddRecio(QString,QMenu*);
    void ReadRecio(QMenu * Parent,QString ParPath,int Lays);
signals:
    void LabelSignal(QList<QString>);

private slots:

    void getMousePressFont(const QTextCharFormat& format);

    void textFamily(const QString &f);

    void DeleteAddLabDialog();

    void on_actionNew_triggered();

    void updateMenus();           //更新菜单

    MdiChild *createMdiChild(); //创建子窗口

    void setActiveSubWindow(QWidget *window); // 设置活动子窗口

    void AddToMdiChild(QStringList);

    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCloseAll_triggered();

    void on_actionTile_triggered();

    void on_actionCascade_triggered();

    void on_actionNext_triggered();

    void on_actionPrevious_triggered();

    void updateWindowMenu();

    void updateLabelMenu(QStringList);

    void on_actionClose_triggered();

    void showTextRowAndCol(); // 显示文本的行号和列号

    void ReadLabelMenus();

    void on_actionAddLab_triggered();

    void on_actionWordCard_triggered();

    void setTheTrayResult(bool);

    void on_action_triggered();

    void on_actionTextBold_triggered();

    void on_actionTextItalic_triggered();

    void on_actionTextUnderline_triggered();

    void on_actionAlignLeft_triggered();

    void on_actionAlignCenter_triggered();

    void on_actionAlignRight_triggered();

    void on_actionAlignJustify_triggered();

    void textSize(const QString &p);

    void textStyle(int styleIndex);

    void on_actionSaveAs_triggered();

    void on_actionTextColor_triggered();

    void on_actionSetColor_triggered();

    void on_actionDefColor_triggered();

    void on_action_4_triggered();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_actionW_triggered();

private:

    Ui::MainWindow *ui;
    QAction *actionSeparator;    //分隔符
    MdiChild *activeMdiChild(); //活动窗口
    QMdiSubWindow *findMdiChild(const QString &fileName); // 查找子窗口
    QSignalMapper *windowMapper;
    MySignalMapper *LabelMapper;
    QStringList LabelSetting;
    QString strToDisplay;
    QAction *actionCreate;
    QMenu* ROOTMenu;

    QComboBox *comboStyle;
    QFontComboBox *comboFont;
    QComboBox *comboSize;

    //QMap<QAction*,QString> ActionToString;
    QList<QSonAction*> SonActionList;
    AddLabDialog* myAddLab;
    NewWord *newLabel;
    int id_GetZhengDian;
    int  id_Hour;
    QSystemTrayIcon *trayicon;
    QMenu *trayiconMenu;

    QColor posColor;

    bool TrayResult;

    void readSettings();  // 读取窗口设置
    void writeSettings(); // 写入窗口设置
    void initWindow(); // 初始化窗口

    QAction* trayiconQuit;

    void setupTextActions();
    void colorChanged(const QColor &c);

    //文件路径
    QString dataPath;//存放每日记录
    QString targetPath;//存放目标文件
    QString backgroundPath;//背景图文件
    QString backgroundPath2;//背景图文件
};

#endif // MAINWINDOW_H
