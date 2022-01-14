#ifndef GRWIDGET_H
#define GRWIDGET_H

#include <QGraphicsView>
#include "geometry_main.h"

class Node;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);
    //GraphWidget(QWidget *parent, QGraphicsScene *scene);

    void itemMoved();

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    //void mouseMoveEvent(QMouseEvent *event);
    //void mousePressEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);

    void scaleView(qreal scaleFactor);

private:
    int timerId;
    Node *centerNode;
public:
    GObj_Type trytoadd = GObj_Type::NONE;
    int numitemstoadd = 0;
    Point *lastPoint = 0;
};

#endif // GRWIDGET_H
