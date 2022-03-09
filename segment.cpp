#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"

#include "segment.h"

#include "point.h"
#include "line.h"
#include "ray.h"
#include "circle.h"
#include "angle.h"

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

    //sticks calculations
    auto r = p2 - p1;
    qreal dr = distance(0, 0, r.x(), r.y());
    qreal ax, ay;
    if (std::abs(r.x()) < EPS){
        ax = 1;
        ay = 0;
    } else {
        ay = 1;
        ax = - r.y() / r.x();
    }
    qreal d = sqrt(ax * ax + ay * ay);
    ax *= 5/d;
    ay *= 5/d;
    auto mid = QPointF((p1.x() + p2.x())/2, (p1.y() + p2.y())/2);
    auto stick = QPointF(ax, ay);
    //

    p.setPen(mColor);
    if (mIsSelected){
        QPen pen(Qt::blue);
        pen.setWidth(3);
        p.setPen(pen);
    }
    p.drawLine(p1, p2);

    if (num_sticks > 0) p.drawLine(mid + stick, mid - stick);
    if (num_sticks > 1) p.drawLine(mid + 3/dr * r + stick, mid + 3/dr * r - stick);
    if (num_sticks > 2) p.drawLine(mid - 3/dr * r + stick, mid - 3/dr * r - stick);
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

QString Segment::dumpData(){
    QString data = "1 ";
    data += QString::number((int)tr_type) + " ";
    return data;
}

bool Segment::dumpParse(QTextStream& stream){
    if (!generalDumpParse(stream)) return false;
    int check_num;
    stream >> check_num;
    if (check_num != 1){
        stream.readLine();
        return false;
    }

    int tmp;
    stream >> tmp;
    tr_type = (Triangle_Obj)tmp;

    if(tr_type == Triangle_Obj::Not_in_Triangle){
        setEnds(static_cast<Point*>(parentObjects.at(0)), static_cast<Point*>(parentObjects.at(1)));
    }

    return true;
}
