#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <iostream>

#include "geometry_main.h"
#include "grwidget.h"

STYLE::STYLE(DrStyle st0): style(st0){}
STYLE::~STYLE(){}

///POINT METHODS

Point::Point(GraphWidget *graphWidget, double x0, double y0, bool keptbymouse, DrStyle st0):
    STYLE(st0), graph(graphWidget), x(x0), y(y0), color(Qt::blue), kbm(keptbymouse) {
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
    return QRectF(-5 - adjust, -5 - adjust,
                  11 + adjust, 11 + adjust);
}

QPainterPath Point::shape() const
{
    QPainterPath path;
    path.addEllipse(-5, -5, 10, 10);
    return path;
}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QRadialGradient gradient(-1, -1, 5);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(1, 1);
        gradient.setFocalPoint(1, 1);
        gradient.setColorAt(1, QColor(Qt::black));
        gradient.setColorAt(0, QColor(Qt::white));
    } else {
        gradient.setColorAt(0, Qt::darkBlue);
        gradient.setColorAt(1, QColor(color));
    }
    painter->setBrush(gradient);
    auto pen = QPen(QBrush(color), 2);
    pen.setCosmetic(true); //! always one size in px
    painter->setPen(pen);
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

/*void Point::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!kbm) return;
    Q_UNUSED(event);
    //
    update();
    //scene()->mouseGrabberItem() == this
}*/

void Point::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Point::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (kbm) kbm = false;
    std::cout << pos().x() << " | " << pos().y() << std::endl;
    std::cout << event->buttonDownScreenPos(Qt::MouseButton::LeftButton).x() << " | " << event->buttonDownScreenPos(Qt::MouseButton::LeftButton).y() << std::endl;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}


///LINE METHODS

Line::Line(GraphWidget *graphWidget, double x01, double y01, double x02, double y02, DrStyle st0):
    STYLE(st0), graph(graphWidget), p1(graph, x01, y01), p2(graph, x02, y02){}

Line::~Line(){}
