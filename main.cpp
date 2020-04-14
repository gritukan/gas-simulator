#include "gas_station.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QApplication a(argc, argv);
    TGasStation w;
    w.show();
    return a.exec();
}
