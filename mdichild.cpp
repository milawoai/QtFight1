#include "mdichild.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QApplication>
#include <QFileDialog>
#include <QCloseEvent>
#include <QPushButton>
#include <QByteArray>
#include <QTextCodec>
#include <QTextDocumentWriter>
#include <QClipboard>
#include <QMimeData>
#include <QImageReader>
#include <QDateTime>
#include <QTextBlock>

MdiChild::MdiChild(QWidget *parent) :
    QTextEdit(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);// 这样可以在子窗口关闭时销毁这个类的对象
    connect(this,SIGNAL(MousePressFont(QTextCharFormat)),parent,SLOT(getMousePressFont(QTextCharFormat)));
    isUntitled = true; 
}

void MdiChild::newFile()// 新建文件操作
{
   // 设置窗口编号，因为编号一直被保存，所以需要使用静态变量
    static int sequenceNumber = 1;

    // 新建的文档没有被保存过
    isUntitled = true;

    // 将当前文件命名为未命名文档加编号，编号先使用再加1
    curFile = /*QStringLiteral*/tr("UnTitledFile%1.txt").arg(sequenceNumber++);

    // 设置窗口标题，使用[*]可以在文档被更改后在文件名称后显示”*“号
    setWindowTitle(curFile + "[*]" + /*QStringLiteral*/tr(" - MutilEdit"));

    // 当文档被更改时发射contentsChanged()信号，
    // 执行我们的documentWasModified()槽函数
    connect(document(), SIGNAL(contentsChanged()),this, SLOT(documentWasModified()));
}

// 文档被更改时，窗口显示更改状态标志
void MdiChild::documentWasModified()
{
    // 根据文档的isModified()函数的返回值，判断我们编辑器内容是否被更改了
    // 如果被更改了，就要在设置了[*]号的地方显示“*”号，这里我们会在窗口标题中显示
    setWindowModified(document()->isModified());
}

void MdiChild::paste()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if(this->canInsertFromMimeData(mimeData))
    {
        this->insertFromMimeData(mimeData);
    }
}

bool MdiChild::loadFile(const QString &fileName)
{
    // 新建QFile对象
    if (!QFile::exists(fileName))
        return false;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return false;

    QDir *dir = new QDir(fileName);

    thePath = dir->absolutePath();/*dir->dirName();*/

    QString strTemp,strT;
    strTemp = dir->absolutePath();
    strT = dir->dirName();

    int len = strTemp.indexOf(strT);
    thePath = strTemp.left(len);

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (Qt::mightBeRichText(str)) {
        this->setHtml(str);
    } else {
        str = QString::fromLocal8Bit(data);
        this->setPlainText(str);
    }

//    QFile file(fileName);

//    // 只读方式打开文件，出错则提示，并返回false
//    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        QMessageBox::warning(this, tr("MutilEdit"),
//                             tr("Can't read it%1:\n%2.")
//                             .arg(fileName).arg(file.errorString()));
//        return false;
//    }

//    // 新建文本流对象
//    QTextStream in(&file);

//    // 设置鼠标状态为等待状态
//    QApplication::setOverrideCursor(Qt::WaitCursor);

//    // 读取文件的全部文本内容，并添加到编辑器中
//    setPlainText(in.readAll());

//    // 恢复鼠标状态
//    QApplication::restoreOverrideCursor();

    // 设置当前文件
    setCurrentFile(fileName);

    connect(document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));

    return true;
}

 void MdiChild::setCurrentFile(const QString &fileName)
 {
     // canonicalFilePath()可以除去路径中的符号链接，“.”和“..”等符号
     curFile = QFileInfo(fileName).canonicalFilePath();

     // 文件已经被保存过了
     isUntitled = false;

     // 文档没有被更改过
     document()->setModified(false);

     // 窗口不显示被更改标志
     setWindowModified(false);

     // 设置窗口标题，userFriendlyCurrentFile()返回文件名
     setWindowTitle(userFriendlyCurrentFile() + "[*]");
 }


 QString MdiChild::userFriendlyCurrentFile()
 {
     return QFileInfo(curFile).fileName(); // 从文件路径中提取文件名
 }

 void MdiChild::mousePressEvent(QMouseEvent *e)
 {
     QTextCharFormat format = (this->textCursor()).charFormat();

     emit MousePressFont(format);
     QTextEdit::mousePressEvent(e);
 }

 // 保存操作
 bool MdiChild::save()
 {
     if (isUntitled) { // 如果文件未被保存过，则执行另存为操作
         return saveAs();
     } else {
         return saveFile(curFile); //否则直接保存文件
     }
 }

 bool MdiChild::saveAs()
 {
     // 获取文件路径，如果为空，则返回false
     QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"),curFile);
     if (fileName.isEmpty())
         return false;

     return saveFile(fileName); // 否则保存文件
 }

 bool MdiChild::saveFile(const QString &fileName)
 {
     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("MutilEdit"),
                              tr("无法写入文件 %1:\n%2.")
                              .arg(fileName).arg(file.errorString()));
         return false;
     }

//     QTextStream out(&file);
//     QApplication::setOverrideCursor(Qt::WaitCursor);
//     out << toPlainText(); // 以纯文本文件写入
//     QApplication::restoreOverrideCursor();

     QTextDocumentWriter writer(fileName);
     bool success = writer.write(this->document());
     if (success)
         this->document()->setModified(false);
     setCurrentFile(fileName);

//     QTextDocumentWriter writer(fileName);
//     bool success = writer.write(this->document());
//     if (success)
//     {
//         this->document()->setModified(false);
//     }
//     return success;
     return true;
 }

 void MdiChild::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
 {
     QTextCursor cursor = this->textCursor();
     if (!cursor.hasSelection())
     {
         cursor.select(QTextCursor::WordUnderCursor);
     }
     cursor.mergeCharFormat(format);
     this->mergeCurrentCharFormat(format);
 }

 void MdiChild::mergeFormatOnWordOrSelection2(const QTextCharFormat &format)
 {
     QTextCursor cursor = this->textCursor();
//     if (!cursor.hasSelection())
//     {
//         cursor.select(QTextCursor::WordUnderCursor);
//     }
     cursor.mergeCharFormat(format);
     this->mergeCurrentCharFormat(format);
 }

 void MdiChild::closeEvent(QCloseEvent *event)
 {
     if (maybeSave()) { // 如果maybeSave()函数返回true，则关闭窗口
         event->accept();
     } else {   // 否则忽略该事件
         event->ignore();
     }
 }

 void MdiChild::keyPressEvent(QKeyEvent *event)
 {
     if(event->modifiers() == Qt::ControlModifier)
     {
         if(event->key() == Qt::Key_V)
         {
             //QTextCharFormat format = (this->textCursor()).charFormat();
             this->paste();
//             (this->textCursor()).mergeCharFormat(format);
//             this->mergeCurrentCharFormat(format);
         }
         else if(event->key() == Qt::Key_G)
         {
             QTextCharFormat format = (this->textCursor()).charFormat();
             QTextBlockFormat Blockformat = (this->textCursor()).blockFormat();
             this->paste();
             QTextBlockFormat nextBlockformat = (this->textCursor()).blockFormat();
             if(nextBlockformat!=Blockformat)
             {
                 this->append("");
             }
             (this->textCursor()).mergeCharFormat(format);
             (this->textCursor()).setBlockFormat(Blockformat);
             this->mergeCurrentCharFormat(format);

         }
         else QTextEdit::keyPressEvent(event);
     }
     else if(event->key() == Qt::Key_Return)
     {
         //this->insertPlainText("    ");
         QTextBlock textBlock = (this->textCursor()).block();
         QString qs_str = textBlock.text();
         QString qs_insertStr = "";
         int i = 0;
         while(qs_str.at(i).isSpace()||qs_str.at(i) == '\t')
         {
             qs_insertStr+=qs_str.at(i);
             i++;
         }
         if(qs_str.simplified().right(5).contains(":")||qs_str.simplified().right(5).contains("："))
         {
             qs_insertStr+="    ";
         }
         QTextEdit::keyPressEvent(event);
         this->insertPlainText(qs_insertStr);
     }
     else
     {
         QTextEdit::keyPressEvent(event);
     }
 }

 bool MdiChild::canInsertFromMimeData(const QMimeData *source) const
 {
     return source->hasImage() || source->hasUrls() ||
             QTextEdit::canInsertFromMimeData(source);
 }

 void MdiChild::insertFromMimeData(const QMimeData *source)
 {
     if (source->hasImage())
     {
         static int i = 1;
         QDateTime NowDate = QDateTime::currentDateTime();
         QString toDayFile = NowDate.toString("yyyy-MM-dd-hh-mm-ss");
         QString SavePath = thePath+toDayFile+QString("%1.bmp").arg(i++);
         QUrl url(SavePath);
         dropImage(url, qvariant_cast<QImage>(source->imageData()),SavePath);
     }
     else if (source->hasUrls())
     {
         foreach (QUrl url, source->urls())
         {
             QFileInfo info(url.toLocalFile());
             if (QImageReader::supportedImageFormats().contains(info.suffix().toLower().toLatin1()))
                 dropImage(url, QImage(info.filePath()),url.path());
             else
                 dropTextFile(url);
         }
     }
     else
     {
         QTextEdit::insertFromMimeData(source);
     }
 }

 bool MdiChild::maybeSave()
 {
     if (document()->isModified()) { // 如果文档被更改过
         QMessageBox box;
         box.setWindowTitle(tr("MutilEdit"));
         box.setText(tr("Do you want to save the change of %1 ?")
                     .arg(userFriendlyCurrentFile()));
         box.setIcon(QMessageBox::Warning);

         // 添加按钮，QMessageBox::YesRole可以表明这个按钮的行为
         QPushButton *yesBtn = box.addButton(tr("Yes(&Y)"),QMessageBox::YesRole);
         box.addButton(tr("No(&N)"),QMessageBox::NoRole);
         QPushButton *cancelBtn = box.addButton(tr("Cancel"),
                                                QMessageBox::RejectRole);
         box.exec(); // 弹出对话框，让用户选择是否保存修改，或者取消关闭操作
         if (box.clickedButton() == yesBtn)  // 如果用户选择是，则返回保存操作的结果
             return save();
         else if (box.clickedButton() == cancelBtn) // 如果选择取消，则返回false
             return false;
     }
     return true; // 如果文档没有更改过，则直接返回true
 }

 void MdiChild::dropImage(const QUrl &url, const QImage &image,const QString &SavePath)
 {
     if (!image.isNull())
     {
         image.save(SavePath);
         document()->addResource(QTextDocument::ImageResource, url, image);
         textCursor().insertImage(url.toString());
     }
 }

 void MdiChild::dropTextFile(const QUrl &url)
 {
     QFile file(url.toLocalFile());
      if (file.open(QIODevice::ReadOnly | QIODevice::Text))
      {
          textCursor().insertText(file.readAll());
      }
 }



