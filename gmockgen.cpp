#include "gmockgen.h"
#include "ui_gmockgen.h"
#include "directory.h"
#include <QDirIterator>
#include <QtAlgorithms>
#include <QMessageBox>
#include <QDesktopServices>

extern QString outDir;
extern std::vector<std::string> exceptionFlags;
static bool cppCheckFlag = true;
static bool hppCheckFlag = true;
static QString progressLabel;
static int total=0;

typedef struct {
    QString path;
    QString name;
    QString tail;
} FileInfo_t;

bool compareFilePaths(const QFileInfo &s1, const QFileInfo  &s2)
{
    return s1.baseName() < s2.baseName();
}

GMockGen::GMockGen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GMockGen)
    ,thread(new QThread(this))
    ,worker(new Worker())
    ,PreviousPath(QDir::currentPath())

{
    ui->setupUi(this);
    this->setFixedSize(837,362);
    connect(ui->browserfile, &QPushButton::clicked,this, &GMockGen::openFile);
    connect(ui->browserfolder, &QPushButton::clicked,this, &GMockGen::openFolder);
    connect(ui->browser, &QPushButton::clicked,this, &GMockGen::chooseOutputFolder);
    connect(ui->cppcheck,&QCheckBox::stateChanged,this,&GMockGen::cppCheck);
    connect(ui->hppcheck,&QCheckBox::stateChanged,this,&GMockGen::hppCheck);
    connect(ui->mockbutton,&QPushButton::clicked,this,&GMockGen::mockGen);
    connect(ui->cancelbutton,&QPushButton::clicked,this,&GMockGen::cancel);
    connect(ui->listWidget,&QListWidget::itemChanged,this,&GMockGen::updatetotal);
    worker->moveToThread(thread);
    connect(this,&GMockGen::doWork,worker,&Worker::doWork);
    connect(worker,&Worker::progress,this,&GMockGen::progress);
    connect(worker,&Worker::finishedWork,this,&GMockGen::doneWork);

    ui->OutputUrl->setReadOnly(true);
    this->outputDir = QDir::currentPath() + "/output";
    ui->OutputUrl->setText(outputDir);
}

GMockGen::~GMockGen()
{
    worker->deleteLater();
    thread->exit();
    delete ui;
}

void GMockGen::openFile(){
    QStringList files = QFileDialog::getOpenFileNames(
                this,
                "Choose Files",
                this->PreviousPath,
                "Sources (*.hpp *.h *.cpp *.c)");
    tarfilelist.clear();
    filelist.clear();
    if(files.size()>0){
        while(ui->listWidget->count()>0)
        {
            ui->listWidget->takeItem(0);
        }
        ui->listWidget->addItems(files);
        for(int i = 0; i < ui->listWidget->count(); ++i){
            ui->listWidget->item(i)->setFlags(ui->listWidget->item(i)->flags() | Qt::ItemIsUserCheckable);
            ui->listWidget->item(i)->setCheckState(Qt::Checked);
        }
    }
    ui->hppcheck->setCheckState(Qt::Unchecked);
    ui->cppcheck->setCheckState(Qt::Unchecked);
    hppCheckFlag = false;
    cppCheckFlag = false;
    total = 0;
    for(int i = 0; i < ui->listWidget->count(); ++i){
        if(ui->listWidget->item(i)->checkState() == Qt::Checked){
            total++;
        }
    }
    progressLabel = QString("Progress: %1/%2").arg(0).arg(total);
    ui->progressLabel->setText(progressLabel);
}

void GMockGen::openFolder(){
    hppCheckFlag = true;
    cppCheckFlag = true;

    QString dir;

    dir = QFileDialog::getExistingDirectory(this, tr("Choose Folder"),
                                            this->PreviousPath,
                                            QFileDialog::ShowDirsOnly
                                            | QFileDialog::DontResolveSymlinks);

    if(dir.size() < 2)
        return;
    this->PreviousPath = dir;
    QDirIterator it(dir,{"*.h", "*.hpp","*.c","*.cpp"}, QDir::Files,QDirIterator::Subdirectories);
    tarfilelist.clear();
    filelist.clear();
    while (it.hasNext()) {
        if(it.fileInfo().completeSuffix() != "pb.h")
            tarfilelist.push_back(it.fileInfo());
        it.next();
    }
    if(tarfilelist.size()>0){
        tarfilelist.removeFirst();
        sort(tarfilelist.begin(),tarfilelist.end(),compareFilePaths);
        for(auto it : tarfilelist ){
            filelist.push_back(it.filePath());
        }
        while(ui->listWidget->count()>0)
        {
            ui->listWidget->takeItem(0);
        }
        ui->listWidget->addItems(filelist);
        for(int i = 0; i < ui->listWidget->count(); ++i){
            ui->listWidget->item(i)->setFlags(ui->listWidget->item(i)->flags() | Qt::ItemIsUserCheckable);
            ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
        }
    }
    total = 0;
    for(int i = 0; i < ui->listWidget->count(); ++i){
        if(ui->listWidget->item(i)->checkState() == Qt::Checked){
            total++;
        }
    }
    progressLabel = QString("Progress: %1/%2").arg(0).arg(total);
    ui->progressLabel->setText(progressLabel);
}

void GMockGen::cppCheck(){
    if(cppCheckFlag == true){
        hppCheckFlag = false;
        for(int i = 0; i < ui->listWidget->count(); ++i){
            ui->listWidget->item(i)->setFlags(ui->listWidget->item(i)->flags() | Qt::ItemIsUserCheckable);
            ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
        }
        if(ui->cppcheck->checkState() == Qt::Checked ){
            ui->hppcheck->setCheckState(Qt::Unchecked);
            for(int i = 0; i < ui->listWidget->count(); ++i){
                if(i != (ui->listWidget->count() -1) && tarfilelist.at(i).baseName() == tarfilelist.at(i+1).baseName() \
                        && tarfilelist.at(i).path() == tarfilelist.at(i+1).path()){
                    if(tarfilelist.at(i).suffix() == "c" || tarfilelist.at(i).suffix() == "cpp")
                        ui->listWidget->item(i)->setCheckState(Qt::Checked);
                    else
                        ui->listWidget->item(i+1)->setCheckState(Qt::Checked);
                    i++;
                }else{
                    ui->listWidget->item(i)->setCheckState(Qt::Checked);
                }
            }
        }
        hppCheckFlag = true;
    }
    total = 0;
    for(int i = 0; i < ui->listWidget->count(); ++i){
        if(ui->listWidget->item(i)->checkState() == Qt::Checked){
            total++;
        }
    }
    progressLabel = QString("Progress: %1/%2").arg(0).arg(total);
    ui->progressLabel->setText(progressLabel);
}

void GMockGen::hppCheck(){
    if(hppCheckFlag == true){
        cppCheckFlag = false;
        for(int i = 0; i < ui->listWidget->count(); ++i){
            ui->listWidget->item(i)->setFlags(ui->listWidget->item(i)->flags() | Qt::ItemIsUserCheckable);
            ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
        }
        if(ui->hppcheck->checkState() == Qt::Checked ){
            ui->cppcheck->setCheckState(Qt::Unchecked);
            for(int i = 0; i < ui->listWidget->count(); ++i){
                if(i != (ui->listWidget->count() -1) && tarfilelist.at(i).baseName() == tarfilelist.at(i+1).baseName() \
                        && tarfilelist.at(i).path() == tarfilelist.at(i+1).path()){
                    if(tarfilelist.at(i).suffix() == "h" || tarfilelist.at(i).suffix() == "hpp")
                        ui->listWidget->item(i)->setCheckState(Qt::Checked);
                    else
                        ui->listWidget->item(i+1)->setCheckState(Qt::Checked);
                    i++;
                }else{
                    ui->listWidget->item(i)->setCheckState(Qt::Checked);
                }
            }
        }
        cppCheckFlag = true;
    }
    total = 0;
    for(int i = 0; i < ui->listWidget->count(); ++i){
        if(ui->listWidget->item(i)->checkState() == Qt::Checked){
            total++;
        }
    }
    progressLabel = QString("Progress: %1/%2").arg(0).arg(total);
    ui->progressLabel->setText(progressLabel);
}

void GMockGen::chooseOutputFolder(){

    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Output Folder"),
                                                    QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dir.size() < 2)
        dir =  QDir::currentPath();
    this->outputDir = dir + "/output";
    outDir=this->outputDir;
    ui->OutputUrl->setText(outputDir);
}


void GMockGen::mockGen(){
    if(total > 0){
        thread->start();
        ui->progressBar->setMaximum(total);
        emit doWork(ui->listWidget,this->outputDir);
        ui->cancelbutton->setEnabled(true);
        enableWidgets(false);
    }else{
        QMessageBox msgBox;
        msgBox.setText("Please select at least one file!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void GMockGen::progress(int steps){
    ui->progressBar->setValue(steps);
    progressLabel = QString("Progress: %1/%2").arg(steps).arg(total);
    ui->progressLabel->setText(progressLabel);
}
void GMockGen::doneWork(){
    enableWidgets(true);
    ui->cancelbutton->setEnabled(false);
    thread->terminate();
    QMessageBox msgBox;
    msgBox.setText("Do you want to open output folder?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Discard);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if(msgBox.exec() == QMessageBox::Ok ){
        QDesktopServices::openUrl(QUrl(this->outputDir, QUrl::TolerantMode));
    }
}

void GMockGen::cancel(){
    thread->terminate();
    enableWidgets(true);
    ui->progressBar->setMaximum(0);
}

void GMockGen::enableWidgets(bool isEnable){
    ui->listWidget->setEnabled(isEnable);
    ui->OutputUrl->setEnabled(isEnable);
    ui->browser->setEnabled(isEnable);
    ui->browserfile->setEnabled(isEnable);
    ui->browserfolder->setEnabled(isEnable);
    ui->cppcheck->setEnabled(isEnable);
    ui->hppcheck->setEnabled(isEnable);
    ui->mockbutton->setEnabled(isEnable);
    ui->cancelbutton->setEnabled(!isEnable);
}

void GMockGen::updatetotal(QListWidgetItem *item){
    total = 0;
    for(int i = 0; i < ui->listWidget->count(); ++i){
        if(ui->listWidget->item(i)->checkState() == Qt::Checked){
            total++;
        }
    }
    progressLabel = QString("Progress: %1/%2").arg(0).arg(total);
    ui->progressLabel->setText(progressLabel);
}
