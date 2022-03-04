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
    GOBJ(board, GObj_Type::TRIANGLE, true, true, Qt::lightGray), mP1(p1), mP2(p2), mP3(p3)
{
    bis_i = new Point(board);
    bis_i->changeLabel("I");
    bis_i->hide();
    med_i = new Point(board);
    med_i->changeLabel("M");
    med_i->hide();
    hgt_i = new Point(board);
    hgt_i->changeLabel("H");
    hgt_i->hide();
    mdp_i = new Point(board);
    mdp_i->changeLabel("O");
    mdp_i->hide();
    eul_c = new Point(board);
    eul_c->changeLabel("E");
    eul_c->hide();
    edge_a = new Segment(board, p2, p3);
    edge_b = new Segment(board, p3, p1);
    edge_c = new Segment(board, p1, p2);
    in = new Circle(board, bis_i, 100);
    in->hide();
    cir = new Circle(board, mdp_i, 100);
    cir->hide();
    Euler = new Circle(board, eul_c, 100);
    Euler->hide();
    _alpha = new Angle(board, mP3, mP1, mP2);
    _alpha->setNumArcs(1);
    _alpha->hide();
    _beta  = new Angle(board, p1, p2, p3);
    _beta->setNumArcs(2);
    _beta->hide();
    _gamma = new Angle(board, p2, p3, p1);
    _gamma->setNumArcs(3);
    _gamma->hide();
    board->connect_objects(this, bis_i,  Child_Type::InTriangle);
    board->connect_objects(this, med_i,  Child_Type::InTriangle);
    board->connect_objects(this, hgt_i,  Child_Type::InTriangle);
    board->connect_objects(this, mdp_i,  Child_Type::InTriangle);
    board->connect_objects(this, eul_c,  Child_Type::InTriangle);
    board->connect_objects(this, in,     Child_Type::InTriangle);
    board->connect_objects(this, cir,    Child_Type::InTriangle);
    board->connect_objects(this, Euler,  Child_Type::InTriangle);
    board->connect_objects(this, edge_a, Child_Type::InTriangle);
    board->connect_objects(this, edge_b, Child_Type::InTriangle);
    board->connect_objects(this, edge_c, Child_Type::InTriangle);
    board->connect_objects(this, _alpha, Child_Type::InTriangle);
    board->connect_objects(this, _beta,  Child_Type::InTriangle);
    board->connect_objects(this, _gamma, Child_Type::InTriangle);
}

Triangle::~Triangle(){
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

    auto eul_c_point = (mdp_i_point + hgt_i_point) / 2;
    eul_c->X = eul_c_point.x();
    eul_c->Y = eul_c_point.y();

    auto l = new Line(mBoard, mP1, mP2);
    l->recalculate();
    auto r = distance(bis_i, l);
    delete l;
    in->set_r(r);

    r = distance(mdp_i, mP1);
    cir->set_r(r);

    r /= 2;
    Euler->set_r(r);

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
        p.setPen(Qt::cyan);
        p.setBrush(QBrush(Qt::cyan));
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
    auto m = (mP1->scr_x - p.x()) * (mP2->scr_y - mP1->scr_y) - (mP2->scr_x - mP1->scr_x) * (mP1->scr_y - p.y());
    auto n = (mP2->scr_x - p.x()) * (mP3->scr_y - mP2->scr_y) - (mP3->scr_x - mP2->scr_x) * (mP2->scr_y - p.y());
    auto k = (mP3->scr_x - p.x()) * (mP1->scr_y - mP3->scr_y) - (mP1->scr_x - mP3->scr_x) * (mP3->scr_y - p.y());
    auto pr1 = (n*m < 0);
    auto pr2 = (n*k < 0);
    auto pr3 = (k*m < 0);
    if (pr1 || pr2 || pr3) return false;
    return true;
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

qreal Triangle::alpha() const{
    return _alpha->degrees();
}
qreal Triangle::beta() const{
    return _beta->degrees();
}
qreal Triangle::gamma() const{
    return _gamma->degrees();
}

QString Triangle::dumpData(){
    return "1 0 ";
}
