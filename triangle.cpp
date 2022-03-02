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
    bis_i = new Point(board);
    med_i = new Point(board);
    hgt_i = new Point(board);
    mdp_i = new Point(board);
    eul_c = new Point(board);
    in = new Circle(board, bis_i, 1);
    cir = new Circle(board, mdp_i, 1);
    Euler = new Circle(board, eul_c, 1);
    board->connect_objects(this, bis_i, Child_Type::InTriangle);
    board->connect_objects(this, med_i, Child_Type::InTriangle);
    board->connect_objects(this, hgt_i, Child_Type::InTriangle);
    board->connect_objects(this, mdp_i, Child_Type::InTriangle);
    board->connect_objects(this, eul_c, Child_Type::InTriangle);
    board->connect_objects(this, in, Child_Type::InTriangle);
    board->connect_objects(this, cir, Child_Type::InTriangle);
    board->connect_objects(this, Euler, Child_Type::InTriangle);
}


void Triangle::recalculate(){
    checkExistance();

    if (mBoard->onOneLine(mP1, mP2, mP3)) return;

    auto A = QPointF(mP1->X, mP1->Y);
    auto B = QPointF(mP2->X, mP2->Y);
    auto C = QPointF(mP3->X, mP3->Y);

    auto bis_i_point = A + (b() * (B - A) + c() * (C - A))/perimeter();
    bis_i->X = bis_i_point.x();
    bis_i->Y = bis_i_point.y();

    auto med_i_point = (A + B + C) / 3;
    med_i->X = med_i_point.x();
    med_i->Y = med_i_point.y();

    auto c1 = (C.x() - B.x()) * A.x() + (C.y() - B.y()) * A.y();
    auto c2 = (C.x() - A.x()) * B.x() + (C.y() - A.y()) * B.y();
    auto a1 = C.x() - B.x();
    auto b1 = C.y() - B.y();
    auto a2 = C.x() - A.x();
    auto b2 = C.y() - A.y();
    auto d = a1 * b2 - a2 * b1;
    auto hgt_i_point = QPointF((c1 * b2 - c2 * b1)/d, (a1 * c2 - a2 * c1)/d);
    hgt_i->X = hgt_i_point.x();
    hgt_i->Y = hgt_i_point.y();

    auto mdp_i_point = 3 * med_i_point / 2 - hgt_i_point / 2;
    mdp_i->X = mdp_i_point.x();
    mdp_i->Y = mdp_i_point.y();

    auto eul_c_point = hgt_i_point / 3 + 2 * med_i_point / 3;
    eul_c->X = eul_c_point.x();
    eul_c->Y = eul_c_point.y();

    changeView();
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

    QPolygonF pol;
    pol << QPointF(mP1->scr_x, mP1->scr_y) << QPointF(mP2->scr_x, mP2->scr_y) << QPointF(mP3->scr_x, mP3->scr_y);

    p.drawPolygon(pol);
}

void Triangle::changeView(){
    bis_i->changeView();
    med_i->changeView();
    hgt_i->changeView();
    mdp_i->changeView();
    eul_c->changeView();
    in->changeView();
    cir->changeView();
    Euler->changeView();
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

qreal Triangle::a() const{
    return QLineF(QPointF(mP2->X, mP2->Y), QPointF(mP3->X, mP3->Y)).length();
}

qreal Triangle::b() const{
    return QLineF(QPointF(mP1->X, mP1->Y), QPointF(mP3->X, mP3->Y)).length();

}
qreal Triangle::c() const{
    return QLineF(QPointF(mP1->X, mP1->Y), QPointF(mP2->X, mP2->Y)).length();
}
qreal Triangle::area() const{
    auto p = perimeter()/2;
    return sqrt(p * (p - a()) * (p - b()) * (p - c()));
}
