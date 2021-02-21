#ifndef GMOCKGEN_H
#define GMOCKGEN_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <QListWidget>
#include "directory.h"
#include "worker.h"
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class GMockGen; }
QT_END_NAMESPACE

class GMockGen : public QMainWindow
{
    Q_OBJECT
public:
    GMockGen(QWidget *parent = nullptr);
    QString getOutputDir(){
        return outputDir;
    }
    void enableWidgets(bool isEnable);
    ~GMockGen();
public slots:
    void openFile();
    void openFolder();
    void chooseOutputFolder();
    void cppCheck();
    void hppCheck();
    void mockGen();
    void progress(int steps);
    void doneWork();
    void cancel();
    void updatetotal(QListWidgetItem *item);
signals:
    void doWork(QListWidget* list, QString outPath);

private:
    QString outputDir;
    Ui::GMockGen *ui;
    QFileInfoList tarfilelist;
    QStringList filelist;
    QThread* thread;
    Worker* worker;
    QString PreviousPath;
};

#endif // GMOCKGEN_H
