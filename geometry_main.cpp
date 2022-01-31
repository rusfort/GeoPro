#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"
#include "geometry_main.h"

STYLE::STYLE(DrStyle st0): style(st0){}
STYLE::~STYLE(){}

GOBJ::GOBJ(GeoBoard* board, GObj_Type t, bool is_depending, bool do_exist, QColor color) :
    type(t), depending(is_depending), mColor(color), mBoard(board), mIsSelected(false), exists(do_exist)
{
    connect(this, SIGNAL(selectionChanged()), mBoard, SLOT(update()));
}

void GOBJ::delObj(){
    //for (auto* obj : parentObjects){ //deleting all mentions about this object | BUG IS HERE
        //obj->eraseInfoAboutChild(this);
    //}
    for (const std::pair<GOBJ*, Child_Type>& p : childObjects){ //deleting all children
        p.first->delObj();
    }

    if (mIsSelected) mBoard->num_obj_selected--;
    mBoard->delObject(this);
}

///POINT METHODS==================================================================


Point::Point(GeoBoard* board, double x, double y, double radius,  QColor color) :
    GOBJ(board, GObj_Type::POINT, false, true, color), mRadius(radius)
{
    connect(this, SIGNAL(posChanged()), mBoard, SLOT(update()));
    scr_x = x;
    scr_y = y;
    X = board->getMathPoint(QPointF(x, y)).x();
    Y = board->getMathPoint(QPointF(x, y)).y();
    recalculate();
}

void Point::recalculate(){
    switch(child_type){
    case Child_Type::Unknown:
    {
        return;
    }
    case Child_Type::Intersection:
    {
        //TODO
        break;
    }
    default:
        break;
    }
}

void Point::draw(){
    if (!exists) return;
    recalculate();
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
    scr_x = newPos.x();
    scr_y = newPos.y();
    X = board()->getMathPoint(newPos).x();
    Y = board()->getMathPoint(newPos).y();
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
    scr_x0 = mP1->scr_x;
    scr_y0 = mP1->scr_y;
    if (std::abs(mP1->scr_x - mP2->scr_x) < EPS){
        is_vertical = true;
        k = 0;
    } else {
        is_vertical = false;
        k = (mP1->scr_y - mP2->scr_y)/(mP1->scr_x - mP2->scr_x);
    }
    x0 = mP1->X;
    y0 = mP1->Y;
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
    scr_x1 = mP1->scr_x;
    scr_y1 = mP1->scr_y;
    scr_x2 = mP2->scr_x;
    scr_y2 = mP2->scr_y;
    if (std::abs(mP1->scr_x - mP2->scr_x) < EPS){
        is_vertical = true;
    }
    x1 = mP1->X;
    y1 = mP1->Y;
    x2 = mP2->X;
    y2 = mP2->Y;
    length = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
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
    scr_x0 = mP1->scr_x;
    scr_y0 = mP1->scr_y;
    if (std::abs(mP1->scr_x - mP2->scr_x) < EPS){
        is_vertical = true;
        k = 0;
    } else {
        is_vertical = false;
        k = (mP1->scr_y - mP2->scr_y)/(mP1->scr_x - mP2->scr_x);
    }
    x0 = mP1->X;
    y0 = mP1->Y;
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

