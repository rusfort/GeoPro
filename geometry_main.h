#ifndef GEOMETRY_MAIN_H
#define GEOMETRY_MAIN_H

#include <QGraphicsItem>
#include <QList>

class GraphWidget;
class QGraphicsSceneMouseEvent;

enum class DrStyle{
    None,
    Light,
    Bold
};

enum class GObj_Type{
    NONE,
    POINT,
    LINE
};

class GOBJ{
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
};

class STYLE{
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

class Point : public QGraphicsItem, public GOBJ, public STYLE{
public:
    Point(GraphWidget *graphWidget, double x0, double y0, bool keptbymouse = false, DrStyle st0 = DrStyle::None);
    ~Point();

    bool advance();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    GraphWidget *graph;
    QPointF newPos;
public:
    double x;
    double y;
    int color;
    bool kbm; //if the point is just created
};

class Line : public QGraphicsItem, public GOBJ, public STYLE{
public:
    Line(GraphWidget *graphWidget, double x01, double y01, double x02, double y02, DrStyle st0 = DrStyle::None);
    ~Line();
private:
    GraphWidget *graph;
public:
    Point p1;
    Point p2;
};

#endif // GEOMETRY_MAIN_H
