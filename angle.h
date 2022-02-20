#ifndef ANGLE_H
#define ANGLE_H

#include "geometry_main.h"

class Angle : public GOBJ
{
    Q_OBJECT
public:
    Angle(GeoBoard* board, Point* vert, Point* p1, Point* p2); //classic (3 Points)
    Angle(GeoBoard* board, Ray* ray, Point* p);                //by Ray & point
    Angle(GeoBoard* board, Ray* ray, qreal deg);               //by Ray & degree measure
    virtual ~Angle() {}
    void draw() override;
    bool isCaught(QPointF p) override;
    void move(QPointF newPos) override;
    void changeView() override;
    void recalculate() override;
    qreal degrees() const {return _degrees;}
    qreal x0() const {return _x0;}
    qreal y0() const {return _y0;}
    Point* getVertex() const {return vertex;}
    Point* getFirstPoint() const {return mP1;}
    Point* getSecondPoint() const {return mP2;}
signals:
    void posChanged();
private:
    Point *vertex, *mP1, *mP2;
    qreal _degrees;       //degree measure
    qreal _x0, _y0;       //_x0, _y0 - vertex coords
    qreal scr_x0, scr_y0; //the same, but actual on the screen
};

#endif // ANGLE_H
