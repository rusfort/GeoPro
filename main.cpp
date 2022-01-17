#include "geopro.h"
#include "geometry_main.h"
#include "grwidget.h"
#include "widget.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //GeoPro w;
    //w.show();
    GeoBoard b;
    GeoPoint p1(&b, b.width() / 2.0, b.height() / 2.0, 10);
    GeoPoint p2(&b, b.width() / 3.0, b.height() / 3.0, 10);
    GeoLine l(&b, &p1, &p2);
    b.addObject(&p1);
    b.addObject(&p2);
    b.addObject(&l);
    b.show();

    return a.exec();
}
