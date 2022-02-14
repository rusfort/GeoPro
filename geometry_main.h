#ifndef GEOMETRY_MAIN_H
#define GEOMETRY_MAIN_H

#include <QObject>
#include <QWidget>
#include <QPointF>
#include <QColor>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QList>
#include <utility>

#include "widget.h"
#include "service.h"

#define EPS 0.0001

class GOBJ;
class Line;
class Ray;
class Segment;

class GOBJ : public QObject{
    Q_OBJECT
public:
    GOBJ(GeoBoard* board, GObj_Type t = GObj_Type::NONE, bool is_depending = false, bool do_exist = true, QColor color = Qt::black);
    virtual ~GOBJ() { }
private:
    bool visible = true;
public:
    inline bool is_visible() const{
        return visible;
    }
    inline void hide(){
        visible = false;
    }
    inline void make_visible(){
        visible = true;
    }
    inline GObj_Type type_is() const{
        return type;
    }
    QColor color() const{
        return mColor;
    }
    void setColor(QColor color){
        mColor = color;
    }
    bool isSelected() const{
        return mIsSelected;
    }
    void setSelected(bool mode = true){
        if(mIsSelected != mode){
            mIsSelected = mode;
            emit selectionChanged();
        }
    }
    void eraseInfoAboutChild(GOBJ* obj){
        auto res = childObjects.find(obj);
        if (res != childObjects.end()) childObjects.erase(res);
    }
    QString getLabel() const {return obj_label;}
    void changeLabel(const QString& new_label) {obj_label = new_label;}
    void delObj();
    void checkExistance();
    virtual void draw() = 0;
    virtual bool isCaught(QPointF p) = 0;
    virtual void move(QPointF dr) = 0;
    virtual void changeView() = 0;
    virtual void recalculate() = 0;
signals:
    void selectionChanged();
protected:
    const GObj_Type type;
    QColor mColor;
    GeoBoard* mBoard;
    bool mIsSelected;
    QString obj_label = "";
public:
    bool depending;
    std::map<GOBJ*, Child_Type> childObjects;
    std::vector<GOBJ*> parentObjects;                    //vector of all parents
    Child_Type child_type = Child_Type::Unknown;         //if this object is a child of smth
    bool exists;   //e.g. intersection can be none (if the user tries to intersect two parallel lines)
};

class STYLE{ //temporarily non-used
protected:
    DrStyle style;
public:
    STYLE(DrStyle st0);
    ~STYLE();
    inline void change_style(DrStyle st){
        style = st;
    }
    inline DrStyle get_style(DrStyle st) const{
        return st;
    }
};

class Point : public GOBJ
{
    Q_OBJECT
public:
    Point(GeoBoard* board, double x = 0.0, double y = 0.0, double radius = 5.0,  QColor color = Qt::black);
    virtual ~Point() {}
    GeoBoard* board() const { return mBoard; }
    void setBoard(GeoBoard* board) { mBoard = board; }
    double rad() const { return mRadius; }
    void setRad(double rad) { mRadius = rad; }
    void setIntersectionType();
    void draw() override;
    bool isCaught(QPointF p) override;
    void move(QPointF newPos) override;
    void changeView() override;
    void recalculate() override;
signals:
    void posChanged();
private:
    double mRadius;
public:
    qreal X, Y;          //mathematical (native) coords
    qreal scr_x, scr_y;  //screen coords
    Intersection_Type inters_type = Intersection_Type::None;
    std::multimap<GObj_Type, GOBJ*> parents_intersected; //2 objects ordered by their type (to easily get the Intersection_Type)
};

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
signals:
    void posChanged();
private:
    Point *mP1, *mP2;
    bool is_vertical;
    qreal _x0, _y0, _k;   //_x0, _y0 - some point on a line, _k - incline
    qreal scr_x0, scr_y0; //the same, but actual on the screen
};

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
signals:
    void posChanged();
private:
    std::vector<Point*> threePoints; //for bisector (order is important)
    Point *mP1, *mP2;
    bool is_vertical;
    qreal _x0, _y0, _k;   //x0, y0 - beginning of the ray, k - incline
    qreal scr_x0, scr_y0; //the same, but actual on the screen
};

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
    std::pair<QPointF, QPointF> get_draw_pair();
    qreal r() const {return _r;}
    qreal x0() const {return _x0;}
    qreal y0() const {return _y0;}
    const Point* getcenter() const {return center;}
signals:
    void posChanged();
private:
    Point *center;
    qreal _x0, _y0, _r;   //_x0, _y0 - center point math coords, _r - radius
    qreal scr_x0, scr_y0, scr_r; //the same, but actual on the screen
public:
    std::vector<Point*> basePoints;
};

//-------------------------------------------------------------------------

struct intersect_sol //solution of line+circle intersection
{
    int num_points = 0;
    qreal x1;
    qreal y1;
    qreal x2;
    qreal y2;
};


std::pair<QPointF, qreal> getCircleCenterAndRadius(const Point* p1, const Point* p2, const Point* p3);
std::pair<QPointF, qreal> getCircleCenterAndRadius(const std::vector<Point *> &bPoints);
qreal sign_distance(const Point* p, const Line* l);
qreal distance(const Point* p1, const Point* p2);
qreal distance(const Point* p, const Line* l);
QPointF getBaseOfPerpendicular(const Point* p, const Line* l);
intersect_sol get_inter_solution (const Line* l, const Circle* C);

#endif // GEOMETRY_MAIN_H
