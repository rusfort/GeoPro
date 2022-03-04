#ifndef RAY_H
#define RAY_H

#include "geometry_main.h"

class Ray : public GOBJ
{
    Q_OBJECT
public:
    Ray(GeoBoard* board, Point* p1, Point* p2);
    virtual ~Ray() {}
    void draw() override;
    bool isCaught(QPointF p) override;
    void move(QPointF newPos) override;
    void changeView() override;
    void recalculate() override;
    std::pair<QPointF, QPointF> get_draw_pair();
    qreal k() const {return _k;}
    qreal x0() const {return _x0;}
    qreal y0() const {return _y0;}
    bool isVertical() const {return is_vertical;}
    Point* getFirstPoint() const {return mP1;}
    Point* getSecondPoint() const {return mP2;}
    QString dumpData() override;
signals:
    void posChanged();
private:
    std::vector<Point*> threePoints; //for bisector (order is important)
    Point *mP1, *mP2;
    bool is_vertical;
    qreal _x0, _y0, _k;   //x0, y0 - beginning of the ray, k - incline
    qreal scr_x0, scr_y0; //the same, but actual on the screen
};

#endif // RAY_H
