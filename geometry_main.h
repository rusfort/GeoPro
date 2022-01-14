#ifndef GEOMETRY_MAIN_H
#define GEOMETRY_MAIN_H

#include <QGraphicsItem>
#include <QList>

class GraphWidget;
class QGraphicsSceneMouseEvent;

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
    Point(Point* p0);
    ~Point();

    inline void setsize(int size0){
        if (size0 > 0) size = size0;
    }
    inline GraphWidget* getgw() const{
        return graph;
    }

    void addline(Line * l);
    inline QList<Line *> lines() const{
        return lineList;
    }

    void addseg(Segment * s);
    inline QList<Segment *> segments() const{
        return segList;
    }

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
    QList<Line *> lineList;
    QList<Segment *> segList;
public:
    double x;
    double y;
    int color;
    bool kbm; //if the point is just created
    int size;
};

class Line : public QGraphicsItem, public GOBJ, public STYLE{
public:
    Line(GraphWidget *graphWidget, Point* p1, Point* p2, DrStyle st0 = DrStyle::None);
    ~Line();

    bool advance();
    void adjust();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    //QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    GraphWidget *graph;
public:
    Point* p1_;
    Point* p2_;
    QPointF sourcePoint;
    QPointF destPoint;
    int color;
};

class Segment : public QGraphicsItem, public GOBJ, public STYLE{
public:
    Segment(GraphWidget *graphWidget, Point* p1, Point* p2, DrStyle st0 = DrStyle::None);
    ~Segment();

    bool advance();
    void adjust();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    //QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    GraphWidget *graph;
public:
    Point* p1_;
    Point* p2_;
    QPointF sourcePoint;
    QPointF destPoint;
    int color;
};

#endif // GEOMETRY_MAIN_H
