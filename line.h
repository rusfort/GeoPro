#ifndef LINE_H
#define LINE_H

#include "geometry_main.h"

class Line : public GOBJ
{
    Q_OBJECT
public:
    Line(GeoBoard* board, Point* p1, Point* p2);      //classic (2 Points)
    Line(GeoBoard* board, Ray* ray);                  //by Ray
    Line(GeoBoard* board, Segment* seg);              //by Segment
    Line(GeoBoard* board, qreal A, qreal B, qreal C); //by equation: Ax + By + C = 0
    virtual ~Line() {}
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
    void setBaseLine(GOBJ* l){baseline = l;}
    void setBasePoint(Point* p){mP1 = p;}
    Point* getFirstPoint() const {return mP1;}
    Point* getSecondPoint() const {return mP2;}
    QString dumpData() override;
    bool dumpParse(QTextStream& stream) override;
signals:
    void posChanged();
private:
    Point *mP1, *mP2;
    GOBJ *baseline = 0; //needed if this = parallel/perpendicular
    bool is_vertical;
    qreal _x0, _y0, _k;   //_x0, _y0 - some point on a line, _k - incline
    qreal scr_x0, scr_y0; //the same, but actual on the screen
};

#endif // LINE_H
