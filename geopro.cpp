#include "geopro.h"
#include "ui_geopro.h"
#include "widget.h"
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

    b = new GeoBoard(this);
    setCentralWidget(b);

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
    if(b->numitemstoadd > 0) return;
    b->trytoadd = GObj_Type::POINT;
    b->numitemstoadd = 1;
}

void GeoPro::on_actionLine_triggered()
{
    if(b->numitemstoadd > 0) return;
    b->trytoadd = GObj_Type::LINE;
    b->numitemstoadd = 2;
}

void GeoPro::on_actionSegment_triggered()
{
    if(b->numitemstoadd > 0) return;
    b->trytoadd = GObj_Type::SEGMENT;
    b->numitemstoadd = 2;
}
