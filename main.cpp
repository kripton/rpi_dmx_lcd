#include <QCoreApplication>
#include "universetolcd.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UniverseToLCD updater;
    updater.dmxuni = "2";
    updater.interval = 500;
    updater.init();

    return a.exec();
}
