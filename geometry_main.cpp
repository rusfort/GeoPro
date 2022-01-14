#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <iostream>
#include <limits>

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
    setZValue(p1->zValue() - 1);
    memPos = QPointF(0, 0);
}

Line::~Line(){}

QRectF Line::boundingRect() const
{
    return QRectF(DSP.x(), DSP.y(), DDP.x() - DSP.x(), DDP.y() - DSP.y())
        .normalized();
}

QPainterPath Line::shape() const
{
    QPainterPath path;
    QPointF p1, p2, p3, p4;
    qreal adj = 10;
    if (DSP.x() < DDP.x()){
        if (DSP.y() < DSP.y()){
            p1 = QPointF(DSP.x(), DSP.y() - adj);
            p2 = QPointF(DSP.x() - adj, DSP.y());
            p3 = QPointF(DDP.x(), DDP.y() + adj);
            p4 = QPointF(DDP.x() + adj, DDP.y());
        } else {
            p1 = QPointF(DSP.x() - adj, DSP.y());
            p2 = QPointF(DSP.x(), DSP.y() + adj);
            p3 = QPointF(DDP.x() + adj, DDP.y());
            p4 = QPointF(DDP.x(), DDP.y() - adj);
        }
    } else {
        if (DSP.y() < DSP.y()){
            p1 = QPointF(DSP.x(), DSP.y() + adj);
            p2 = QPointF(DSP.x() + adj, DSP.y());
            p3 = QPointF(DDP.x(), DDP.y() - adj);
            p4 = QPointF(DDP.x() - adj, DDP.y());
        } else {
            p1 = QPointF(DSP.x() + adj, DSP.y());
            p2 = QPointF(DSP.x(), DSP.y() - adj);
            p3 = QPointF(DDP.x() - adj, DDP.y());
            p4 = QPointF(DDP.x(), DDP.y() + adj);
        }
    }
    QPolygonF polygon;
    polygon << p1 << p2 << p3 << p4;
    path.addPolygon(polygon);
    return path;
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(option);
    QLineF testline(sourcePoint, destPoint);
    if (qFuzzyCompare(testline.length(), qreal(0.)))
        return;
    auto pen = QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen.setCosmetic(true); //! always one size in px
    painter->setPen(pen);
    painter->drawLine(QLineF(DSP, DDP));
}

QVariant Line::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        p1_->setX(p1_->pos().x() - memPos.x() + value.toPointF().x());
        p2_->setX(p2_->pos().x() - memPos.x() + value.toPointF().x());
        p1_->setY(p1_->pos().y() - memPos.y() + value.toPointF().y());
        p2_->setY(p2_->pos().y() - memPos.y() + value.toPointF().y());
        memPos = value.toPointF();
        graph->itemMoved();
        break;
    default:
        break;
    };


    return QGraphicsItem::itemChange(change, value);
}

void Line::adjust()
{
    QLineF line(mapFromItem(p1_, 0, 0), mapFromItem(p2_, 0, 0));
    qreal length = line.length();

    bool vertical = false;
    line_eq(0, vertical);
    if (vertical){
        DSP = QPointF (sourcePoint.x(), -graph->sY()/2);
        DDP = QPointF (sourcePoint.x(), graph->sY()/2);
    } else {
        qreal Y1 = line_eq(-graph->sX()/2, vertical);
        qreal Y2 = line_eq(graph->sX()/2, vertical);
        qreal X1 = -graph->sX()/2;
        qreal X2 = graph->sX()/2;
        if (Y1 > graph->sY()/2) {X1 = anti_line_eq(graph->sY()/2); Y1 = graph->sY()/2;}
        if (Y1 < -graph->sY()/2) {X1 = anti_line_eq(-graph->sY()/2); Y1 = -graph->sY()/2;}
        if (Y2 > graph->sY()/2) {X2 = anti_line_eq(graph->sY()/2); Y2 = graph->sY()/2;}
        if (Y2 < -graph->sY()/2) {X2 = anti_line_eq(-graph->sY()/2); Y2 = -graph->sY()/2;}
        DSP = QPointF (X1, Y1);
        DDP = QPointF (X2, Y2);
    }

    prepareGeometryChange();

    if (length > qreal(20.)) {
        sourcePoint = line.p1();
        destPoint = line.p2();
    } else {
        sourcePoint = destPoint = line.p1();
    }
    update();
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

qreal Line::line_eq(qreal x, bool& is_vertical){
    if (std::numeric_limits<qreal>::max() / 2 / graph->height() * std::abs(sourcePoint.x() - destPoint.x()) <= 1){
        is_vertical = true;
        return 0;
    }
    is_vertical = false;
    qreal k = (sourcePoint.y() - destPoint.y()) / (sourcePoint.x() - destPoint.x());
    return k * (x - destPoint.x()) + destPoint.y();
}

qreal Line::anti_line_eq(qreal y){
    //MUST NOT BE HORISONTAL!!!
    qreal k = (sourcePoint.x() - destPoint.x()) / (sourcePoint.y() - destPoint.y());
    return k * (y - destPoint.y()) + destPoint.x();
}

///SEGMENT METHODS==========================================================================================================

Segment::Segment(GraphWidget *graphWidget,  Point* p1, Point* p2, DrStyle st0):
    STYLE(st0), graph(graphWidget), p1_(p1), p2_(p2), color(Qt::darkGreen){
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(p1->zValue() - 1);
    memPos = QPointF(0, 0);
}

Segment::~Segment(){}

QRectF Segment::boundingRect() const
{
    return QRectF(sourcePoint.x(), sourcePoint.y(),
                  destPoint.x() - sourcePoint.x(), destPoint.y() - sourcePoint.y())
        .normalized();
}

QPainterPath Segment::shape() const
{
    QPainterPath path;
    QPointF p1, p2, p3, p4;
    qreal adj = 10;
    if (sourcePoint.x() < destPoint.x()){
        if (sourcePoint.y() < sourcePoint.y()){
            p1 = QPointF(sourcePoint.x(), sourcePoint.y() - adj);
            p2 = QPointF(sourcePoint.x() - adj, sourcePoint.y());
            p3 = QPointF(destPoint.x(), destPoint.y() + adj);
            p4 = QPointF(destPoint.x() + adj, destPoint.y());
        } else {
            p1 = QPointF(sourcePoint.x() - adj, sourcePoint.y());
            p2 = QPointF(sourcePoint.x(), sourcePoint.y() + adj);
            p3 = QPointF(destPoint.x() + adj, destPoint.y());
            p4 = QPointF(destPoint.x(), destPoint.y() - adj);
        }
    } else {
        if (sourcePoint.y() < sourcePoint.y()){
            p1 = QPointF(sourcePoint.x(), sourcePoint.y() + adj);
            p2 = QPointF(sourcePoint.x() + adj, sourcePoint.y());
            p3 = QPointF(destPoint.x(), destPoint.y() - adj);
            p4 = QPointF(destPoint.x() - adj, destPoint.y());
        } else {
            p1 = QPointF(sourcePoint.x() + adj, sourcePoint.y());
            p2 = QPointF(sourcePoint.x(), sourcePoint.y() - adj);
            p3 = QPointF(destPoint.x() - adj, destPoint.y());
            p4 = QPointF(destPoint.x(), destPoint.y() + adj);
        }
    }
    QPolygonF polygon;
    polygon << p1 << p2 << p3 << p4;
    path.addPolygon(polygon);
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

QVariant Segment::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        p1_->setX(p1_->pos().x() - memPos.x() + value.toPointF().x());
        p2_->setX(p2_->pos().x() - memPos.x() + value.toPointF().x());
        p1_->setY(p1_->pos().y() - memPos.y() + value.toPointF().y());
        p2_->setY(p2_->pos().y() - memPos.y() + value.toPointF().y());
        memPos = value.toPointF();
        graph->itemMoved();
        break;
    default:
        break;
    };


    return QGraphicsItem::itemChange(change, value);
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
