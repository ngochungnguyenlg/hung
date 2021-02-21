#ifndef WORKER_H
#define WORKER_H
#include <QThread>
#include <QListWidget>
#include <QString>
#include <fstream>

class Worker : public QObject
{
    std::fstream log;
    Q_OBJECT
signals:
    void progress(int steps);
    void finishedWork();
public slots:
    void doWork(QListWidget* list, QString outPath);
};

#endif // WORKER_H
