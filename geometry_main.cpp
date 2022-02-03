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
    {
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
            //TODO
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
            //TODO
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
            //TODO
        }
            break;
        case Intersection_Type::Circle_Circle:{
            //TODO
        }
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void Point::draw(){
    if (!exists) return;
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
    double dx = scr_x - p.x();
    double dy = scr_y - p.y();
    return dx * dx + dy * dy - mRadius * mRadius < 0;
}


///LINE METHODS==========================================================================================================


Line::Line(GeoBoard* board, Point* p1, Point* p2) :
    GOBJ(board, GObj_Type::LINE, true, true, p1->color()), mP1(p1), mP2(p2)
{
    recalculate();
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
    QPointF res1 = QPointF(mP1->scr_x, mP1->scr_y);
    QPointF res2 = QPointF(mP2->scr_x, mP2->scr_y);
    auto L = QLineF(res1, res2).length();
    if (L < EPS) L = EPS;
    if (L > A/2) L = A/2;
    QPointF dr = res1 - res2;
    QPointF p1 = res1 + A / L * dr;
    QPointF p2 = res1 - A / L * dr;
    return std::make_pair(p1, p2);
}

void Line::draw(){
    if (!exists) return;
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
    recalculate();
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
    recalculate();
}

void Ray::recalculate(){
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

std::pair<QPointF, QPointF> Ray::get_draw_pair(){
    double A = mBoard->width() > mBoard->height() ? mBoard->width() : mBoard->height();
    QPointF res1 = QPointF(mP1->scr_x, mP1->scr_y);
    QPointF res2 = QPointF(mP2->scr_x, mP2->scr_y);
    auto L = QLineF(res1, res2).length();
    if (L < EPS) L = EPS;
    if (L > A/2) L = A/2;
    QPointF dr = res1 - res2;
    QPointF p1 = res1;
    QPointF p2 = res1 - A / L * dr;
    return std::make_pair(p1, p2);
}

void Ray::draw(){
    if (!exists) return;
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
    if (L< scr_r + 1 && L > scr_r - 1) return true;
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

