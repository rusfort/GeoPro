#include <iostream>
#include <limits>
#include "math.h"
#include <QPainter>
#include "widget.h"

#include "circle.h"

#include "point.h"
#include "ray.h"
#include "line.h"
#include "segment.h"
#include "angle.h"

Circle::Circle(GeoBoard* board, Point* c, qreal radius) :
    GOBJ(board, GObj_Type::CIRCLE, true, true, c->color()), center(c), _r(radius)
{
}

void Circle::recalculate(){
    checkExistance();
    if (!exists) return;
    if (child_type == Child_Type::InTriangle) return;
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
    changeView();
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
    //recalculate();
    scr_x0 = center->scr_x;
    scr_y0 = center->scr_y;
    _x0 = center->X;
    _y0 = center->Y;
    scr_r = _r * mBoard->scale;
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

QString Circle::dumpData(){
    QString data = "1 ";
    if (child_type != Child_Type::OnThreePoints) data += QString::number(center->id()) + " ";
    if (child_type != Child_Type::InTriangle) data += QString::number(basePoints[0]->id()) + " "
                                                     + QString::number(basePoints[1]->id()) + " ";
    else {
        data += QString::number((int)tr_type) + " ";
    }
    if (child_type == Child_Type::OnThreePoints) data += QString::number(basePoints[2]->id()) + " ";
    return data;
}

bool Circle::dumpParse(QTextStream& stream){
    if (!generalDumpParse(stream)) return false;
    int check_num;
    stream >> check_num;
    if (check_num != 1){
        stream.readLine();
        return false;
    }

    int tmp;
    if (child_type != Child_Type::OnThreePoints){
        stream >> tmp;
        setcenter(static_cast<Point*>(mBoard->parsedObjects[tmp]));
    }

    if (child_type == Child_Type::InTriangle){
        stream >> tmp;
        tr_type = (Triangle_Obj)tmp;
        return true;
    }

    stream >> tmp;
    basePoints.push_back(static_cast<Point*>(mBoard->parsedObjects[tmp]));
    stream >> tmp;
    basePoints.push_back(static_cast<Point*>(mBoard->parsedObjects[tmp]));
    if (child_type == Child_Type::OnThreePoints){
        stream >> tmp;
        basePoints.push_back(static_cast<Point*>(mBoard->parsedObjects[tmp]));
    }
    return true;
}
