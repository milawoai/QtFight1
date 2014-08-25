#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mdichild.h"
#include "addlabdialog.h"
#include <QMdiSubWindow>
#include <QFileInfo>
#include <QFileDialog>
#include <QSignalMapper>
#include <QSettings>
#include <QCloseEvent>
#include <QLabel>
#include <QSplashScreen>
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QTime>
#include <QPropertyAnimation>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QTextCodec>
#include <QTextEdit>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QMimeData>
//#ifndef QT_NO_PRINTER
//#include <QPrintDialog>
//#include <QPrinter>
//#include <QPrintPreviewDialog>
#include "MyFileDeal.h"
#include "QSortTheStringList.h"
#include "MyNewword.h"


const QString rsrcPath = ":/myImage/images";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    myAddLab(NULL)
{
    ui->setupUi(this);
    setupTextActions();

    DataDir = new QDir("C:/Users/Administrator/Desktop/data");


//***************背景制造*****************
    int backgroundCount;
    QDir dir="F:/pro/Resource";
    QStringList filter;
    filter<<"*.jpg";
    dir.setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir.entryInfoList(filter));
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    int n = 0;
    if((backgroundCount = fileInfo->count())>0)
    {
         n = qrand()%(backgroundCount);
    }
    QPixmap pix(dir.absoluteFilePath(fileInfo->at(n).fileName()));
    delete fileInfo;

//***************NewWord制造*****************
    QDir dir_B="F:\\pro\\QtFight1\\images\\background";
    dir_B.setNameFilters(filter);
    fileInfo=new QList<QFileInfo>(dir_B.entryInfoList(filter));
    if((backgroundCount = fileInfo->count())>0)
    {
         n = qrand()%(backgroundCount);
    }
    QPixmap pix_B(dir_B.absoluteFilePath(fileInfo->at(n).fileName()));
    delete fileInfo;
    fileInfo = NULL;
    newLabel = new NewWord(pix_B);
    newLabel->show();


    actionSeparator = new QAction(this); // 创建间隔器动作
    actionSeparator->setSeparator(true); // 在其中设置间隔器
    actionCreate = NULL;
    updateMenus();   // 更新菜单
    connect(ui->mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,
            SLOT(updateMenus()));  // 当有活动窗口时更新菜单

    windowMapper = new QSignalMapper(this); // 创建信号映射器
    connect(windowMapper, SIGNAL(mapped(QWidget*)), // 映射器重新发射信号
            this, SLOT(setActiveSubWindow(QWidget*))); // 设置活动窗口

    updateWindowMenu();
    // 更新窗口菜单，并且设置当窗口菜单将要显示的时候更新窗口菜单
    connect(ui->menu_W,SIGNAL(aboutToShow()),this,SLOT(updateWindowMenu()));
    ROOTMenu = ui->menu_L;
    readSettings(); // 初始窗口时读取窗口设置信息
    initWindow(); // 初始化窗口
    ui->mdiArea->resize(pix.size());
    ui->mdiArea->setBackground(pix);

//*******************计时器************
    QTime NowTime = QTime::currentTime();
    int minute = NowTime.minute();
    //int hour = NowTime.hour();
    int second = NowTime.second();

    QDate NowDate = QDate::currentDate();
    QString toDayFile = NowDate.toString("yyyy-MM-dd");
    //qDebug()<<toDayFile;
    fileInfo=new QList<QFileInfo>(DataDir->entryInfoList());
    QFileInfo FileInfoTemp;
    QString FilePath;
    bool CurrentFileExist = false;
    for (int i = 0; i < fileInfo->size(); ++i)
    {
        FileInfoTemp = fileInfo->at(i);
        //qDebug()<<FileInfoTemp.baseName();
        if(FileInfoTemp.baseName()==toDayFile)
        {
            FilePath =FileInfoTemp.absoluteFilePath();
            CurrentFileExist = true;
            break;
        }
    }
    if(CurrentFileExist&&!FilePath.isEmpty())
    {
        QMdiSubWindow *existing = findMdiChild(FilePath);
        if (existing) { // 如果已经存在，则将对应的子窗口设置为活动窗口
            ui->mdiArea->setActiveSubWindow(existing);
            return;
        }
        MdiChild *child = createMdiChild(); // 如果没有打开，则新建子窗口
        if (child->loadFile(FilePath)) {
            child->show();
        } else {
            child->close();
        }
    }
    else
    {
        QFile TodayFile(DataDir->absolutePath()+"\\"+toDayFile+".html");
        TodayFile.open(QIODevice::WriteOnly);
        TodayFile.close();
        MdiChild *child = createMdiChild(); // 如果没有打开，则新建子窗口
        if (child->loadFile(DataDir->absolutePath()+"\\"+toDayFile+".html")) {
            child->show();
        } else {
            child->close();
        }

    }


//    qDebug()<<minute;
//    qDebug()<<hour;
//    qDebug()<<second;
    int Lastsecond = (60 - minute)>0?(60 - minute)*60+second:second;
    id_GetZhengDian = startTimer(Lastsecond*1000);
    //id_GetZhengDian = startTimer(60000);

//*******************生成托盘*****************************
    trayicon = new QSystemTrayIcon(this);
     //创建QIcon对象，参数是图标资源，值为项目的资源文件中图标的地址
     QIcon icon("F:/pro/1.ico");
     trayiconMenu = new QMenu(this);
     //为托盘菜单添加菜单项
     trayiconQuit = trayiconMenu->addAction("quit");
     //为托盘菜单添加分隔符
     trayiconMenu->addSeparator();

     //将创建的QIcon对象作为系统托盘图标
     trayicon->setIcon(icon);
     //显示托盘图标
     trayicon->show();
     //设置系统托盘提示
     trayicon->setToolTip(QStringLiteral("托盘测试"));
     //将创建菜单作为系统托盘菜单
     trayicon->setContextMenu(trayiconMenu);
     //在系统托盘显示气泡消息提示
     trayicon->showMessage(tr("hahaya"), QStringLiteral("托盘测试"), QSystemTrayIcon::Information, 5000);
     //为系统托盘绑定单击信号的槽 即图标激活时
     connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));
     connect(trayiconQuit, SIGNAL(triggered()), this, SLOT(close()));
     connect(trayiconQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

//**********************浮现设置
     QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
     animation->setDuration(1000);
     animation->setStartValue(0);
     animation->setEndValue(1);
     animation->start();


}
MainWindow::~MainWindow()
{
    delete ui;
//    if(actionCreate!=NULL)
//    {
//        delete actionCreate;
//    }
    if(myAddLab!=NULL)
    {
        myAddLab->close();
        delete myAddLab;
        myAddLab=NULL;
    }
    if(newLabel!=NULL)
    {
        newLabel->hide();
        delete newLabel;
        newLabel=NULL;
    }
    if(trayicon!=NULL)
    {
        trayicon->hide();
        delete trayicon;
        trayicon=NULL;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier)
    {
        if(event->key() == Qt::Key_S)
        {
            if(activeMdiChild()&&activeMdiChild()->save())
            {
                ui->statusBar->showMessage(tr("Success saved"),2000);
            }
        }
        else if(event->key() == Qt::Key_H)
        {
            if(this->isVisible())
            {
                this->hide();
            }
            if(!trayicon->isVisible())
            {
               trayicon->show();
           }
        }
        else QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == id_GetZhengDian)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(1000);
        animation->setStartValue(0);
        animation->setEndValue(1);
        animation->start();
        this->show();
        id_Hour = startTimer(3600*1000);
        killTimer(id_GetZhengDian);
    }
    else if(event->timerId() == id_Hour)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(1000);
        animation->setStartValue(0);
        animation->setEndValue(1);
        animation->start();
        this->show();
    }
}

void MainWindow::on_actionNew_triggered()
{
    MdiChild *child = createMdiChild(); // 创建MdiChild
    child->newFile(); // 新建文件
    child->show(); // 显示子窗口
}

void MainWindow::updateMenus()
{
    bool hasMdiChild = (activeMdiChild()!=0);
    ui->actionSave->setEnabled(hasMdiChild);    //设置各个动作是否可用
    ui->actionSaveAs->setEnabled(hasMdiChild);
    ui->actionPaste->setEnabled(hasMdiChild);
    ui->actionClose->setEnabled(hasMdiChild);
    ui->actionCloseAll->setEnabled(hasMdiChild);
    ui->actionTile->setEnabled(hasMdiChild);
    ui->actionCascade->setEnabled(hasMdiChild);
    ui->actionNext->setEnabled(hasMdiChild);
    ui->actionPrevious->setEnabled(hasMdiChild);
    actionSeparator->setVisible(hasMdiChild);   //设置间隔器是否显
    bool hasSelection = (activeMdiChild()
                         && activeMdiChild()->textCursor().hasSelection());

    // 有活动窗口且有被选择的文本，剪切复制才可用
    ui->actionCut->setEnabled(hasSelection);
    ui->actionCopy->setEnabled(hasSelection);

    // 有活动窗口且文档有撤销操作
    ui->actionUndo->setEnabled(activeMdiChild()
                          && activeMdiChild()->document()->isUndoAvailable());

    // 有活动窗口且文档有恢复操作
    ui->actionRedo->setEnabled(activeMdiChild()
                          && activeMdiChild()->document()->isRedoAvailable());

}

MdiChild* MainWindow::activeMdiChild()
{
    if(QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return 0; // 没有活动窗口，直接返回0
}

MdiChild* MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild; // 创建MdiChild部件
    ui->mdiArea->addSubWindow(child); // 向多文档区域添加子窗口，child为中心部件

    // 根据QTextEdit类的是否可以复制信号设置剪切复制动作是否可用
    connect(child,SIGNAL(copyAvailable(bool)),ui->actionCut,
            SLOT(setEnabled(bool)));
    connect(child,SIGNAL(copyAvailable(bool)),ui->actionCopy,
            SLOT(setEnabled(bool)));

    // 根据QTextDocument类的是否可以撤销恢复信号设置撤销恢复动作是否可用
    connect(child->document(),SIGNAL(undoAvailable(bool)),
            ui->actionUndo,SLOT(setEnabled(bool)));
    connect(child->document(),SIGNAL(redoAvailable(bool)),
            ui->actionRedo,SLOT(setEnabled(bool)));

    // 每当编辑器中的光标位置改变，就重新显示行号和列号
    connect(child,SIGNAL(cursorPositionChanged()),this,SLOT(showTextRowAndCol()));
    return child;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this); // 获取文件路径
    if (!fileName.isEmpty()) { // 如果路径不为空，则查看该文件是否已经打开
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing) { // 如果已经存在，则将对应的子窗口设置为活动窗口
            ui->mdiArea->setActiveSubWindow(existing);
            return;
        }

        MdiChild *child = createMdiChild(); // 如果没有打开，则新建子窗口
        if (child->loadFile(fileName)) {
            ui->statusBar->showMessage(tr("打开文件成功"), 2000);
            child->show();
        } else {
            child->close();
        }
    }
}

QMdiSubWindow * MainWindow::findMdiChild(const QString &fileName) // 查找子窗口
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    // 利用foreach语句遍历子窗口列表，如果其文件路径和要查找的路径相同，则返回该窗口
    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

void MainWindow::setActiveSubWindow(QWidget *window) // 设置活动子窗口
{
    if (!window) // 如果传递了窗口部件，则将其设置为活动窗口
        return;
    ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MainWindow::AddToMdiChild(QStringList str)
{
    if(activeMdiChild())
    {
        QString Stem;
        int layers = 0;
        foreach(Stem,str)
        {
            activeMdiChild()->append("");
            for(int i = 0;i<layers;i++)
            {
                activeMdiChild()->insertPlainText("    ");
            }
            activeMdiChild()->insertPlainText("<");
            activeMdiChild()->insertPlainText(Stem);
            activeMdiChild()->insertPlainText(">");

            layers++;
        }
        QList<QString>::iterator strIter = str.end();
        while(strIter!=str.begin())
        {
            strIter--;
            layers--;
            activeMdiChild()->append("");
            for(int i = 0;i<layers;i++)
            {
                activeMdiChild()->insertPlainText("    ");
            }
            activeMdiChild()->insertPlainText("<");
            activeMdiChild()->insertPlainText("\\");
            activeMdiChild()->insertPlainText(*strIter);
            activeMdiChild()->insertPlainText(">");
        }
    }
}

void MainWindow::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
      //单击
      case QSystemTrayIcon::Trigger:
      //双击
      case QSystemTrayIcon::DoubleClick:
          //恢复窗口显示
          this->setWindowState(Qt::WindowActive);
          this->show();
          trayicon->hide();
          break;
      default:
          break;
     }
}

void MainWindow::on_actionSave_triggered()
{
    if(activeMdiChild()&&activeMdiChild()->save())
    {
        ui->statusBar->showMessage(tr("Success saved"),2000);
    }
}

void MainWindow::on_actionExit_triggered()
{
    qApp->closeAllWindows(); // 等价于QApplication::closeAllWindows();
}

void MainWindow::on_actionUndo_triggered()
{
    if(activeMdiChild())
    {
        activeMdiChild()->undo();
    }
}

void MainWindow::on_actionRedo_triggered()
{
    if(activeMdiChild())
    {
        activeMdiChild()->redo();
    }
}

void MainWindow::on_actionCut_triggered()
{
    if(activeMdiChild())
    {
        activeMdiChild()->cut();
    }
}

void MainWindow::on_actionCopy_triggered()
{
    if(activeMdiChild())
    {
        activeMdiChild()->copy();
    }
}

void MainWindow::on_actionPaste_triggered()
{
    if(activeMdiChild())
    {
        activeMdiChild()->paste();
    }
}

void MainWindow::on_actionCloseAll_triggered()
{
    ui->mdiArea->closeAllSubWindows();
}

void MainWindow::on_actionTile_triggered()
{
    ui->mdiArea->tileSubWindows();
}

void MainWindow::on_actionCascade_triggered()
{
     ui->mdiArea->cascadeSubWindows();
}

void MainWindow::on_actionNext_triggered()
{
    ui->mdiArea->activateNextSubWindow();
}

void MainWindow::on_actionPrevious_triggered()
{
     ui->mdiArea->activatePreviousSubWindow();
}

void MainWindow::on_actionClose_triggered()
{
     ui->mdiArea->closeActiveSubWindow();
}


void MainWindow::updateWindowMenu()
{
    ui->menu_W->clear(); // 先清空菜单，然后再添加各个菜单动作
    ui->menu_W->addAction(ui->actionClose);
    ui->menu_W->addAction(ui->actionCloseAll);
    ui->menu_W->addSeparator();
    ui->menu_W->addAction(ui->actionTile);
    ui->menu_W->addAction(ui->actionCascade);
    ui->menu_W->addSeparator();
    ui->menu_W->addAction(ui->actionNext);
    ui->menu_W->addAction(ui->actionPrevious);
    ui->menu_W->addAction(actionSeparator);


    QList<QMdiSubWindow *> windows = ui->mdiArea->subWindowList();
    actionSeparator->setVisible(!windows.isEmpty());

    // 如果有活动窗口，则显示间隔器
    for (int i = 0; i < windows.size(); ++i) { // 遍历各个子窗口
        MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());
        QString text;
        if (i < 9) { // 如果窗口数小于9，则设置编号为快捷键
            text = tr("&%1 %2").arg(i + 1)
                               .arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i + 1)
                              .arg(child->userFriendlyCurrentFile());
        }
        QAction *action  = ui->menu_W->addAction(text); // 添加动作到菜单
        action->setCheckable(true); // 设置动作可以选择

        // 设置当前活动窗口动作为选中状态
        action ->setChecked(child == activeMdiChild());

        // 关联动作的触发信号到信号映射器的map()槽函数上，这个函数会发射mapped()信号
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));

        // 将动作与相应的窗口部件进行映射，在发射mapped()信号时就会以这个窗口部件为参数
        windowMapper->setMapping(action, windows.at(i));
    }
}

void MainWindow::updateLabelMenu(QStringList str)//更新Label列表
{
    if(str.size() == 0)//如没有从子类传入的字符串，就退出
    {
        return;
    }
    QList<QAction*> actionList = ROOTMenu->actions();
    QAction * actiontemp = NULL;
    QAction* result = NULL;
    bool flag = true;
    QAction* record = NULL;
    QAction* ComingIn = NULL;
    QAction* ActionRegister = NULL;//记录根目录的Action
    QStringList::const_iterator constIterator = str.begin();
    foreach (actiontemp, actionList) {
        QString strTemp = actiontemp->text();
        //qDebug()<<strTemp;
        QString trsCon = *(constIterator);
        //qDebug()<<trsCon;
        if(strTemp == trsCon)
        {
            constIterator++;
            flag = false;
            ActionRegister = actiontemp;
            if(str.size()==1)
            {
                return;
            }
            else if(actiontemp->menu()!=NULL)
            {
               ComingIn = actiontemp;
               result = WriterRecio(constIterator,str.end()-1,actiontemp);
               record = actiontemp;
            }
            else
            {
               result = actiontemp;
            }

        }
    }
    if(flag)
    {
        QStringList::const_iterator constIterator = str.begin();
        if(str.size()==1)
        {
             actionCreate = ROOTMenu->addAction(*(constIterator));
        }
        else
        {
            QMenu* First = ROOTMenu;
            while(constIterator!=str.end()-1)
            {
                First = AddRecio(*(constIterator),First);
                constIterator++;
            }
            actionCreate = First->addAction(*(constIterator));
        }
    }
    else
    {
        ROOTMenu->removeAction(ActionRegister);
        if(result->menu()!=NULL)
        {
            QMenu* First = result->menu();
            while(constIterator!=str.end()-1)
            {
                First = AddRecio(*(constIterator),First);
                constIterator++;
            }
            actionCreate =  First->addAction(*(constIterator));

            ROOTMenu->addAction(ComingIn);
        }
        else
        {
            if(record&&record->menu()!=NULL)
            {
                QMenu* First = record->menu();
                First->removeAction(result);
                constIterator = constIterator - 1;
                while(constIterator!=str.end()-1)
                {
                    First = AddRecio(*(constIterator),First);
                    constIterator++;
                }
                actionCreate = First->addAction(*(constIterator));
                ROOTMenu->addAction(ComingIn);
            }
            else
            {
                QMenu* First = ROOTMenu;
                constIterator = str.begin();
                while(constIterator!=str.end()-1)
                {
                    QMenu* newOne = new QMenu(*(constIterator));
                    First->addMenu(newOne);
                    First = newOne;
                    constIterator++;
                }
                actionCreate = First->addAction(*(constIterator));
            }
//            QAction* PAction = result->
        }
    }
    if(actionCreate)
    {
        QSonAction *sonAction = new QSonAction(actionCreate);
        //qDebug()<<str;
        sonAction->setQStringList(str);
        SonActionList.push_back(sonAction);
        connect(actionCreate,SIGNAL(triggered()),sonAction,SLOT(TimeToAction()));
        connect(sonAction,SIGNAL(SonAction(QStringList)),this,SLOT(AddToMdiChild(QStringList)));

        QList<QAction*> actionList = ui->menu_L->actions();
        QList<QString> stringList;
        QAction * actiontemp = NULL;

        foreach (actiontemp, actionList) {
            QString strTemp = actiontemp->text();
            if(strTemp == "Add")
            {
                ;
            }
            else if(actiontemp->menu()!=NULL)
            {
                ChangReadRecio(stringList,actiontemp->menu(),strTemp);
            }
            else
            {
                stringList.push_back(strTemp);
            }
        }
        qSort(stringList.begin(),stringList.end(),StringComPare);
        emit LabelSignal(stringList);

        actiontemp = NULL;
        if(myAddLab)
        {
            myAddLab->update();
        }
    }

}

void MainWindow::closeEvent(QCloseEvent *event) // 关闭事件
{
//    DialogForQSystemTrayIcon *dialogForQSystemTrayIcon = new DialogForQSystemTrayIcon;
//    dialogForQSystemTrayIcon->show();
//    connect(dialogForQSystemTrayIcon,SIGNAL(SendTrayResult(bool)),this,SLOT(setTheTrayResult(bool)));
//    if(TrayResult)
//    {
//        if(trayicon->isVisible())
//         {
//              hide();
//              event->ignore();
//         }
//        else
//        {
//            trayicon->show();
//            hide();
//            event->ignore();
//        }
//    }
//    else
//    {
        ui->mdiArea->closeAllSubWindows(); // 先执行多文档区域的关闭操作
        if(newLabel!=NULL)
        {
            newLabel->hide();
        }
        if (ui->mdiArea->currentSubWindow()) {
            event->ignore(); // 如果还有窗口没有关闭，则忽略该事件
        }
        else {
            writeSettings(); // 在关闭前写入窗口设置
            event->accept();
        }

//    }

}

void MainWindow::ChangReadRecio(QList<QString> &StringList, QMenu *Menu,QString str)
{
    QList<QAction*> actionList = Menu->actions();
    QAction* actiontemp;
    foreach (actiontemp, actionList) {

        if(actiontemp->menu()!=NULL)
        {
            ChangReadRecio(StringList,actiontemp->menu(),str+"\\"+actiontemp->text());
        }
        else
        {
            StringList.push_back(str+"\\"+actiontemp->text());
        }
    }
}

QAction* MainWindow::WriterRecio(QStringList::const_iterator& iter, QStringList::const_iterator iterend, QAction* &Parent)
{
    QList<QAction*> actionList = Parent->menu()->actions();

    QAction* actiontemp;

    if(actionList.empty())
    {
        return Parent;
    }

    if(iter == iterend)
    {
        return Parent;
    }
    bool flag = true;
    foreach (actiontemp, actionList) {
        QString strTemp = actiontemp->text();
        if(strTemp == *(iter))
        {
            flag = false;
            iter++;
            if(actiontemp->menu()!=NULL)
            {
                Parent = actiontemp;
                return WriterRecio(iter++,iterend,Parent);
            }
            else
            {
                return actiontemp;
            }
        }
    }
    if(flag)
    {
       return Parent;
    }
    return NULL;
}

QMenu* MainWindow::AddRecio(QString text, QMenu * Parent)
{
//    Parent->addAction(text);
    QMenu* newOne = new QMenu(text);
    Parent->addMenu(newOne);
    return newOne;
}

void MainWindow::ReadRecio(QMenu * Parent,QString ParPath,int Lays)
{
    QList<QAction*> actionList = Parent->actions();
    QAction * actiontemp = NULL;
    QString Temp = ParPath;
    Lays++;

    foreach (actiontemp, actionList) {
        QString strTemp = actiontemp->text();
        if(actiontemp->menu()!=NULL)
        {
           Temp+=strTemp;
           Temp+='\\';
           ReadRecio(actiontemp->menu(),Temp,Lays);
           Temp = ParPath;
        }
        else
        {
            QSonAction *sonAction = new QSonAction(actiontemp);
            Temp+=strTemp;
            sonAction->setQStringList(Temp.split('\\', QString::SkipEmptyParts));
            SonActionList.push_back(sonAction);
            connect(actiontemp,SIGNAL(triggered()),sonAction,SLOT(TimeToAction()));
            connect(sonAction,SIGNAL(SonAction(QStringList)),this,SLOT(AddToMdiChild(QStringList)));
        }
    }
}

void MainWindow::DeleteAddLabDialog()
{
     myAddLab = NULL;
     qDebug()<<"Dot";
}

void MainWindow::writeSettings() // 写入窗口设置
{
    QSettings settings("mola", "QtFight1");
    settings.setValue("pos", pos());   // 写入位置信息
    settings.setValue("size", size()); // 写入大小信息
    //settings.setValue("Label",);

    QFile file("Label.dat");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    QList<QAction*> actionList = ui->menu_L->actions();
    QList<QString> stringList;
    QAction * actiontemp = NULL;

    foreach (actiontemp, actionList) {
        QString strTemp = actiontemp->text();
        if(strTemp == "Add")
        {
            ;
        }
        else if(actiontemp->menu()!=NULL)
        {
            ChangReadRecio(stringList,actiontemp->menu(),strTemp);
        }
        else
        {
            stringList.push_back(strTemp);
        }
    }
    QString ToWrite;
    foreach (ToWrite,stringList)
    {
        out<<ToWrite<<endl;
    }
}

void MainWindow::readSettings() // 读取窗口设置
{
    QSettings settings("mola", "QtFight1");
    QPoint Mypos;
    QSize Mysize;
    Mypos = settings.value("pos", QPoint(200, 200)).toPoint();
    Mysize = settings.value("size", QSize(400, 400)).toSize();
    move(Mypos);
    resize(Mysize);
}

void MainWindow::showTextRowAndCol() // 显示文本的行号和列号
{
    // 如果有活动窗口，则显示其中光标所在的位置
    if(activeMdiChild()){

        // 因为获取的行号和列号都是从0开始的，所以我们这里进行了加1
        int rowNum = activeMdiChild()->textCursor().blockNumber()+1;
        int colNum = activeMdiChild()->textCursor().columnNumber()+1;

        ui->statusBar->showMessage(tr("%1 col %2 row")
                                   .arg(rowNum).arg(colNum),2000);
    }
}

void MainWindow::ReadLabelMenus()
{

}

void MainWindow::initWindow() // 初始化窗口
{
    setWindowTitle(tr("Muti-Edit"));

    // 我们在工具栏上单击鼠标右键时，可以关闭工具栏
    ui->mainToolBar->setWindowTitle(tr("toolBar"));

    // 当多文档区域的内容超出可视区域后，出现滚动条
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui->statusBar->showMessage(tr("Welcome"));

    QLabel *label = new QLabel(this);
    label->setFrameStyle(QFrame::Box | QFrame::Sunken);
    label->setText(
          tr("Zhou"));
    label->setTextFormat(Qt::RichText); // 标签文本为富文本
    label->setOpenExternalLinks(true);  // 可以打开外部链接
    ui->statusBar->addPermanentWidget(label);
    ui->actionNew->setStatusTip(tr("Create A new File"));
    ui->actionSave->setStatusTip(tr("Save my file"));
    ui->actionSaveAs->setStatusTip(tr("Save as new name"));
    ui->actionExit->setStatusTip(tr("exit my Programe"));
    ui->actionUndo->setStatusTip(tr("Undo"));
    ui->actionRedo->setStatusTip(tr("Redo"));
    ui->actionCut->setStatusTip(tr("Cut"));
    ui->actionCopy->setStatusTip(tr("Copy"));
    ui->actionPaste->setStatusTip(tr("Paste"));
    ui->actionClose->setStatusTip(tr("Close Active Windows"));
    ui->actionCloseAll->setStatusTip(tr("Close All Windows"));
    ui->actionTile->setStatusTip(tr("Title"));
    ui->actionCascade->setStatusTip(tr("Casade"));
    ui->actionNext->setStatusTip(tr("Next"));
    ui->actionPrevious->setStatusTip(tr("previous"));

    QFile file("Label.dat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        updateLabelMenu(line.split('\\'));
    }
}

void MainWindow::on_actionAddLab_triggered()
{
    myAddLab = new AddLabDialog(this);
    connect(this,SIGNAL(LabelSignal(QList<QString>)),myAddLab,SLOT(getLabelDisplay(QList<QString>)));
    connect(myAddLab,SIGNAL(GoBack(QStringList)),this,SLOT(updateLabelMenu(QStringList)));

    //ui->menu_L->menuAction()
    QList<QAction*> actionList = ui->menu_L->actions();
    QList<QString> stringList;
    QAction * actiontemp = NULL;

    foreach (actiontemp, actionList) {
        QString strTemp = actiontemp->text();
        if(strTemp == "Add")
        {
            ;
        }
        else if(actiontemp->menu()!=NULL)
        {
            ChangReadRecio(stringList,actiontemp->menu(),strTemp);
        }
        else
        {
            stringList.push_back(strTemp);
        }
    }
    qSort(stringList.begin(),stringList.end(),StringComPare);
    emit LabelSignal(stringList);

    actiontemp = NULL;
    myAddLab->show();
    connect(myAddLab,SIGNAL(EndIt()),this,SLOT(DeleteAddLabDialog()));
}

void MainWindow::on_actionWordCard_triggered()
{
    if(newLabel!=NULL)
    {
        newLabel->show();
    }
}

void MainWindow::setTheTrayResult(bool result)
{
    TrayResult = result;
}

void MainWindow::on_action_triggered()
{

    if(!trayicon->isVisible())
    {
       trayicon->show();
   }
    this->hide();
}

void MainWindow::setupTextActions()
{
    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    ui->actionTextColor->setIcon(pix);

    QComboBox *comboStyle = new QComboBox(ui->toolBar);
    ui->toolBar->addWidget(comboStyle);
    comboStyle->addItem("Standard");
    comboStyle->addItem("Bullet List (Disc)");
    comboStyle->addItem("Bullet List (Circle)");
    comboStyle->addItem("Bullet List (Square)");
    comboStyle->addItem("Ordered List (Decimal)");
    comboStyle->addItem("Ordered List (Alpha lower)");
    comboStyle->addItem("Ordered List (Alpha upper)");
    comboStyle->addItem("Ordered List (Roman lower)");
    comboStyle->addItem("Ordered List (Roman upper)");
    connect(comboStyle, SIGNAL(activated(int)), this, SLOT(textStyle(int)));

    QFontComboBox *comboFont = new QFontComboBox(ui->toolBar);
    ui->toolBar->addWidget(comboFont);
    connect(comboFont, SIGNAL(activated(QString)), this, SLOT(textFamily(QString)));

    QComboBox *comboSize = new QComboBox(ui->toolBar);
    comboSize->setObjectName("comboSize");
    ui->toolBar->addWidget(comboSize);
    comboSize->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(activated(QString)), this, SLOT(textSize(QString)));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font().pointSize())));
}



void MainWindow::on_actionTextBold_triggered()
{
    if(activeMdiChild())
    {
        QTextCharFormat fmt;
        fmt.setFontWeight(ui->actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
        activeMdiChild()->mergeFormatOnWordOrSelection(fmt);
    }
}

void MainWindow::on_actionTextItalic_triggered()
{
    if(activeMdiChild())
    {
        QTextCharFormat fmt;
        fmt.setFontItalic(ui->actionTextItalic->isChecked());
        activeMdiChild()->mergeFormatOnWordOrSelection(fmt);
    }
}

void MainWindow::on_actionTextUnderline_triggered()
{
    if(activeMdiChild())
    {
        QTextCharFormat fmt;
        fmt.setFontUnderline(ui->actionTextItalic->isChecked());
        activeMdiChild()->mergeFormatOnWordOrSelection(fmt);
    }
}

void MainWindow::on_actionAlignLeft_triggered()
{
    if(activeMdiChild())
    {
        activeMdiChild()->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    }
}

void MainWindow::on_actionAlignCenter_triggered()
{
    if(activeMdiChild())
    {
        activeMdiChild()->setAlignment(Qt::AlignHCenter);
    }
}

void MainWindow::on_actionAlignRight_triggered()
{
    if(activeMdiChild())
    {
        activeMdiChild()->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    }
}

void MainWindow::on_actionAlignJustify_triggered()
{
    if(activeMdiChild())
    {
        activeMdiChild()->setAlignment(Qt::AlignJustify);
    }
}
void MainWindow::textFamily(const QString &f)
{
    if(activeMdiChild())
    {
        QTextCharFormat fmt;
        fmt.setFontFamily(f);
        activeMdiChild()->mergeFormatOnWordOrSelection(fmt);
    }
}

void MainWindow::textStyle(int styleIndex)
{
    if(activeMdiChild())
    {
        QTextCursor cursor = activeMdiChild()->textCursor();
        if (styleIndex != 0) {
            QTextListFormat::Style style = QTextListFormat::ListDisc;
            switch (styleIndex) {
                default:
                case 1:
                    style = QTextListFormat::ListDisc;
                    break;
                case 2:
                    style = QTextListFormat::ListCircle;
                    break;
                case 3:
                    style = QTextListFormat::ListSquare;
                    break;
                case 4:
                    style = QTextListFormat::ListDecimal;
                    break;
                case 5:
                    style = QTextListFormat::ListLowerAlpha;
                    break;
                case 6:
                    style = QTextListFormat::ListUpperAlpha;
                    break;
                case 7:
                    style = QTextListFormat::ListLowerRoman;
                    break;
                case 8:
                    style = QTextListFormat::ListUpperRoman;
                    break;
            }

            cursor.beginEditBlock();

            QTextBlockFormat blockFmt = cursor.blockFormat();

            QTextListFormat listFmt;

            if (cursor.currentList()) {
                listFmt = cursor.currentList()->format();
            } else {
                listFmt.setIndent(blockFmt.indent() + 1);
                blockFmt.setIndent(0);
                cursor.setBlockFormat(blockFmt);
            }

            listFmt.setStyle(style);

            cursor.createList(listFmt);

            cursor.endEditBlock();
        } else {
            // ####
            QTextBlockFormat bfmt;
            bfmt.setObjectIndex(-1);
            cursor.mergeBlockFormat(bfmt);
        }
    }
}

void MainWindow::textSize(const QString &p)
{
    if(activeMdiChild())
    {
        qreal pointSize = p.toFloat();
        if (p.toFloat() > 0) {
            QTextCharFormat fmt;
            fmt.setFontPointSize(pointSize);
            activeMdiChild()->mergeFormatOnWordOrSelection(fmt);
        }
    }
}

void MainWindow::on_actionSaveAs_triggered()
{

}
