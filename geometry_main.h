#ifndef GEOMETRY_MAIN_H
#define GEOMETRY_MAIN_H

#include <QObject>
#include <QWidget>
#include <QPointF>
#include <QColor>
#include <vector>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QList>

#define EPS 0.0001

class GeoBoard;
class Line;
class Segment;

enum class DrStyle{
    None,
    Light,
    Bold
};

enum class GObj_Type{
    NONE,
    POINT,
    LINE,
    SEGMENT
};

class GOBJ : public QObject{
    Q_OBJECT
public:
    GOBJ(GeoBoard* board, GObj_Type t = GObj_Type::NONE, bool is_depending = false, QColor color = Qt::black);
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
    virtual void draw() = 0;
    virtual bool isCaught(QPointF p) = 0;
    virtual void move(QPointF dr) = 0;
    virtual void changeView() = 0;
signals:
    void selectionChanged();
protected:
    const GObj_Type type;
    bool depending;
    QColor mColor;
    GeoBoard* mBoard;
    bool mIsSelected;
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

class Point : public GOBJ, public QPointF
{
    Q_OBJECT
public:
    Point(GeoBoard* board, double x = 0.0, double y = 0.0, double radius = 1.0,  QColor color = Qt::black);
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
    void draw() override;
    bool isCaught(QPointF p) override;
    void move(QPointF newPos) override;
    void changeView() override;
    void recalculate();
signals:
    void posChanged();
private:
    Point *mP1, *mP2;
    bool is_vertical;
    qreal x0, y0, k;      //x0, y0 - some point on a line, k - incline
    qreal scr_x0, scr_y0; //the same, but actual on the screen
};

#endif // GEOMETRY_MAIN_H
