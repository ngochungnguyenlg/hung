#include "gmockgen.h"

#include <QApplication>

extern QString outDir;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GMockGen w;
    outDir = w.getOutputDir();
    w.show();
    return a.exec();
}
