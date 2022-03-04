#ifndef CIRCLE_H
#define CIRCLE_H

#include "geometry_main.h"

class Circle : public GOBJ
{
    Q_OBJECT
public:
    Circle(GeoBoard* board, Point* c, qreal radius);
    virtual ~Circle() {}
    void draw() override;
    bool isCaught(QPointF p) override;
    void move(QPointF newPos) override;
    void changeView() override;
    void recalculate() override;
    qreal r() const {return _r;}
    qreal x0() const {return _x0;}
    qreal y0() const {return _y0;}
    const Point* getcenter() const {return center;}
    void set_r(qreal newr) {_r = newr;}
    QString dumpData() override;
signals:
    void posChanged();
private:
    Point *center;
    qreal _x0, _y0, _r;   //_x0, _y0 - center point math coords, _r - radius
    qreal scr_x0, scr_y0, scr_r; //the same, but actual on the screen
public:
    std::vector<Point*> basePoints;
};

#endif // CIRCLE_H
