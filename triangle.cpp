#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"
#include "geometry_main.h"

#include "triangle.h"

#include "angle.h"
#include "line.h"
#include "point.h"
#include "ray.h"
#include "segment.h"
#include "circle.h"

Triangle::Triangle(GeoBoard* board, Point* p1, Point* p2, Point* p3) :
    GOBJ(board, GObj_Type::ANGLE, true, true, Qt::lightGray), mP1(p1), mP2(p2), mP3(p3)
{
}


void Triangle::recalculate(){
    checkExistance();
    //TODO
}

void Triangle::draw(){
    if (!exists) return;
    recalculate();
    if (!is_visible()) return;

    QPainter p;
    p.begin(mBoard);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(mColor);
    p.setBrush(QBrush(mColor));
    if(mIsSelected){
        p.setPen(Qt::blue);
        p.setBrush(QBrush(Qt::blue));
    }
    //TODO
}

void Triangle::changeView(){
}


bool Triangle::isCaught(QPointF p){
    recalculate();
    //auto d = QLineF(QPointF(vertex->scr_x, vertex->scr_y), p).length();


    return false;
}


void Triangle::move(QPointF newPos){
    Q_UNUSED(newPos);
    return;
}
