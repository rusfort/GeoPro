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
    GOBJ(board, GObj_Type::ANGLE, true, true, vert->color()), vertex(vert), mP1(p1), mP2(p2), fixed_degrees(false)
{
}
Angle::Angle(GeoBoard* board, Ray* ray, Point* p) :
    GOBJ(board, GObj_Type::ANGLE, true, true, p->color()), vertex(ray->getFirstPoint()), mP1(ray->getSecondPoint()), mP2(p), fixed_degrees(false)
{
}
Angle::Angle(GeoBoard* board, Ray* ray, qreal deg) :
    GOBJ(board, GObj_Type::ANGLE, true, true, ray->color()), vertex(ray->getFirstPoint()), mP1(ray->getSecondPoint()), fixed_degrees(true)
{
    auto d = QLineF(QPointF(vertex->X, vertex->Y), QPointF(mP1->X, mP1->Y)).length();
    auto dx = mP1->X - vertex->X;
    auto dy = mP1->Y - vertex->Y;
    qreal init_deg = acos(dx / d) * 180 / PI;
    if (dy > 0) init_deg = -init_deg;
    init_deg += deg;

    mP2 = new Point(board, vertex->X + 10 * cos(init_deg / 180 * PI), vertex->Y + 10 * sin(init_deg / 180 * PI));
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
    _startdeg = acos(dx / d) * 180 / PI;
    if (dy > 0) _startdeg = -_startdeg;
    if (fixed_degrees){
        _startdeg += _degrees;
        mP2->X = vertex->X + 10 * cos(_startdeg / 180 * PI);
        mP2->Y = vertex->Y + 10 * sin(_startdeg / 180 * PI);
    } else {
        auto d2 = QLineF(QPointF(vertex->X, vertex->Y), QPointF(mP2->X, mP2->Y)).length();
        auto dx2 = mP2->X - vertex->X;
        auto dy2 = mP2->Y - vertex->Y;
        qreal second_deg = acos(dx2 / d2) * 180 / PI;
        if (dy2 > 0) second_deg = -second_deg;
        _degrees = second_deg - _startdeg;
        if (_degrees > 180) _degrees = -360 + _degrees;
        else if (_degrees < -180) _degrees = 360 + _degrees;
    }
}

void Angle::draw(){
    if (!exists) return;
    recalculate();
    if (!is_visible()) return;

    QRectF rectangle(vertex->scr_x - _radius, vertex->scr_y - _radius, 2*_radius, 2*_radius);
    int startAngle = _startdeg * 16;
    int spanAngle = _degrees * 16;

    if (180 - std::abs(_degrees) < EPS) return;

    QPainter p;
    p.begin(mBoard);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(mColor);
    if (num_arcs == 0) p.setBrush(QBrush(mColor));
    if(mIsSelected){
        p.setPen(Qt::blue);
        if (num_arcs == 0) p.setBrush(QBrush(Qt::blue));
    }
    p.drawPie(rectangle, startAngle, spanAngle);
    if (num_arcs >= 2){
        QRectF rectangle2(vertex->scr_x - _radius + 3, vertex->scr_y - _radius + 3, 2*_radius - 6, 2*_radius - 6);
        p.drawArc(rectangle2, startAngle, spanAngle);
    }
    if (num_arcs == 3){
        QRectF rectangle3(vertex->scr_x - _radius + 6, vertex->scr_y - _radius + 6, 2*_radius - 12, 2*_radius - 12);
        p.drawArc(rectangle3, startAngle, spanAngle);
    }
}

void Angle::changeView(){
}


bool Angle::isCaught(QPointF p){
    recalculate();
    auto d = QLineF(QPointF(vertex->scr_x, vertex->scr_y), p).length();
    if (d > _radius) return false;
    auto dx = p.x() - vertex->scr_x;
    auto dy = p.y() - vertex->scr_y;
    auto deg = acos(dx / d) * 180 / PI;
    if (dy > 0) deg = -deg;
    auto enddeg = _startdeg + _degrees;

    if (_startdeg >= 0){
        if (_degrees < 0 && enddeg < deg && deg < _startdeg) return true;
        if (_degrees > 0){
            if (_startdeg < deg && deg < enddeg) return true;
            if (_startdeg < deg + 360 && deg + 360 < enddeg) return true;
        }
    } else {
        if (_degrees < 0){
            if (deg > 0) deg -= 360;
            if (enddeg < deg && deg < _startdeg) return true;
        } else {
            if (deg < 0 && _startdeg < deg) return true;
            if (deg > 0 && deg < enddeg) return true;
        }
    }

    return false;
}


void Angle::move(QPointF newPos){
    Q_UNUSED(newPos);
    return;
}

QString Angle::dumpData(){
    QString data = "1 " + QString::number(num_arcs) + " "
            + QString::number(fixed_degrees) + " "
            + QString::number(vertex->id()) + " "
            + QString::number(mP1->id()) + " "
            + QString::number(mP2->id()) + " ";
    return data;
}

bool Angle::dumpParse(QTextStream& stream){
    if (!generalDumpParse(stream)) return false;
    int check_num;
    stream >> check_num;
    if (check_num != 1){
        stream.readLine();
        return false;
    }
    //for tests!
    stream.readLine(); //remove later!!!
    return true;
}
