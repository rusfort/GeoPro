#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"
#include "geometry_main.h"

#include "line.h"
#include "point.h"
#include "ray.h"
#include "segment.h"
#include "circle.h"

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
    if (mIsSelected) mBoard->num_obj_selected--;
    if (child_type != Child_Type::InTriangle){
        for (auto& obj : parentObjects){ //deleting all mentions about this object
            obj->eraseInfoAboutChild(this);
        }
        mBoard->delObject(this);
    } else { //child_type == Child_Type::InTriangle
        if (mIsSelected) mIsSelected = false;
        visible = false;
    }
}

void GOBJ::eraseInfoAboutChild(GOBJ* obj){
    auto res = childObjects.find(obj);
    if (res != childObjects.end()) childObjects.erase(res);
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

bool GOBJ::labelCaught(QPoint p){
    Q_UNUSED(p);
    if (getLabel() == "") return false;
    return false;
}

void GOBJ::moveLabel(QPointF newpos){
    Q_UNUSED(newpos);
}

QString GOBJ::dumpData(){
    return "-1 NO DATA PROVIDED FOR THIS OBJECT";
}

bool GOBJ::dumpParse(QTextStream& stream){
    return generalDumpParse(stream);
}

QString GOBJ::generalDumpData(){ // id | type | color | visible | exists | depending | label | child type
                                 // | num of parents | parent id1 | ... | parent idN
    auto L = getLabel() == "" ? "~" : getLabel();
    QString data = QString::number(obj_id) + " "
            + QString::number((int)type) + " "
            + mColor.name(QColor::HexArgb) + " "
            + QString::number(visible) + " "
            + QString::number(exists) + " "
            + QString::number(depending) + " "
            + L + " "
            + QString::number((int)child_type) + " ";
    data += QString::number(parentObjects.size()) + " ";
    for (auto ch_ob : parentObjects){
        data += QString::number(ch_ob->id()) + " ";
    }
    return data;
}

bool GOBJ::generalDumpParse(QTextStream &stream){
    QString col;
    stream >> col;
    mColor = QColor(col);
    int tmp;
    stream >> tmp;
    exists = tmp;
    stream >> tmp;
    depending = tmp;

    QString l;
    stream >> l;
    if (l != "~") changeLabel(l);

    stream >> tmp;
    child_type = (Child_Type)tmp;

    int n;
    stream >> n;

    for (int i = 0; i < n; ++i) {
        stream >> tmp;
        mBoard->connect_objects(mBoard->parsedObjects[tmp], this, child_type);
    }
    return true;
}

///-----------------------------


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

qreal distance(qreal x1, qreal y1, qreal x2, qreal y2){
    return QLineF(QPointF(x1, y1), QPointF(x2, y2)).length();
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

QPointF getBaseOfPerpendicular(const QPointF* p, const Line* l){
    auto point = new Point(l->board(), p->x(), p->y());
    auto get = getBaseOfPerpendicular(point, l);
    delete point;
    return get;
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
    } else {
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
    }

    //CASE 1: TWO LINE BASE POINTS ARE OUTSIDE THE CIRCLE

    auto dist1 = distance(l->getFirstPoint(), C->getcenter());
    auto dist2 = distance(l->getSecondPoint(), C->getcenter());

    if (dist1 > C->r() && dist2 > C->r()){
        auto p1 = QPointF(l->getFirstPoint()->X, l->getFirstPoint()->Y);
        auto p2 = QPointF(l->getSecondPoint()->X, l->getSecondPoint()->Y);
        auto p3 = QPointF(SOL.x1, SOL.y1);
        auto p4 = QPointF(SOL.x2, SOL.y2);
        if (!rightOrder4(p1, p2, p3, p4) && !rightOrder4(p1, p3, p4, p2) &&
                !rightOrder4(p2, p1, p4, p3) && !rightOrder4(p2, p4, p3, p1)){
            std::swap(SOL.x1, SOL.x2);
            std::swap(SOL.y1, SOL.y2);
        }
        return SOL;
    }

    //CASE 2: AT LEAST ONE OF THE LINE BASE POINTS IS INSIDE THE CIRCLE

    if (!onTheOneSide(QPointF(l->getFirstPoint()->X, l->getFirstPoint()->Y),
            QPointF(l->getSecondPoint()->X, l->getSecondPoint()->Y), QPointF(SOL.x2, SOL.y2)) ||
            !onTheOneSide(QPointF(l->getSecondPoint()->X, l->getSecondPoint()->Y),
            QPointF(l->getFirstPoint()->X, l->getFirstPoint()->Y), QPointF(SOL.x1, SOL.y1))){
        std::swap(SOL.x1, SOL.x2);
        std::swap(SOL.y1, SOL.y2);
    }
    return SOL;
}

bool onTheOneSide(QPointF firstP, QPointF secondP, QPointF thisP){
    QPointF dr = secondP - firstP;
    QPointF v = thisP - firstP;
    return (dr.x()*v.x() + dr.y()*v.y() > 0);
}

bool rightOrder4(QPointF p1, QPointF p2, QPointF p3, QPointF p4){
    return (onTheOneSide(p1, p2, p3) && onTheOneSide(p2, p3, p4) && onTheOneSide(p4, p3, p2) && onTheOneSide(p3, p2, p1));
}

QString getNameOfType(GObj_Type t){
    switch (t) {
    case GObj_Type::POINT:
        return "Point";
    case GObj_Type::LINE:
        return "Line";
    case GObj_Type::RAY:
        return "Ray";
    case GObj_Type::SEGMENT:
        return "Segment";
    case GObj_Type::CIRCLE:
        return "Circle";
    case GObj_Type::ANGLE:
        return "Angle";
    case GObj_Type::TRIANGLE:
        return "Triangle";
    default:
        return "None";
    }
}

QString getNameOfChildType(Child_Type t){
    switch (t) {
    case Child_Type::Parallel:
        return "Parallel";
    case Child_Type::Perpendicular:
        return "Perpendicular";
    case Child_Type::OnTwoPoints:
        return "Standard";
    case Child_Type::Bisector:
        return "Bisector";
    default:
        return "Unknown";
    }
}
