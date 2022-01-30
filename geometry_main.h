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
class Segment;

class GOBJ : public QObject{
    Q_OBJECT
public:
    GOBJ(GeoBoard* board, GOBJ* pointer_to_obj, GObj_Type t = GObj_Type::NONE, bool is_depending = false, bool do_exist = true, QColor color = Qt::black);
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
    /*void eraseInfoAboutChild(GOBJ* obj){
        auto res = childObjects.find(obj);
        if (res != childObjects.end()) childObjects.erase(res);
    }*/
    void delObj();
    GOBJ* g() {return g_ptr;}
    virtual void draw() = 0;
    virtual bool isCaught(QPointF p) = 0;
    virtual void move(QPointF dr) = 0;
    virtual void changeView() = 0;
signals:
    void selectionChanged();
protected:
    const GObj_Type type;
    bool depending;
    bool exists;   //e.g. intersection can be none
    QColor mColor;
    GeoBoard* mBoard;
    GOBJ* g_ptr = 0; //pointer to a real geometry object
    bool mIsSelected;
public:
    std::map<GOBJ*, Child_Type> childObjects;
    std::vector<GOBJ*> parentObjects;
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
    Point(GeoBoard* board, double x = 0.0, double y = 0.0, double radius = 1.0,  QColor color = Qt::black);
    virtual ~Point() {}
    GeoBoard* board() const { return mBoard; }
    void setBoard(GeoBoard* board) { mBoard = board; }
    double rad() const { return mRadius; }
    void setRad(double rad) { mRadius = rad; }
    void draw() override;
    bool isCaught(QPointF p) override;
    void move(QPointF newPos) override;
    void changeView() override;
signals:
    void posChanged();
private:
    double mRadius;
public:
    qreal X, Y;
    qreal scr_x, scr_y;
};

class Line : public GOBJ
{
    Q_OBJECT
public:
    Line(GeoBoard* board, Point* p1, Point* p2);
    virtual ~Line() {}
    void draw() override;
    bool isCaught(QPointF p) override;
    void move(QPointF newPos) override;
    void changeView() override;
    void recalculate();
    std::pair<QPointF, QPointF> get_draw_pair();
signals:
    void posChanged();
private:
    Point *mP1, *mP2;
    bool is_vertical;
    qreal x0, y0, k;      //x0, y0 - some point on a line, k - incline
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
    void recalculate();
    qreal getlength() {return length;}
signals:
    void posChanged();
private:
    Point *mP1, *mP2;
    bool is_vertical;
    qreal x1, y1, x2, y2; //xi, yi - ends
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
    void recalculate();
    std::pair<QPointF, QPointF> get_draw_pair();
signals:
    void posChanged();
private:
    Point *mP1, *mP2;
    bool is_vertical;
    qreal x0, y0, k;      //x0, y0 - beginning of the ray, k - incline
    qreal scr_x0, scr_y0; //the same, but actual on the screen
};

#endif // GEOMETRY_MAIN_H
