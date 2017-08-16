#include <QCoreApplication>
#include "cancollector.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CanCollector test;
    test.start();
    return a.exec();
}
