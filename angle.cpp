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
    GOBJ(board, GObj_Type::LINE, true, true, vert->color()), vertex(vert), mP1(p1), mP2(p2)
{
}
Angle::Angle(GeoBoard* board, Ray* ray, Point* p) :
    GOBJ(board, GObj_Type::LINE, true, true, p->color()), vertex(ray->getFirstPoint()), mP1(ray->getSecondPoint()), mP2(p)
{
}
Angle::Angle(GeoBoard* board, Ray* ray, qreal deg) :
    GOBJ(board, GObj_Type::LINE, true, true, ray->color()), vertex(ray->getFirstPoint()), mP1(ray->getSecondPoint())
{
    //use deg
}

void Angle::recalculate(){
    checkExistance();
    //
    scr_x0 = mP1->scr_x;
    scr_y0 = mP1->scr_y;
    _x0 = mP1->X;
    _y0 = mP1->Y;
}

void Angle::draw(){
    if (!exists) return;
    recalculate();
    if (!is_visible()) return;
    //
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
