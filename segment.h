#ifndef SEGMENT_H
#define SEGMENT_H

#include "geometry_main.h"

class Segment : public GOBJ
{
    Q_OBJECT
public:
    Segment(GeoBoard* board, Point* p1, Point* p2);
    virtual ~Segment() {}
    void draw() override;
    bool isCaught(QPointF p) override;
    void move(QPointF newPos) override;
    void changeView() override;
    void recalculate() override;
    qreal getlength() {return length;}
    qreal x1() const {return _x1;}
    qreal y1() const {return _y1;}
    qreal x2() const {return _x2;}
    qreal y2() const {return _y2;}
    Point* getFirstPoint() const {return mP1;}
    Point* getSecondPoint() const {return mP2;}
signals:
    void posChanged();
private:
    Point *mP1, *mP2;
    bool is_vertical;
    qreal _x1, _y1, _x2, _y2;                 //xi, yi - ends
    qreal scr_x1, scr_y1, scr_x2, scr_y2; //the same, but actual on the screen
    qreal length;
};

#endif // SEGMENT_H
