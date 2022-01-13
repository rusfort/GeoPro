#include "geopro.h"
#include "ui_geopro.h"
#include "painthelper.h"
#include "widget.h"
#include "grwidget.h"
#include "geometry_main.h"
#include "testgraph.h"

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

    GraphWidget *widget = new GraphWidget(this);
    setCentralWidget(widget);

    std::cout << pos().x() << " - " << pos().y() << std::endl;

    //widget->show();


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

/*void GeoPro::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QList<GraphWidget *> widgets = findChildren<GraphWidget *>();
    QList<QGraphicsScene *> scenes = widgets[0]->findChildren<QGraphicsScene *>();
    foreach (QGraphicsItem *item, scenes[0]->items()) {
        if (qgraphicsitem_cast<Point *>(item)){
            auto p = qgraphicsitem_cast<Point *>(item);
            if (p->kbm){
                p->setPos(event->windowPos().x(), event->windowPos().y());
                break;
            }
        }
    }
}*/
void GeoPro::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QList<GraphWidget *> widgets = findChildren<GraphWidget *>();
    if(widgets[0]->trytoadd == GObj_Type::NONE) return;

    QList<QGraphicsScene *> scenes = widgets[0]->findChildren<QGraphicsScene *>();
    if(widgets[0]->trytoadd == GObj_Type::POINT){
        widgets[0]->trytoadd = GObj_Type::NONE;
        Point *p = new Point(widgets[0], 0, 0, true);
        scenes[0]->addItem(p);
        auto Pos1 = event->windowPos().toPoint();
        qreal factor = widgets[0]->transform().mapRect(QRectF(0, 0, 1, 1)).width();
        QPointF WPOS = QPointF(widgets[0]->pos().x(), widgets[0]->pos().y());
        QPoint Pos3 = widgets[0]->mapFromScene(QPointF(0, 0));
        QPointF POS = (Pos1 - Pos3- WPOS)/factor ;
        p->setPos(POS);
    }
}

void GeoPro::on_actionPoint_triggered()
{
    QList<GraphWidget *> widgets = findChildren<GraphWidget *>();
    widgets[0]->trytoadd = GObj_Type::POINT;
    /*QList<GraphWidget *> widgets = findChildren<GraphWidget *>();
    QList<QGraphicsScene *> scenes = widgets[0]->findChildren<QGraphicsScene *>();

    Point *p = new Point(widgets[0], 0, 0, true);
    scenes[0]->addItem(p);
    p->setPos(p->x, p->y);*/
}
