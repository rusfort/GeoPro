#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"

#include "segment.h"

#include "point.h"
#include "line.h"
#include "ray.h"
#include "circle.h"
#include "angle.h"

Segment::Segment(GeoBoard* board, Point* p1, Point* p2) :
    GOBJ(board, GObj_Type::SEGMENT, true, true, p1->color()), mP1(p1), mP2(p2)
{
}

void Segment::recalculate(){
    checkExistance();
    scr_x1 = mP1->scr_x;
    scr_y1 = mP1->scr_y;
    scr_x2 = mP2->scr_x;
    scr_y2 = mP2->scr_y;
    if (std::abs(mP1->scr_x - mP2->scr_x) < EPS){
        is_vertical = true;
    }
    _x1 = mP1->X;
    _y1 = mP1->Y;
    _x2 = mP2->X;
    _y2 = mP2->Y;
    length = sqrt((_x1 - _x2)*(_x1 - _x2) + (_y1 - _y2)*(_y1 - _y2));
}

void Segment::draw(){
    if (!exists) return;
    recalculate();
    if (!is_visible()) return;
    QPainter p;
    auto p1 = QPointF(mP1->scr_x, mP1->scr_y);
    auto p2 = QPointF(mP2->scr_x, mP2->scr_y);

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

void Segment::changeView(){
}


bool Segment::isCaught(QPointF p){
    recalculate();
    auto p1 = QPointF(mP1->scr_x, mP1->scr_y);
    auto p2 = QPointF(mP2->scr_x, mP2->scr_y);
    if (QLineF(p1, p).length() + QLineF(p2, p).length() < QLineF(p1, p2).length() + 2) return true;
    return false;
}


void Segment::move(QPointF newPos){
    Q_UNUSED(newPos);
    return;
}
