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
    GOBJ(GeoBoard* board, GObj_Type t = GObj_Type::NONE, QColor color = Qt::black);
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
    virtual bool isCatched(QPointF p) = 0;
    virtual void move(QPointF dr) = 0;
signals:
    void selectionChanged();
protected:
    const GObj_Type type;
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
    Point(const Point& copyFrom);
    GeoBoard* board() const { return mBoard; }
    void setBoard(GeoBoard* board) { mBoard = board; }
    double rad() const { return mRadius; }
    void setRad(double rad) { mRadius = rad; }
    void draw() override;
    bool isCatched(QPointF p) override;
    void move(QPointF newPos) override{
        rx() = newPos.x();
        ry() = newPos.y();
        emit posChanged();
    }
signals:
    void posChanged();
private:
    double mRadius;
    qreal X, Y;
    qreal scr_x, scr_y;
};

class Line : public GOBJ
{
    Q_OBJECT
public:
    Line(GeoBoard* board, Point* p1, Point* p2);
    void draw() override;
    bool isCatched(QPointF p) override;
    void move(QPointF newPos) override;
signals:
    void posChanged();
private:
    Point *mP1, *mP2;
    qreal x0, y0, k;      //x0, y0 - some point on a line, k - incline
    qreal scr_x0, scr_y0; //the same, but actual on the screen
};




///OLD VERSION:
/**
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
    qreal line_eq(qreal x, bool &is_vertical); //line equation aka y = kx + b
    qreal anti_line_eq(qreal y); //anti line equation aka x = ky + b

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    GraphWidget *graph;
public:
    Point* p1_;
    Point* p2_;
    QPointF sourcePoint;
    QPointF destPoint;
    QPointF DSP; //draw source point
    QPointF DDP; //draw dest point
    QPointF memPos; //position of the Line
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
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    GraphWidget *graph;
public:
    Point* p1_;
    Point* p2_;
    QPointF sourcePoint;
    QPointF destPoint;
    QPointF memPos; //position of the Segment
    int color;
};*/

#endif // GEOMETRY_MAIN_H
