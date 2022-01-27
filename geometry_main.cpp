#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"
#include "geometry_main.h"

STYLE::STYLE(DrStyle st0): style(st0){}
STYLE::~STYLE(){}

GOBJ::GOBJ(GeoBoard* board, GOBJ *pointer_to_obj, GObj_Type t, bool is_depending, QColor color) :
    type(t), depending(is_depending), mColor(color), mBoard(board), g_ptr(pointer_to_obj), mIsSelected(false)
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
    GOBJ(board, this, GObj_Type::POINT, false, color), mRadius(radius)
{
    connect(this, SIGNAL(posChanged()), mBoard, SLOT(update()));
    scr_x = x;
    scr_y = y;
    X = board->getMathPoint(QPointF(x, y)).x();
    Y = board->getMathPoint(QPointF(x, y)).y();
}

void Point::draw(){
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
    GOBJ(board, this, GObj_Type::LINE, true, p1->color()), mP1(p1), mP2(p2)
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

void Line::draw(){
    double A = mBoard->width() > mBoard->height() ? mBoard->width() : mBoard->height();
    QPointF dr = QPointF(mP1->scr_x, mP1->scr_y) - QPointF(mP2->scr_x, mP2->scr_y);
    QPointF p1 = QPointF(mP1->scr_x, mP1->scr_y) + A * dr;
    QPointF p2 = QPointF(mP1->scr_x, mP1->scr_y) - A * dr;

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
    //TODO: BUGS!!!
    recalculate();
    double d = 0;
    if (!is_vertical && std::abs(k) < sqrt(std::numeric_limits<qreal>::max())) d = std::abs(-k * p.rx() + p.ry() + k * scr_x0 - scr_y0)/(1 + k * k);
    else d = std::abs(scr_x0 - p.rx());
    if (d < 3) return true;
    return false;
}


void Line::move(QPointF newPos){
    Q_UNUSED(newPos);
    return;
}


///SEGMENT METHODS==========================================================================================================


Segment::Segment(GeoBoard* board, Point* p1, Point* p2) :
    GOBJ(board, this, GObj_Type::SEGMENT, true, p1->color()), mP1(p1), mP2(p2)
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
