#ifndef MDICHILD_H
#define MDICHILD_H

#include <QTextEdit>

class MdiChild : public QTextEdit
{
    Q_OBJECT
public:
    explicit MdiChild(QWidget *parent = 0);
    void newFile();
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void mergeFormatOnWordOrSelection2(const QTextCharFormat &format);
    QString userFriendlyCurrentFile();
    QString currentFile(){return curFile;}
protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent* event);
    bool canInsertFromMimeData(const QMimeData * source) const;
    void insertFromMimeData(const QMimeData* source);
signals:

public slots:
    void paste();
private slots:
    void documentWasModified();

private:
    bool maybeSave();
    void dropImage(const QUrl &url, const QImage &image,const QString &SavePath);
    void dropTextFile(const QUrl& url);
    void setCurrentFile(const QString &fileName);
    QString curFile;
    bool isUntitled;
    QString thePath;
};

#endif // MDICHILD_H
