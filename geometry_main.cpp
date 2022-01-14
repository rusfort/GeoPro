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
    STYLE(st0), graph(graphWidget), x(x0), y(y0), color(Qt::blue), kbm(keptbymouse), size(5) {
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

Point::Point(Point *p0):
STYLE(p0->style), graph(p0->getgw()), x(p0->x), y(p0->y), color(p0->color), kbm(p0->kbm), size(p0->size) {
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

Point::~Point(){}

QRectF Point::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-size - adjust, -size - adjust,
                  2*size + adjust, 2*size + adjust);
}

QPainterPath Point::shape() const
{
    QPainterPath path;
    path.addEllipse(-size, -size, 2*size, 2*size);
    return path;
}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QRadialGradient gradient(-1, -1, size);
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
    painter->drawEllipse(-size, -size, 2*size, 2*size);
}

QVariant Point::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Line *line, lineList)
            line->adjust();
        foreach (Segment *seg, segList)
            seg->adjust();
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
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Point::addline (Line *l)
{
    lineList << l;
    l->adjust();
}

void Point::addseg (Segment *s)
{
    segList << s;
    s->adjust();
}


///LINE METHODS==========================================================================================================

Line::Line(GraphWidget *graphWidget,  Point* p1, Point* p2, DrStyle st0):
    STYLE(st0), graph(graphWidget), p1_(p1), p2_(p2), color(Qt::darkGreen){
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

Line::~Line(){}

QRectF Line::boundingRect() const
{
    //qreal adjust = 2;
    //QPointF sourcePoint = QPointF(p1_->x, p1_->y);
    //QPointF destPoint = QPointF(p2_->x, p2_->y);
    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized();
    //return QRectF(p1_->x, p1_->y, p2_->x, p2_->y); //MUST NOT BE CORRECT!
}

QPainterPath Line::shape() const
{
    QPainterPath path;
    path.addRect(p1_->x, p1_->y, p2_->x, p2_->y); //MUST NOT BE CORRECT!
    return path;
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(option);
    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;
    auto pen = QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen.setCosmetic(true); //! always one size in px

    painter->setPen(pen);
    painter->drawLine(line);
    /*!!!*/ //painter->drawLine(p1_->x, p1_->y, p2_->x, p2_->y); //THIS IS NOT A LINE! SEGMENT!
}

/*QVariant Line::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}*/

void Line::adjust()
{
    QLineF line(mapFromItem(p1_, 0, 0), mapFromItem(p2_, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        sourcePoint = line.p1();
        destPoint = line.p2();
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

void Line::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //if (kbm) kbm = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

///SEGMENT METHODS==========================================================================================================

Segment::Segment(GraphWidget *graphWidget,  Point* p1, Point* p2, DrStyle st0):
    STYLE(st0), graph(graphWidget), p1_(p1), p2_(p2), color(Qt::darkGreen){
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

Segment::~Segment(){}

QRectF Segment::boundingRect() const
{
    //qreal adjust = 2;
    //QPointF sourcePoint = QPointF(p1_->x, p1_->y);
    //QPointF destPoint = QPointF(p2_->x, p2_->y);
    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized();
    //return QRectF(p1_->x, p1_->y, p2_->x, p2_->y);
}

QPainterPath Segment::shape() const
{
    QPainterPath path;
    path.addRect(p1_->x, p1_->y, p2_->x, p2_->y);
    return path;
}

void Segment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(option);
    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;
    auto pen = QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen.setCosmetic(true); //! always one size in px

    painter->setPen(pen);
    painter->drawLine(line);
}

void Segment::adjust()
{
    QLineF line(mapFromItem(p1_, 0, 0), mapFromItem(p2_, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        sourcePoint = line.p1();
        destPoint = line.p2();
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

void Segment::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Segment::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //if (kbm) kbm = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
