#include "geopro.h"
#include "geometry_main.h"
#include "grwidget.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat fmt;
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);

    GeoPro w;
    w.show();
    GraphWidget widget;
    widget.show();

    return a.exec();
}
