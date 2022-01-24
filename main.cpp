#include "geopro.h"
#include "geometry_main.h"
#include "widget.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GeoPro w;
    w.show();

    return a.exec();
}
