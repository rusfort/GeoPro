#include "geopro.h"
#include "ui_geopro.h"
#include "painthelper.h"
#include "widget.h"
#include "grwidget.h"
#include "geometry_main.h"
#include "testgraph.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QtGui>


GeoPro::GeoPro(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GeoPro)
{
    ui->setupUi(this);

    auto central = new QWidget(this);
    setCentralWidget(central);
    //central->resize(size());
    GraphWidget *widget = new GraphWidget(central);
    widget->show();


    QTimer *timer = new QTimer(this);
    ///connect(timer, &QTimer::timeout, native, &Widget::animate);
    timer->start(50);
}

GeoPro::~GeoPro()
{
    delete ui;
}

void GeoPro::on_actionClose_triggered()
{
    QApplication::quit();
}

void GeoPro::resizeEvent(QResizeEvent* event)
{
    QList<GraphWidget *> widgets = findChildren<GraphWidget *>();
    widgets[0]->setGeometry(0, 0, event->size().width(), event->size().height());
}
