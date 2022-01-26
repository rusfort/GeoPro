#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"
#include "geometry_main.h"

STYLE::STYLE(DrStyle st0): style(st0){}
STYLE::~STYLE(){}

GOBJ::GOBJ(GeoBoard* board, GObj_Type t, bool is_depending, QColor color) :
    type(t), depending(is_depending), mColor(color), mBoard(board), mIsSelected(false)
{
    connect(this, SIGNAL(selectionChanged()), mBoard, SLOT(update()));
}


///POINT METHODS==================================================================


Point::Point(GeoBoard* board, double x, double y, double radius,  QColor color) :
    GOBJ(board, GObj_Type::POINT, false, color), QPointF(x, y), mRadius(radius)
{
    connect(this, SIGNAL(posChanged()), mBoard, SLOT(update()));
    scr_x = x;
    scr_y = y;
    X = board->getMathPoint(QPointF(x, y)).x();
    Y = board->getMathPoint(QPointF(x, y)).y();
}

void Point::draw(){
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

void Point::move(QPointF newPos){
    rx() = newPos.x();
    ry() = newPos.y();
    scr_x = newPos.x();
    scr_y = newPos.y();
    X = board()->getMathPoint(newPos).x();
    Y = board()->getMathPoint(newPos).y();
    emit posChanged();
}

void Point::changeView(){
    scr_x = board()->getScreenView(QPointF(X, Y)).rx();
    scr_y = board()->getScreenView(QPointF(X, Y)).ry();
    rx() = scr_x;
    ry() = scr_y;
}

bool Point::isCaught(QPointF p){
    double dx = rx() - p.x();
    double dy = ry() - p.y();
    return dx * dx + dy * dy - mRadius * mRadius < 0;
}


///LINE METHODS==========================================================================================================


Line::Line(GeoBoard* board, Point* p1, Point* p2) :
    GOBJ(board, GObj_Type::LINE, true, p1->color()), mP1(p1), mP2(p2)
{
    recalculate();
}

void Line::recalculate(){
    scr_x0 = mP1->scr_x;
    scr_y0 = mP1->scr_y;
    if (std::abs(mP1->scr_x - mP2->scr_x) < EPS){
        is_vertical = true;
        k = 0;
    } else {
        is_vertical = false;
        k = (mP1->scr_y - mP2->scr_y)/(mP1->scr_x - mP2->scr_x);
    }
    x0 = mP1->X;
    y0 = mP1->Y;
}

void Line::draw(){
    double A = mBoard->width() > mBoard->height() ? mBoard->width() : mBoard->height();
    QPointF dr = *mP1 - *mP2;
    QPointF p1 = *mP1 + A * dr;
    QPointF p2 = *mP1 - A * dr;

    QPainter p;
    p.begin(mBoard);
    p.setRenderHint(QPainter::Antialiasing);
    if (mIsSelected){
        QPen pen(Qt::blue);
        pen.setWidth(3);
        p.setPen(pen);
        p.drawLine(p1, p2);
    }
    p.setPen(mColor);
    p.drawLine(p1, p2);
}

void Line::changeView(){
}


bool Line::isCaught(QPointF p){
    //TODO: BUGS!!!
    recalculate();
    double d = 0;
    if (!is_vertical && std::abs(k) < sqrt(std::numeric_limits<qreal>::max())) d = std::abs(-k * p.rx() + p.ry() + k * scr_x0 - scr_y0)/(1 + k * k);
    else d = std::abs(scr_x0 - p.rx());
    if (d < 3) return true;
    return false;
}


void Line::move(QPointF newPos){
    Q_UNUSED(newPos);
    return;
}

