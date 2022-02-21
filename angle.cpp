#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"
#include "geometry_main.h"

#include "angle.h"

#include "line.h"
#include "point.h"
#include "ray.h"
#include "segment.h"
#include "circle.h"

Angle::Angle(GeoBoard* board, Point* vert, Point* p1, Point* p2) :
    GOBJ(board, GObj_Type::LINE, true, true, vert->color()), vertex(vert), mP1(p1), mP2(p2), fixed_degrees(false)
{
}
Angle::Angle(GeoBoard* board, Ray* ray, Point* p) :
    GOBJ(board, GObj_Type::LINE, true, true, p->color()), vertex(ray->getFirstPoint()), mP1(ray->getSecondPoint()), mP2(p), fixed_degrees(false)
{
}
Angle::Angle(GeoBoard* board, Ray* ray, qreal deg) :
    GOBJ(board, GObj_Type::LINE, true, true, ray->color()), vertex(ray->getFirstPoint()), mP1(ray->getSecondPoint()), fixed_degrees(true)
{
    auto d = QLineF(QPointF(vertex->X, vertex->Y), QPointF(mP1->X, mP1->Y)).length();
    auto dx = mP1->X - vertex->X;
    auto dy = mP1->Y - vertex->Y;
    qreal init_deg = acos(dx / d);
    if (dy < 0) init_deg = -init_deg;
    init_deg += deg;

    mP2 = new Point(board, vertex->X + 10 * cos(init_deg), vertex->Y + 10 * sin(init_deg));
    _degrees = deg;
}

void Angle::recalculate(){
    checkExistance();
    scr_x0 = vertex->scr_x;
    scr_y0 = vertex->scr_y;
    _x0 = vertex->X;
    _y0 = vertex->Y;
    auto d = QLineF(QPointF(vertex->X, vertex->Y), QPointF(mP1->X, mP1->Y)).length();
    auto dx = mP1->X - vertex->X;
    auto dy = mP1->Y - vertex->Y;
    qreal first_deg = acos(dx / d);
    if (dy < 0) first_deg = -first_deg;
    if (fixed_degrees){
        first_deg += _degrees;
        mP2->X = vertex->X + 10 * cos(first_deg);
        mP2->Y = vertex->Y + 10 * sin(first_deg);
    } else {
        auto d2 = QLineF(QPointF(vertex->X, vertex->Y), QPointF(mP2->X, mP2->Y)).length();
        auto dx2 = mP2->X - vertex->X;
        auto dy2 = mP2->Y - vertex->Y;
        qreal second_deg = acos(dx2 / d2);
        if (dy2 < 0) second_deg = -second_deg;
        _degrees = second_deg - first_deg;
    }
}

void Angle::draw(){
    if (!exists) return;
    recalculate();
    if (!is_visible()) return;

    QRectF rectangle(100.0, 200.0, 500.0, 600.0); //test!
    int startAngle = 30 * 16; //test!
    int spanAngle = 120 * 16; //test!

    QPainter p;
    p.setBrush(QBrush(Qt::blue));
    p.drawPie(rectangle, startAngle, spanAngle);
}

void Angle::changeView(){
}


bool Angle::isCaught(QPointF p){
    recalculate();
    //
    return false;
}


void Angle::move(QPointF newPos){
    Q_UNUSED(newPos);
    return;
}
