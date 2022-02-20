#ifndef POINT_H
#define POINT_H

#include "geometry_main.h"

class Point : public GOBJ
{
    Q_OBJECT
public:
    Point(GeoBoard* board, double x = 0.0, double y = 0.0, double radius = 5.0,  QColor color = Qt::black);
    virtual ~Point() {}
    double rad() const { return mRadius; }
    void setRad(double rad) { mRadius = rad; }
    void setIntersectionType();
    void setFixOnFigure(GOBJ* Figure);
    void draw() override;
    bool isCaught(QPointF p) override;
    void move(QPointF newPos) override;
    void changeView() override;
    void recalculate() override;
    bool labelCaught(QPoint p) override;
    void moveLabel(QPointF newpos) override;
signals:
    void posChanged();
private:
    double mRadius;
public:
    qreal X, Y;          //mathematical (native) coords
    qreal mX, mY;        //previous math coords (e.g. for intersection swap check)
    qreal scr_x, scr_y;  //screen coords
    qreal k = 0;         //unused when the Point is free or intersection
    Intersection_Type inters_type = Intersection_Type::None;
    std::multimap<GObj_Type, GOBJ*> parents_intersected; //2 objects ordered by their type (to easily get the Intersection_Type)
};

#endif // POINT_H
