#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"

#include "ray.h"

#include "point.h"
#include "line.h"
#include "segment.h"
#include "circle.h"
#include "angle.h"

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
    QPointF p2 = res1 - (S_len / mBoard->scale + A) / L * dr;
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

QString Ray::dumpData(){
    QString data = "1 ";
    if (child_type != Child_Type::Bisector) data += QString::number(mP1->id()) + " "
                                                  + QString::number(mP2->id()) + " ";
    return data;
}

bool Ray::dumpParse(QTextStream& stream){
    if (!generalDumpParse(stream)) return false;;
    //TODO
    return true;
}
