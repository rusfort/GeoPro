#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"
#include "geometry_main.h"

STYLE::STYLE(DrStyle st0): style(st0){}
STYLE::~STYLE(){}

GOBJ::GOBJ(GeoBoard* board, GObj_Type t, bool is_depending, bool do_exist, QColor color) :
    type(t), mColor(color), mBoard(board), mIsSelected(false), depending(is_depending), exists(do_exist)
{
    connect(this, SIGNAL(selectionChanged()), mBoard, SLOT(update()));
}

void GOBJ::delObj(){
    while(childObjects.size() > 0){
        for (auto& p : childObjects){ //deleting all children
            p.first->delObj();
            break;
        }
    }
    for (auto& obj : parentObjects){ //deleting all mentions about this object
        obj->eraseInfoAboutChild(this);
    }

    if (mIsSelected) mBoard->num_obj_selected--;
    mBoard->delObject(this);
}

void GOBJ::checkExistance(){
    for (auto& obj : parentObjects){
        if(obj->exists != true){
            exists = false;
            return;
        }
    }
    exists = true; //try to resurrect the object
}

///POINT METHODS==================================================================


Point::Point(GeoBoard* board, double x, double y, double radius, QColor color) :
    GOBJ(board, GObj_Type::POINT, false, true, color), mRadius(radius)
{
    connect(this, SIGNAL(posChanged()), mBoard, SLOT(update()));
    scr_x = x;
    scr_y = y;
    X = board->getMathPoint(QPointF(x, y)).x();
    Y = board->getMathPoint(QPointF(x, y)).y();
}

void Point::setIntersectionType(){
    if (parents_intersected.empty()){
        inters_type = Intersection_Type::None;
        return;
    }
    auto it = parents_intersected.begin();
    switch (it->first){
    case GObj_Type::LINE:{
        ++it;
        if (it->first == GObj_Type::LINE)    inters_type = Intersection_Type::Line_Line;
        if (it->first == GObj_Type::RAY)     inters_type = Intersection_Type::Line_Ray;
        if (it->first == GObj_Type::SEGMENT) inters_type = Intersection_Type::Line_Segment;
        if (it->first == GObj_Type::CIRCLE)  inters_type = Intersection_Type::Line_Circle;
    }
        break;
    case GObj_Type::RAY:{
        ++it;
        if (it->first == GObj_Type::RAY)     inters_type = Intersection_Type::Ray_Ray;
        if (it->first == GObj_Type::SEGMENT) inters_type = Intersection_Type::Ray_Segment;
        if (it->first == GObj_Type::CIRCLE)  inters_type = Intersection_Type::Ray_Circle;
    }
        break;
    case GObj_Type::SEGMENT:{
        ++it;
        if (it->first == GObj_Type::SEGMENT) inters_type = Intersection_Type::Segment_Segment;
        if (it->first == GObj_Type::CIRCLE)  inters_type = Intersection_Type::Segment_Circle;
    }
        break;
    case GObj_Type::CIRCLE:{
        ++it;
        if (it->first == GObj_Type::CIRCLE)  inters_type = Intersection_Type::Circle_Circle;
    }
        break;
    default:
        break;
    }
}

void Point::recalculate(){
    checkExistance();
    switch(child_type){
    case Child_Type::Unknown:
    {
        return;
    }
    case Child_Type::Intersection:
    case Child_Type::Intersection2:
    {
        /*Future refactor ideas:
         * 1) Use functions like "bool is_on_Segment(Point* p)"
         * 2) Segment and Ray may have smth like baseLine as an object (so we can easily intersect all of those)
         */
        auto it1 = parents_intersected.begin();
        auto it2 = it1;
        ++it2;
        switch(inters_type){
        case Intersection_Type::Line_Line:{
            auto l1 = static_cast<Line*>(it1->second);
            auto l2 = static_cast<Line*>(it2->second);
            if (std::abs(l1->k() - l2->k()) < EPS) exists = false;
            else{
                exists = true;
                if (l1->isVertical()){
                    X = l1->x0();
                } else if (l2->isVertical()){
                    X = l2->x0();
                } else {
                    X = (l2->y0() - l1->y0() + l1->k() * l1->x0() - l2->k() * l2->x0()) / (l1->k() - l2->k());
                }
                Y = l1->y0() + l1->k() * (X - l1->x0());
                scr_x = board()->getScreenView(QPointF(X, Y)).x();
                scr_y = board()->getScreenView(QPointF(X, Y)).y();
            }
        }
            break;
        case Intersection_Type::Line_Ray:{
            auto l = static_cast<Line*>(it1->second);
            auto r = static_cast<Ray*>(it2->second);
            if (std::abs(l->k() - r->k()) < EPS) exists = false;
            else{
                if (l->isVertical()){
                    X = l->x0();
                } else if (r->isVertical()){
                    X = r->x0();
                } else {
                    X = (r->y0() - l->y0() + l->k() * l->x0() - r->k() * r->x0()) / (l->k() - r->k());
                }
                Y = l->y0() + l->k() * (X - l->x0());
                scr_x = board()->getScreenView(QPointF(X, Y)).x();
                scr_y = board()->getScreenView(QPointF(X, Y)).y();
                if (distance(this, r->getFirstPoint()) < distance(this, r->getSecondPoint()) &&
                        distance(this, r->getFirstPoint()) + distance(this, r->getSecondPoint()) >
                        distance(r->getFirstPoint(), r->getSecondPoint()) + EPS) exists = false;
                else exists = true;
            }
        }
            break;
        case Intersection_Type::Line_Segment:{
            auto l = static_cast<Line*>(it1->second);
            auto s = static_cast<Segment*>(it2->second);
            qreal k = (s->y2() - s->y1())/(s->x2() - s->x1());
            if (std::abs(l->k() - k) < EPS) exists = false;
            else{
                if (l->isVertical()){
                    X = l->x0();
                } else if (std::abs(s->x1() - s->x2()) < EPS){
                    X = s->x1();
                } else {
                    X = (s->y1() - l->y0() + l->k() * l->x0() - k * s->x1()) / (l->k() - k);
                }
                Y = l->y0() + l->k() * (X - l->x0());
                scr_x = board()->getScreenView(QPointF(X, Y)).x();
                scr_y = board()->getScreenView(QPointF(X, Y)).y();
                exists = true;
                if (distance(this, s->getFirstPoint()) + distance(this, s->getSecondPoint()) >
                        distance(s->getFirstPoint(), s->getSecondPoint()) + EPS) exists = false;
            }
        }
            break;
        case Intersection_Type::Line_Circle:{
            auto l = static_cast<Line*>(it1->second);
            auto c = static_cast<Circle*>(it2->second);
            auto sol = get_inter_solution(l, c);
            if (sol.num_points == 0){
                exists = false;
                break;
            }
            if (child_type == Child_Type::Intersection){
                X = sol.x1;
                Y = sol.y1;
                scr_x = board()->getScreenView(QPointF(X, Y)).x();
                scr_y = board()->getScreenView(QPointF(X, Y)).y();
                exists = true;
            } else {
                if (sol.num_points == 1) exists = false;
                else {
                    X = sol.x2;
                    Y = sol.y2;
                    scr_x = board()->getScreenView(QPointF(X, Y)).x();
                    scr_y = board()->getScreenView(QPointF(X, Y)).y();
                    exists = true;
                }
            }
        }
            break;
        case Intersection_Type::Ray_Ray:{
            auto r1 = static_cast<Ray*>(it1->second);
            auto r2 = static_cast<Ray*>(it2->second);
            if (std::abs(r1->k() - r2->k()) < EPS) exists = false;
            else{
                if (r1->isVertical()){
                    X = r1->x0();
                } else if (r2->isVertical()){
                    X = r2->x0();
                } else {
                    X = (r2->y0() - r1->y0() + r1->k() * r1->x0() - r2->k() * r2->x0()) / (r1->k() - r2->k());
                }
                Y = r1->y0() + r1->k() * (X - r1->x0());
                scr_x = board()->getScreenView(QPointF(X, Y)).x();
                scr_y = board()->getScreenView(QPointF(X, Y)).y();
                exists = true;
                if (distance(this, r1->getFirstPoint()) < distance(this, r1->getSecondPoint()) &&
                        distance(this, r1->getFirstPoint()) + distance(this, r1->getSecondPoint()) >
                        distance(r1->getFirstPoint(), r1->getSecondPoint()) + EPS) exists = false;
                if (distance(this, r2->getFirstPoint()) < distance(this, r2->getSecondPoint()) &&
                        distance(this, r2->getFirstPoint()) + distance(this, r2->getSecondPoint()) >
                        distance(r2->getFirstPoint(), r2->getSecondPoint()) + EPS) exists = false;
            }
        }
            break;
        case Intersection_Type::Ray_Segment:{
            auto r = static_cast<Ray*>(it1->second);
            auto s = static_cast<Segment*>(it2->second);
            qreal k = (s->y2() - s->y1())/(s->x2() - s->x1());
            if (std::abs(r->k() - k) < EPS) exists = false;
            else{
                if (r->isVertical()){
                    X = r->x0();
                } else if (std::abs(s->x1() - s->x2()) < EPS){
                    X = s->x1();
                } else {
                    X = (s->y1() - r->y0() + r->k() * r->x0() - k * s->x1()) / (r->k() - k);
                }
                Y = r->y0() + r->k() * (X - r->x0());
                scr_x = board()->getScreenView(QPointF(X, Y)).x();
                scr_y = board()->getScreenView(QPointF(X, Y)).y();
                exists = true;
                if (distance(this, s->getFirstPoint()) + distance(this, s->getSecondPoint()) >
                        distance(s->getFirstPoint(), s->getSecondPoint()) + EPS) exists = false;
                if (distance(this, r->getFirstPoint()) < distance(this, r->getSecondPoint()) &&
                        distance(this, r->getFirstPoint()) + distance(this, r->getSecondPoint()) >
                        distance(r->getFirstPoint(), r->getSecondPoint()) + EPS) exists = false;
            }
        }
            break;
        case Intersection_Type::Ray_Circle:{
            auto r = static_cast<Ray*>(it1->second);
            auto c = static_cast<Circle*>(it2->second);
            auto l = new Line(board(), r);
            auto sol = get_inter_solution(l, c);
            if (sol.num_points == 0){
                exists = false;
                break;
            }
            if (child_type == Child_Type::Intersection){
                X = sol.x1;
                Y = sol.y1;
                scr_x = board()->getScreenView(QPointF(X, Y)).x();
                scr_y = board()->getScreenView(QPointF(X, Y)).y();
            } else {
                if (sol.num_points == 1){
                    exists = false;
                    break;
                }
                else {
                    X = sol.x2;
                    Y = sol.y2;
                    scr_x = board()->getScreenView(QPointF(X, Y)).x();
                    scr_y = board()->getScreenView(QPointF(X, Y)).y();
                }
            }
            if (distance(this, r->getFirstPoint()) < distance(this, r->getSecondPoint()) &&
                    distance(this, r->getFirstPoint()) + distance(this, r->getSecondPoint()) >
                    distance(r->getFirstPoint(), r->getSecondPoint()) + EPS) exists = false;
            else exists = true;
        }
            break;
        case Intersection_Type::Segment_Segment:{
            auto s1 = static_cast<Segment*>(it1->second);
            auto s2 = static_cast<Segment*>(it2->second);
            qreal k1 = (s1->y2() - s1->y1())/(s1->x2() - s1->x1());
            qreal k2 = (s2->y2() - s2->y1())/(s2->x2() - s2->x1());
            if (std::abs(k1 - k2) < EPS) exists = false;
            else{
                if (std::abs(s1->x1() - s1->x2()) < EPS){
                    X = s1->x1();
                } else if (std::abs(s2->x1() - s2->x2()) < EPS){
                    X = s2->x1();
                } else {
                    X = (s2->y1() - s1->y1() + k1 * s1->x1() - k2 * s2->x1()) / (k1 - k2);
                }
                Y = s1->y1() + k1 * (X - s1->x1());
                scr_x = board()->getScreenView(QPointF(X, Y)).x();
                scr_y = board()->getScreenView(QPointF(X, Y)).y();
                exists = true;
                if (distance(this, s1->getFirstPoint()) + distance(this, s1->getSecondPoint()) >
                        distance(s1->getFirstPoint(), s1->getSecondPoint()) + EPS) exists = false;
                if (distance(this, s2->getFirstPoint()) + distance(this, s2->getSecondPoint()) >
                        distance(s2->getFirstPoint(), s2->getSecondPoint()) + EPS) exists = false;
            }
        }
            break;
        case Intersection_Type::Segment_Circle:{
            auto s = static_cast<Segment*>(it1->second);
            auto c = static_cast<Circle*>(it2->second);
            auto l = new Line(board(), s);
            auto sol = get_inter_solution(l, c);
            if (sol.num_points == 0){
                exists = false;
                break;
            }
            if (child_type == Child_Type::Intersection){
                X = sol.x1;
                Y = sol.y1;
                scr_x = board()->getScreenView(QPointF(X, Y)).x();
                scr_y = board()->getScreenView(QPointF(X, Y)).y();
            } else {
                if (sol.num_points == 1){
                    exists = false;
                    break;
                }
                else {
                    X = sol.x2;
                    Y = sol.y2;
                    scr_x = board()->getScreenView(QPointF(X, Y)).x();
                    scr_y = board()->getScreenView(QPointF(X, Y)).y();
                }
            }
            if (distance(this, s->getFirstPoint()) + distance(this, s->getSecondPoint()) >
                    distance(s->getFirstPoint(), s->getSecondPoint()) + EPS) exists = false;
            else exists = true;
        }
            break;
        case Intersection_Type::Circle_Circle:{
            auto c1 = static_cast<Circle*>(it1->second);
            auto c2 = static_cast<Circle*>(it2->second);
            auto deltaX = c1->x0() - c2->x0(), deltaY = c1->y0() - c2->y0();
            if (std::abs(deltaX) < EPS && std::abs(deltaY) < EPS){ //equal centers
                exists = false;
                break;
            }
            if (distance(c1->getcenter(), c2->getcenter()) < std::abs(c1->r() - c2->r())){ //one circle inside another
                exists = false;
                break;
            }
            auto l = new Line(board(), 2 * deltaX, 2 * deltaY, c1->r() * c1->r() - c2->r() * c2->r()
                              - c1->x0() * c1->x0() + c2->x0() * c2->x0() - c1->y0() * c1->y0() + c2->y0() * c2->y0());
            auto sol = get_inter_solution(l, c1);
            if (sol.num_points == 0){
                exists = false;
                break;
            }
            if (child_type == Child_Type::Intersection){
                X = sol.x1;
                Y = sol.y1;
                scr_x = board()->getScreenView(QPointF(X, Y)).x();
                scr_y = board()->getScreenView(QPointF(X, Y)).y();
                exists = true;
            } else {
                if (sol.num_points == 1) exists = false;
                else {
                    X = sol.x2;
                    Y = sol.y2;
                    scr_x = board()->getScreenView(QPointF(X, Y)).x();
                    scr_y = board()->getScreenView(QPointF(X, Y)).y();
                    exists = true;
                }
            }
        }
            break;
        default:
            break;
        }
        break;
    }
    case Child_Type::Middle:
    {
        auto p1 = static_cast<Point*>(parentObjects.at(0));
        auto p2 = static_cast<Point*>(parentObjects.at(1));

        X = (p1->X + p2->X)/2;
        Y = (p1->Y + p2->Y)/2;
        scr_x = (p1->scr_x + p2->scr_x)/2;
        scr_y = (p1->scr_y + p2->scr_y)/2;
        break;
    }
    default:
        break;
    }
}

void Point::draw(){
    if (!exists) return;
    if (!is_visible()) return;
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
    p.drawEllipse(QPointF(scr_x, scr_y), mRadius, mRadius);
}

void Point::move(QPointF newPos){
    if (!depending){
        scr_x = newPos.x();
        scr_y = newPos.y();
        X = board()->getMathPoint(newPos).x();
        Y = board()->getMathPoint(newPos).y();
    }
    emit posChanged();
}

void Point::changeView(){
    scr_x = board()->getScreenView(QPointF(X, Y)).rx();
    scr_y = board()->getScreenView(QPointF(X, Y)).ry();
}

bool Point::isCaught(QPointF p){
    return QLineF(QPointF(scr_x, scr_y), p).length() < mRadius + 2;
}


///LINE METHODS==========================================================================================================


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
    GOBJ(board, GObj_Type::LINE, true, false)
{
    mP1 = 0;
    mP2 = 0;
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
    scr_x0 = mP1->scr_x;
    scr_y0 = mP1->scr_y;
    if (std::abs(mP1->scr_x - mP2->scr_x) < EPS){
        is_vertical = true;
        _k = 0;
    } else {
        is_vertical = false;
        _k = (mP1->scr_y - mP2->scr_y)/(mP1->scr_x - mP2->scr_x);
    }
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


///SEGMENT METHODS==========================================================================================================


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


///RAY METHODS==========================================================================================================


Ray::Ray(GeoBoard* board, Point* p1, Point* p2) :
    GOBJ(board, GObj_Type::RAY, true, true, p1->color()), mP1(p1), mP2(p2)
{
}

void Ray::recalculate(){
    checkExistance();
    scr_x0 = mP1->scr_x;
    scr_y0 = mP1->scr_y;
    if (child_type == Child_Type::Bisector){
        auto p1 = parentObjects[0];
        auto p2 = parentObjects[1];
        auto p3 = parentObjects[2];
        QPointF res1 = QPointF(static_cast<Point*>(p1)->X, static_cast<Point*>(p1)->Y);
        QPointF res2 = QPointF(static_cast<Point*>(p2)->X, static_cast<Point*>(p2)->Y);
        QPointF res3 = QPointF(static_cast<Point*>(p3)->X, static_cast<Point*>(p3)->Y);
        auto L32 = QLineF(res3, res2).length();
        auto L12 = QLineF(res1, res2).length();
        if (L12 < EPS || L32 < EPS){
            exists = false;
            return;
        } else {
            exists = true;
            QPointF dr1 = res1 - res2;
            QPointF dr3 = res3 - res2;
            QPointF dr2 = dr1/L12 + dr3/L32;
            QPointF bisector_point = res2 + dr2;
            mP2->X = bisector_point.x();
            mP2->Y = bisector_point.y();
            mP2->scr_x = mBoard->getScreenView(QPointF(mP2->X, mP2->Y)).x();
            mP2->scr_y = mBoard->getScreenView(QPointF(mP2->X, mP2->Y)).y();
        }
    }
    if (std::abs(mP1->scr_x - mP2->scr_x) < EPS){
        is_vertical = true;
        _k = 0;
    } else {
        is_vertical = false;
        _k = (mP1->scr_y - mP2->scr_y)/(mP1->scr_x - mP2->scr_x);
    }
    _x0 = mP1->X;
    _y0 = mP1->Y;
}

std::pair<QPointF, QPointF> Ray::get_draw_pair(){
    double A = mBoard->width() > mBoard->height() ? mBoard->width() : mBoard->height();
    double S_len = sqrt(mBoard->shift.x() * mBoard->shift.x() + mBoard->shift.y() * mBoard->shift.y());//shift length
    QPointF res1 = QPointF(mP1->scr_x, mP1->scr_y);
    QPointF res2 = QPointF(mP2->scr_x, mP2->scr_y);
    auto L = QLineF(res1, res2).length();
    if (L < EPS) L = EPS;
    if (L > A/2) L = A/2;
    QPointF dr = res1 - res2;
    QPointF p1 = res1;
    QPointF p2 = res1 - (S_len + A) / L * dr;
    return std::make_pair(p1, p2);
}

void Ray::draw(){
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

void Ray::changeView(){
}


bool Ray::isCaught(QPointF p){
    recalculate();
    auto p1 = get_draw_pair().first;
    auto p2 = get_draw_pair().second;
    if (QLineF(p1, p).length() + QLineF(p2, p).length() < QLineF(p1, p2).length() + 0.05) return true;
    return false;
}


void Ray::move(QPointF newPos){
    Q_UNUSED(newPos);
    return;
}



///CIRCLE METHODS==========================================================================================================


Circle::Circle(GeoBoard* board, Point* c, qreal radius) :
    GOBJ(board, GObj_Type::CIRCLE, true, true, c->color()), center(c), _r(radius)
{
}

void Circle::recalculate(){
    checkExistance();
    if (!exists) return;
    if (child_type == Child_Type::OnTwoPoints){
        scr_r = sqrt((basePoints[0]->scr_x - basePoints[1]->scr_x) * (basePoints[0]->scr_x - basePoints[1]->scr_x) +
                     (basePoints[0]->scr_y - basePoints[1]->scr_y) * (basePoints[0]->scr_y - basePoints[1]->scr_y));
        _r = scr_r / mBoard->scale;
    } else { //OnThreePoints
        auto params = getCircleCenterAndRadius(basePoints);
        _r = params.second;
        scr_r = _r * mBoard->scale;
        if(scr_r < EPS){
            exists = false;
            return;
        } else {
            exists = true;
        }
        center->X = params.first.x();
        center->Y = params.first.y();
        center->scr_x = mBoard->getScreenView(QPointF(center->X, center->Y)).x();
        center->scr_y = mBoard->getScreenView(QPointF(center->X, center->Y)).y();
    }
    scr_x0 = center->scr_x;
    scr_y0 = center->scr_y;
    _x0 = center->X;
    _y0 = center->Y;
}

void Circle::draw(){
    if (!exists) return;
    if (!is_visible()) return;
    QPainter p;
    p.begin(mBoard);
    p.setRenderHint(QPainter::Antialiasing);
    if (mIsSelected){
        QPen pen(Qt::blue);
        pen.setWidth(3);
        p.setPen(pen);
        p.drawEllipse(QPointF(scr_x0, scr_y0), scr_r, scr_r);
    }
    p.setPen(mColor);
    p.drawEllipse(QPointF(scr_x0, scr_y0), scr_r, scr_r);
}

void Circle::changeView(){
    recalculate();
}


bool Circle::isCaught(QPointF p){
    recalculate();
    auto L = QLineF(QPointF(scr_x0, scr_y0), p).length();
    if (L< scr_r + 5 && L > scr_r - 5) return true;
    return false;
}


void Circle::move(QPointF newPos){
    Q_UNUSED(newPos);
    return;
}



///OTHER FUNCTIONS==========================================================================================================


std::pair<QPointF, qreal> getCircleCenterAndRadius(const Point* p1, const Point* p2, const Point* p3){
    qreal x = 0;
    qreal y = 0;
    qreal r = 20;
    qreal a1 = 2*(p1->X - p2->X);
    qreal b1 = 2*(p1->Y - p2->Y);
    qreal a2 = 2*(p1->X - p3->X);
    qreal b2 = 2*(p1->Y - p3->Y);
    qreal c1 = (p1->X * p1->X - p2->X * p2->X) + (p1->Y * p1->Y - p2->Y * p2->Y);
    qreal c2 = (p1->X * p1->X - p3->X * p3->X) + (p1->Y * p1->Y - p3->Y * p3->Y);
    qreal d = b2 * a1 - b1 * a2;

    if (std::abs(d) < EPS || (std::abs(a1) < EPS && std::abs(a2) < EPS))
        return std::make_pair(QPointF(0, 0), 0.0); //Three points on the same line

    y = (c2 * a1 - c1 * a2) / d;
    if (std::abs(a2) < EPS) x = (c1 - b1 * y) / a1;
    else x = (c2 - b2 * y) / a2;

    r = sqrt((p1->X - x) * (p1->X - x) + (p1->Y - y) * (p1->Y - y));

    return std::make_pair(QPointF(x, y), r);
}

std::pair<QPointF, qreal> getCircleCenterAndRadius(const std::vector<Point*>& bPoints){
    assert(bPoints.size() == 3);
    return getCircleCenterAndRadius(bPoints[0], bPoints[1], bPoints[2]);
}

qreal distance(const Point* p1, const Point* p2){
    return QLineF(QPointF(p1->X, p1->Y), QPointF(p2->X, p2->Y)).length();
}

qreal sign_distance(const Point* p, const Line* l){
    return (l->k() * p->X - p->Y + (l->y0() - l->k() * l->x0())) / sqrt (1.0 + l->k() * l->k());
}

qreal distance(const Point* p, const Line* l){
    return std::abs(sign_distance(p, l));
}

QPointF getBaseOfPerpendicular(const Point* p, const Line* l){
    qreal d = sign_distance(p, l);
    qreal px = p->X - l->k() * d / sqrt (1.0 + l->k() * l->k());
    qreal py = l->y0() + l->k() * (px - l->x0());
    return QPointF(px, py);
}

intersect_sol get_inter_solution (const Line* l, const Circle* C){
    intersect_sol SOL;

    if (l->isVertical()){
        if (std::abs(l->x0() - C->x0()) > C->r()){
            SOL.num_points = 0;
            return SOL;
        }
        if (std::abs(l->x0() - C->x0()) > C->r() - EPS){
            SOL.num_points = 1;
            SOL.x1 = l->x0();
            SOL.y1 = C->y0();
            return SOL;
        }
        SOL.num_points = 2;
        SOL.x1 = l->x0();
        SOL.y1 = C->y0() - sqrt(C->r()*C->r() - (l->x0() - C->x0())*(l->x0() - C->x0()));
        SOL.x2 = l->x0();
        SOL.y2 = C->y0() + sqrt(C->r()*C->r() - (l->x0() - C->x0())*(l->x0() - C->x0()));
        return SOL;
    }

    qreal dist = distance(C->getcenter(), l);
    auto p = getBaseOfPerpendicular(C->getcenter(), l);

    if (dist > C->r()){
        SOL.num_points = 0;
        return SOL;
    }
    if (dist > C->r() - EPS){
        SOL.num_points = 1;
        SOL.x1 = p.x();
        SOL.y1 = p.y();
        return SOL;
    }
    SOL.num_points = 2;
    qreal z = sqrt(C->r()*C->r() - dist * dist);
    qreal dx = z / sqrt (1.0 + l->k() * l->k());
    SOL.x1 = p.x() + dx;
    SOL.y1 = p.y() + dx * l->k();
    SOL.x2 = p.x() - dx;
    SOL.y2 = p.y() - dx * l->k();
    return SOL;
}
