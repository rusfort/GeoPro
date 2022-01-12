#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "geometry_main.h"
#include "grwidget.h"

STYLE::STYLE(DrStyle st0): style(st0){}
STYLE::~STYLE(){}

///POINT METHODS

Point::Point(GraphWidget *graphWidget, double x0, double y0, DrStyle st0):
    STYLE(st0), graph(graphWidget), x(x0), y(y0), color(Qt::blue) {
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

Point::~Point(){}

bool Point::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF Point::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust,
                  23 + adjust, 23 + adjust);
}

QPainterPath Point::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    //painter->setBrush(Qt::darkGray);
    //painter->drawEllipse(-3, -3, 10, 10);

    QRadialGradient gradient(-1, -1, 5);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(1, 1);
        gradient.setFocalPoint(1, 1);
        gradient.setColorAt(1, QColor(Qt::black));
        gradient.setColorAt(0, QColor(Qt::white));
    } else {
        gradient.setColorAt(0, Qt::darkBlue);
        gradient.setColorAt(1, Qt::blue);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::blue, 0));
    painter->drawEllipse(-5, -5, 10, 10);
}

QVariant Point::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Point::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Point::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}


///LINE METHODS

Line::Line(GraphWidget *graphWidget, double x01, double y01, double x02, double y02, DrStyle st0):
    STYLE(st0), graph(graphWidget), p1(graph, x01, y01), p2(graph, x02, y02){}

Line::~Line(){}
