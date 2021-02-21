#include "worker.h"
#include "directory.h"
#include <QDebug>
#include <QException>

void Worker::doWork(QListWidget* list,QString outPath){
    int count = 0;
    string log_path=outPath.toStdString()+"/Log/read_list.csv";
    log.open(log_path.c_str(), ios::out);
//    while(log.fail())
//    {
//        log.open(log_path.c_str(), ios::out);
//    }
    for(int i = 0; i < list->count(); ++i){
        if(list->item(i)->checkState() == Qt::Checked){
                QString filepath = list->item(i)->text();
                filepath.replace('/',"\\",Qt::CaseSensitive);
//                qDebug() << filepath;
                makeMockFile *file = new makeMockFile(filepath.toStdString(),outPath.toStdString());
                if(file==nullptr)
                {
                    qDebug()<< "Cannot provide memmory";
                }
                log<<"read this file: "<<filepath.toStdString()<<"\n out path: "<<outPath.toStdString()<<std::endl;
                file->removeOldfile();
                file->makeFileHeader();
                file->readFilename();
                file->merfile();
                delete file;
                count++;
        }
        emit progress(count);
    }
    log.close();
    emit finishedWork();
}
