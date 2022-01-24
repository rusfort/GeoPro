#include "geopro.h"
#include "ui_geopro.h"
#include "widget.h"
#include "grwidget.h"
#include "geometry_main.h"

#include <iostream>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QtGui>
#include <QGraphicsView>


GeoPro::GeoPro(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GeoPro)
{
    ui->setupUi(this);
    setWindowTitle(tr("GeoPro 0.0.1 [alpha]"));

    //GraphWidget *widget = new GraphWidget(this);
    //setCentralWidget(widget);

    //GeoBoard *b = new GeoBoard(this);
    b = new GeoBoard(this);
    setCentralWidget(b);
    Point *p1 = new Point(b, b->width() / 2.0, b->height() / 2.0, 5);
    b->addObject(p1);

    /*GeoPoint *p1 = new GeoPoint(b, b->width() / 2.0, b->height() / 2.0, 5);
    GeoPoint *p2 = new GeoPoint(b, b->width() / 3.0, b->height() / 3.0, 5);
    GeoLine *l = new GeoLine(b, p1, p2);
    b->addObject(p1);
    b->addObject(p2);
    b->addObject(l);*/

    ///QTimer *timer = new QTimer(this);
    ///connect(timer, &QTimer::timeout, native, &Widget::animate);
    ///timer->start(50);
}

GeoPro::~GeoPro()
{
    delete ui;
}

void GeoPro::on_actionClose_triggered()
{
    QApplication::quit();
}

void GeoPro::on_actionPoint_triggered()
{
    //QList<GraphWidget *> widgets = findChildren<GraphWidget *>();
    //QList<GeoBoard *> widgets = findChildren<GeoBoard *>();
    if(b->numitemstoadd > 0) return;
    b->trytoadd = GObj_Type::POINT;
    b->numitemstoadd = 1;
    std::cout << "KEK\n" << std::endl;
    /*QList<GraphWidget *> widgets = findChildren<GraphWidget *>();
    QList<QGraphicsScene *> scenes = widgets[0]->findChildren<QGraphicsScene *>();

    Point *p = new Point(widgets[0], 0, 0, true);
    scenes[0]->addItem(p);
    p->setPos(p->x, p->y);*/
}

void GeoPro::on_actionLine_triggered()
{
    //QList<GraphWidget *> widgets = findChildren<GraphWidget *>();
    QList<GeoBoard *> widgets = findChildren<GeoBoard *>();
    if(widgets[0]->numitemstoadd > 0) return;
    widgets[0]->trytoadd = GObj_Type::LINE;
    widgets[0]->numitemstoadd = 2;
}

void GeoPro::on_actionSegment_triggered()
{
    //QList<GraphWidget *> widgets = findChildren<GraphWidget *>();
    QList<GeoBoard *> widgets = findChildren<GeoBoard *>();
    if(widgets[0]->numitemstoadd > 0) return;
    widgets[0]->trytoadd = GObj_Type::SEGMENT;
    widgets[0]->numitemstoadd = 2;
}
