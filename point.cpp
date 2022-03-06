#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"

#include "point.h"

#include "ray.h"
#include "line.h"
#include "segment.h"
#include "circle.h"
#include "angle.h"

Point::Point(GeoBoard* board, double x, double y, double radius, QColor color) :
    GOBJ(board, GObj_Type::POINT, false, true, color), mRadius(radius)
{
    connect(this, SIGNAL(posChanged()), mBoard, SLOT(update()));
    scr_x = x;
    scr_y = y;
    X = board->getMathPoint(QPointF(x, y)).x();
    Y = board->getMathPoint(QPointF(x, y)).y();
    mX = X;
    mY = Y;
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
            l->recalculate();
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
            l->recalculate();
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
    case Child_Type::OnSegment:
    {
        auto s = static_cast<Segment*>(parentObjects.at(0));
        auto p1 = s->getFirstPoint();
        auto p2 = s->getSecondPoint();

        if (std::abs(k) < EPS){
            if (std::abs(p1->X - p2->X) < EPS){
                k =  (Y - p1->Y) / (p2->Y - p1->Y);
            } else k = (X - p1->X) / (p2->X - p1->X);
        }

        X = p1->X + k * (p2->X - p1->X);
        Y = p1->Y + k * (p2->Y - p1->Y);
        scr_x = p1->scr_x + k * (p2->scr_x - p1->scr_x);
        scr_y = p1->scr_y + k * (p2->scr_y - p1->scr_y);
        break;
    }
    case Child_Type::OnRay:
    {
        auto r = static_cast<Ray*>(parentObjects.at(0));
        auto p1 = r->getFirstPoint();
        auto p2 = r->getSecondPoint();

        if (std::abs(k) < EPS){
            if (std::abs(p1->X - p2->X) < EPS){
                k =  (Y - p1->Y) / (p2->Y - p1->Y);
            } else k = (X - p1->X) / (p2->X - p1->X);
        }

        X = p1->X + k * (p2->X - p1->X);
        Y = p1->Y + k * (p2->Y - p1->Y);
        scr_x = p1->scr_x + k * (p2->scr_x - p1->scr_x);
        scr_y = p1->scr_y + k * (p2->scr_y - p1->scr_y);
        break;
    }
    case Child_Type::OnLine:
    {
        auto l = static_cast<Line*>(parentObjects.at(0));
        auto p1 = l->getFirstPoint();
        auto p2 = l->getSecondPoint();

        if (std::abs(k) < EPS){
            if (std::abs(p1->X - p2->X) < EPS){
                k =  (Y - p1->Y) / (p2->Y - p1->Y);
            } else k = (X - p1->X) / (p2->X - p1->X);
        }

        X = p1->X + k * (p2->X - p1->X);
        Y = p1->Y + k * (p2->Y - p1->Y);
        scr_x = p1->scr_x + k * (p2->scr_x - p1->scr_x);
        scr_y = p1->scr_y + k * (p2->scr_y - p1->scr_y);
        break;
    }
    case Child_Type::OnCircle:
    {
        auto c = static_cast<Circle*>(parentObjects.at(0));
        auto p = c->getcenter();
        auto r = c->r();

        if (std::abs(k) < EPS){
            if(Y > p->Y) k = acos((X - p->X)/r);
            else k = -acos((X - p->X)/r);
        }

        X = p->X + r * cos(k);
        Y = p->Y + r * sin(k);
        scr_x = p->scr_x + r * mBoard->scale * cos(k);
        scr_y = p->scr_y + r * mBoard->scale * sin(k);
        break;
    }
    default:
        break;
    }

    mX = X;
    mY = Y;
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

    p.setFont(QFont(obj_label.font, obj_label.font_size, QFont::Bold));
    QFontMetrics metrics = p.fontMetrics();
    obj_label.stringHeight = metrics.ascent() + metrics.descent();
    obj_label.stringWidth = metrics.width(obj_label.label);
    obj_label.left = scr_x + obj_label.pos_x;
    obj_label.top = scr_y + obj_label.pos_y - metrics.ascent();
    p.drawText(QPointF(scr_x + obj_label.pos_x, scr_y + obj_label.pos_y), obj_label.label);
}

void Point::move(QPointF newPos){
    if (fixed) return;
    if (!depending){
        scr_x = newPos.x();
        scr_y = newPos.y();
        X = board()->getMathPoint(newPos).x();
        Y = board()->getMathPoint(newPos).y();
    } else if (child_type >= Child_Type::OnLine && child_type <= Child_Type::OnCircle){
        if (child_type == Child_Type::OnCircle){
            auto c = static_cast<Circle*>(parentObjects.at(0));
            auto cen = mBoard->getScreenView(QPointF(c->x0(), c->y0()));
            auto L = QLineF(cen, newPos).length();
            auto P = cen + c->r() * mBoard->scale / L * (newPos - cen);
            scr_x = P.x();
            scr_y = P.y();
            X = board()->getMathPoint(P).x();
            Y = board()->getMathPoint(P).y();
        } else {
            Line* l;
            if (child_type == Child_Type::OnLine) l = static_cast<Line*>(parentObjects.at(0));
            if (child_type == Child_Type::OnRay) l = new Line(mBoard, static_cast<Ray*>(parentObjects.at(0)));
            if (child_type == Child_Type::OnSegment) l = new Line(mBoard, static_cast<Segment*>(parentObjects.at(0)));
            l->recalculate();
            auto P = board()->getScreenView(getBaseOfPerpendicular(&newPos, l));

            switch (child_type) {
            case Child_Type::OnLine:{
                scr_x = P.x();
                scr_y = P.y();
                X = board()->getMathPoint(P).x();
                Y = board()->getMathPoint(P).y();
                break;
            }
            case Child_Type::OnSegment:{
                auto s = static_cast<Segment*>(parentObjects.at(0));
                if (s->isCaught(P)){
                    scr_x = P.x();
                    scr_y = P.y();
                    X = board()->getMathPoint(P).x();
                    Y = board()->getMathPoint(P).y();
                }
                break;
            }
            case Child_Type::OnRay:{
                auto r = static_cast<Ray*>(parentObjects.at(0));
                if (r->isCaught(P)){
                    scr_x = P.x();
                    scr_y = P.y();
                    X = board()->getMathPoint(P).x();
                    Y = board()->getMathPoint(P).y();
                }
                break;
            }
            default:
                break;
            }

            if (child_type != Child_Type::OnLine) delete l;
        }
    } else return;
    k = 0;
    recalculate();
    emit posChanged();
}

void Point::changeView(){
    scr_x = board()->getScreenView(QPointF(X, Y)).rx();
    scr_y = board()->getScreenView(QPointF(X, Y)).ry();
}

bool Point::isCaught(QPointF p){
    return QLineF(QPointF(scr_x, scr_y), p).length() < mRadius + 2;
}

bool Point::labelCaught(QPoint p){
    if (getLabel() == "") return false;
    return (p.x() >= obj_label.left && p.y() >= obj_label.top &&
            p.x() <= obj_label.left + obj_label.stringWidth &&
            p.y() <= obj_label.top + obj_label.stringHeight);
}

void Point::moveLabel(QPointF newpos){
    const int max_offset = 5;
    if (newpos.x() >= scr_x - mRadius - max_offset - obj_label.stringWidth && newpos.x() <= scr_x + mRadius + max_offset)
        obj_label.pos_x = newpos.x() - scr_x;
    auto ascent = scr_y + obj_label.pos_y - obj_label.top;
    auto descent = obj_label.stringHeight - ascent;
    if (newpos.y() >= scr_y - mRadius - max_offset - descent && newpos.y() <= scr_y + mRadius + max_offset + ascent)
        obj_label.pos_y = newpos.y() - scr_y;
}

void Point::setFixOnFigure(GOBJ* Figure){
    switch (Figure->type_is()) {
    case GObj_Type::SEGMENT:{
        child_type = Child_Type::OnSegment;
        Figure->childObjects[this] = Child_Type::OnSegment;
        break;
    }
    case GObj_Type::RAY:{
        child_type = Child_Type::OnRay;
        Figure->childObjects[this] = Child_Type::OnRay;
        break;
    }
    case GObj_Type::LINE:{
        child_type = Child_Type::OnLine;
        Figure->childObjects[this] = Child_Type::OnLine;
        break;
    }
    case GObj_Type::CIRCLE:{
        child_type = Child_Type::OnCircle;
        Figure->childObjects[this] = Child_Type::OnCircle;
        break;
    }
    default:
        break;
    }
    depending = true;
    exists = true;
    parentObjects.push_back(Figure);
    recalculate();
}

QString Point::dumpData(){
    QString data = "1 " + QString::number(mRadius) + " ";
    switch(child_type){
    case Child_Type::Unknown:
    {
        data += QString::number(X) + " " + QString::number(Y) + " ";
        break;
    }
    case Child_Type::Intersection:
    case Child_Type::Intersection2:
    {

        auto it1 = parents_intersected.begin();
        auto it2 = it1;
        ++it2;
        data += QString::number((int)inters_type) + " "
                + QString::number(it1->second->id()) + " " + QString::number(it2->second->id()) + " ";
        break;
    }
    case Child_Type::Middle:
    {
        data += QString::number(parentObjects.at(0)->id()) + " " + QString::number(parentObjects.at(1)->id()) + " ";
        break;
    }
    case Child_Type::Triangle:
    {
        data += QString::number((int)tr_type) + " ";
        break;
    }
    default:
    {
        QString::number(parentObjects.at(0)->id()) + " " + QString::number(k) + " ";
        break;
    }
    }

    return data;
}

bool Point::dumpParse(QTextStream& stream){
    if (!generalDumpParse(stream)) return false;
    int check_num;
    stream >> check_num;
    if (check_num != 1){
        stream.readLine();
        return false;
    }
    int tmp;
    stream >> tmp;
    mRadius = tmp;

    switch(child_type){
    case Child_Type::Unknown:
    {
        qreal z;
        stream >> z;
        X = z;
        stream >> z;
        Y = z;
        break;
    }
    case Child_Type::Intersection:
    case Child_Type::Intersection2:
    {
        stream >> tmp;
        inters_type = (Intersection_Type)tmp;
        stream >> tmp;
        parents_intersected.insert(std::make_pair(mBoard->parsedObjects[tmp]->type_is(), mBoard->parsedObjects[tmp]));
        stream >> tmp;
        parents_intersected.insert(std::make_pair(mBoard->parsedObjects[tmp]->type_is(), mBoard->parsedObjects[tmp]));
        break;
    }
    case Child_Type::Middle:
    {
        stream >> tmp;
        parentObjects[0] = mBoard->parsedObjects[tmp];
        stream >> tmp;
        parentObjects[1] = mBoard->parsedObjects[tmp];
        break;
    }
    case Child_Type::InTriangle:
    {
        stream >> tmp;
        tr_type = (Triangle_Obj)tmp;
        break;
    }
    default:
    {
        stream >> tmp;
        parentObjects[0] = mBoard->parsedObjects[tmp];
        stream >> k;
        break;
    }
    }

    //stream.readLine();

    return true;
}
