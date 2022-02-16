#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"

#include "line.h"

#include "point.h"
#include "ray.h"
#include "segment.h"
#include "circle.h"

Line::Line(GeoBoard* board, Point* p1, Point* p2) :
    GOBJ(board, GObj_Type::LINE, true, true, p1->color()), mP1(p1), mP2(p2)
{
}

Line::Line(GeoBoard* board, Ray* ray) :
    GOBJ(board, GObj_Type::LINE, true, true, ray->getFirstPoint()->color()), mP1(ray->getFirstPoint()), mP2(ray->getSecondPoint())
{
}

Line::Line(GeoBoard* board, Segment* seg) :
    GOBJ(board, GObj_Type::LINE, true, true, seg->getFirstPoint()->color()), mP1(seg->getFirstPoint()), mP2(seg->getSecondPoint())
{
}

Line::Line(GeoBoard* board, qreal A, qreal B, qreal C) :
    GOBJ(board, GObj_Type::LINE, true, false, Qt::black)
{
    mP1 = new Point(board);
    mP2 = new Point(board);
    if (std::abs(B) < EPS){
        assert(std::abs(A) >= EPS);
        is_vertical = true;
        _k = 0;
        _y0 = 0;
        _x0 = -C/A;
    } else {
        is_vertical = false;
        _k = -A/B;
        _x0 = 0;
        _y0 = -C/B;
    }
    scr_x0 = mBoard->getScreenView(QPointF(_x0, _y0)).rx();
    scr_y0 = mBoard->getScreenView(QPointF(_x0, _y0)).ry();
}

void Line::recalculate(){
    checkExistance();
    switch (child_type){
    case Child_Type::Parallel:{
        Line* line = 0;
        if (baseline->type_is() != GObj_Type::LINE){ //making lines (if not a line)
            if (baseline->type_is() == GObj_Type::RAY) line = new Line(mBoard, static_cast<Ray*>(baseline));
            if (baseline->type_is() == GObj_Type::SEGMENT) line = new Line(mBoard, static_cast<Segment*>(baseline));
        } else line = static_cast<Line*>(baseline);
        line->recalculate();
        _k = line->k();
        is_vertical = line->isVertical();
        if (is_vertical){
            mP2->X = mP1->X;
            mP2->scr_x = mP1->scr_x;
            mP2->Y = mP1->Y + 100; //just to make vertical line
            mP2->scr_y = mP1->scr_y + 100; //just to make vertical line
        } else {
            mP2->X = mP1->X + 100;
            mP2->Y = mP1->Y + _k * (mP2->X - mP1->X);
            mP2->scr_x = mBoard->getScreenView(QPointF(mP2->X, mP2->Y)).x();
            mP2->scr_y = mBoard->getScreenView(QPointF(mP2->X, mP2->Y)).y();
        }
        if (baseline->type_is() != GObj_Type::LINE) delete line;
        break;
    }
    case Child_Type::Perpendicular:{
        Line* line = 0;
        if (baseline->type_is() != GObj_Type::LINE){ //making lines (if not a line)
            if (baseline->type_is() == GObj_Type::RAY) line = new Line(mBoard, static_cast<Ray*>(baseline));
            if (baseline->type_is() == GObj_Type::SEGMENT) line = new Line(mBoard, static_cast<Segment*>(baseline));
        } else line = static_cast<Line*>(baseline);
        line->recalculate();
        if (!line->isVertical() && std::abs(line->k()) >= EPS){
            _k = -1/line->k();
        } else _k = 0;
        is_vertical = (std::abs(line->k()) < EPS && !line->isVertical()) ? true : false;
        if (is_vertical){
            mP2->X = mP1->X;
            mP2->scr_x = mP1->scr_x;
            mP2->Y = mP1->Y + 100; //just to make vertical line
            mP2->scr_y = mP1->scr_y + 100; //just to make vertical line
        } else {
            mP2->X = mP1->X + 100;
            mP2->Y = mP1->Y + _k * (mP2->X - mP1->X);
            mP2->scr_x = mBoard->getScreenView(QPointF(mP2->X, mP2->Y)).x();
            mP2->scr_y = mBoard->getScreenView(QPointF(mP2->X, mP2->Y)).y();
        }
        if (baseline->type_is() != GObj_Type::LINE) delete line;
        break;
    }
    default:{
        if (std::abs(mP1->scr_x - mP2->scr_x) < EPS){
            is_vertical = true;
            _k = 0;
        } else {
            is_vertical = false;
            _k = (mP1->scr_y - mP2->scr_y)/(mP1->scr_x - mP2->scr_x);
        }
        break;
    }
    }
    scr_x0 = mP1->scr_x;
    scr_y0 = mP1->scr_y;
    _x0 = mP1->X;
    _y0 = mP1->Y;
}

std::pair<QPointF, QPointF> Line::get_draw_pair(){
    double A = mBoard->width() > mBoard->height() ? mBoard->width() : mBoard->height();
    double S_len = sqrt(mBoard->shift.x() * mBoard->shift.x() + mBoard->shift.y() * mBoard->shift.y());//shift length
    QPointF res1 = QPointF(mP1->scr_x, mP1->scr_y);
    QPointF res2 = QPointF(mP2->scr_x, mP2->scr_y);
    auto L = QLineF(res1, res2).length();
    if (L < EPS) L = EPS;
    if (L > A/2) L = A/2;
    QPointF dr = res1 - res2;
    QPointF p1 = res1 + (S_len + A) / L * dr;
    QPointF p2 = res1 + (-S_len - A) / L * dr;
    return std::make_pair(p1, p2);
}

void Line::draw(){
    if (!exists) return;
    recalculate();
    if (!is_visible()) return;
    QPointF p1 = get_draw_pair().first;
    QPointF p2 = get_draw_pair().second;

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
    recalculate();
    auto p1 = get_draw_pair().first;
    auto p2 = get_draw_pair().second;
    if (QLineF(p1, p).length() + QLineF(p2, p).length() < QLineF(p1, p2).length() + 0.05) return true;
    return false;
}


void Line::move(QPointF newPos){
    Q_UNUSED(newPos);
    return;
}
