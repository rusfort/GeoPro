#include <iostream>
#include <limits>
#include <QPainter>
#include "widget.h"
#include "geometry_main.h"

STYLE::STYLE(DrStyle st0): style(st0){}
STYLE::~STYLE(){}

GOBJ::GOBJ(GeoBoard* board, GObj_Type t, QColor color) :
    type(t), mColor(color), mBoard(board), mIsSelected(false)
{
    connect(this, SIGNAL(selectionChanged()), mBoard, SLOT(update()));
}


///POINT METHODS==================================================================


Point::Point(GeoBoard* board, double x, double y, double radius,  QColor color) :
    GOBJ(board, GObj_Type::POINT, color), QPointF(x, y), mRadius(radius)
{
    connect(this, SIGNAL(posChanged()), mBoard, SLOT(update()));
}

Point::Point(const Point& copyFrom) :
    GOBJ(copyFrom.board(), GObj_Type::POINT, copyFrom.color()), QPointF(copyFrom.x(), copyFrom.y()), mRadius(copyFrom.rad())
{

}

void Point::draw()
{
    QPainter p;
    p.begin(this->board());
    p.setRenderHint(QPainter::Antialiasing);
    if(mIsSelected)
    {
        QPen pen(Qt::blue);
        pen.setWidth(2);
        p.setPen(pen);
    }
    p.setBrush(QBrush(mColor));
    p.drawEllipse(*this, mRadius, mRadius);
}

bool Point::isCatched(QPointF p)
{
    double dx = rx() - p.x();
    double dy = ry() - p.y();
    return dx * dx + dy * dy - mRadius * mRadius < 0;
}


///LINE METHODS==========================================================================================================


Line::Line(GeoBoard* board, Point* p1, Point* p2) :
    GOBJ(board, GObj_Type::LINE, p1->color()), mP1(p1), mP2(p2)
{

}

void Line::draw()
{
    double k = mBoard->width() > mBoard->height() ? mBoard->width() : mBoard->height();
    QPointF dr = *mP1 - *mP2;
    QPointF p1 = *mP1 + k * dr;
    QPointF p2 = *mP1 - k * dr;

    QPainter p;
    p.begin(mBoard);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(mColor);
    p.drawLine(p1, p2);
}


bool Line::isCatched(QPointF p)
{
    Q_UNUSED(p);
    return false;
}


void Line::move(QPointF newPos)
{
    Q_UNUSED(newPos);
    return;
}

///OLD VERSION:
/**
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
    //qreal extra = 500;

    //return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
    //                                  destPoint.y() - sourcePoint.y()))
    //    .normalized()
    //    .adjusted(-extra, -extra, extra, extra);
    return QRectF(std::min(DSP.x(), DDP.x()), std::min(DSP.y(), DDP.y()),
                  std::abs(DSP.x() - DDP.x()), std::abs(DSP.y() - DDP.y()));
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
    auto pen = QPen(QBrush(Qt::black), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen.setCosmetic(true); //! always one size in px
    painter->setPen(pen);
    painter->drawLine(DSP, DDP);
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
    //qreal lbound = -graph->sX()/2;
    //qreal rbound = graph->sX()/2;
    //qreal ubound = -graph->sY()/2;
    //qreal bbound = graph->sY()/2;

    auto BR = boundingRect();
    qreal lbound = BR.left();
    qreal rbound = BR.right();
    qreal ubound = BR.top();
    qreal bbound = BR.bottom();

    bool vertical = false;
    line_eq(0, vertical);
    if (vertical){
        DSP = QPointF (sourcePoint.x(), ubound);
        DDP = QPointF (sourcePoint.x(), bbound);
    } else {
        qreal Y1 = line_eq(lbound, vertical);
        qreal Y2 = line_eq(rbound, vertical);
        qreal X1 = lbound;
        qreal X2 = rbound;
        if (Y1 > bbound) {X1 = anti_line_eq(bbound); Y1 = bbound;}
        if (Y1 < ubound) {X1 = anti_line_eq(ubound); Y1 = ubound;}
        if (Y2 > bbound) {X2 = anti_line_eq(bbound); Y2 = bbound;}
        if (Y2 < ubound) {X2 = anti_line_eq(ubound); Y2 = ubound;}
        DSP = QPointF (X1, Y1);
        DDP = QPointF (X2, Y2);
    }

    prepareGeometryChange();
    sourcePoint = line.p1();
    destPoint = line.p2();
    update();
}

void Line::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    update();
    QGraphicsItem::dragMoveEvent(event);
}

void Line::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseMoveEvent(event);
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
    qreal extra = 10;
    return QRectF(sourcePoint.x(), sourcePoint.y(),
                  destPoint.x() - sourcePoint.x(), destPoint.y() - sourcePoint.y())
        .normalized().adjusted(-extra, -extra, extra, extra);
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

    if (length > qreal(10.)) {
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
}*/
