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
#define PI 3.14159265

class GOBJ;
class Point;
class Line;
class Ray;
class Segment;
class Circle;
class Angle;

struct ObjLabel{
    QString label = "";
    int font_size = 15;
    int pos_x = 10;    //relative position
    int pos_y = 10;    //relative position
    int stringHeight = 0;
    int stringWidth = 0;
    int left = 0;
    int top = 0;
    QString font = "Times";
    bool label_grabbed = false;
};

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
    inline Child_Type get_child_type() const{
        return child_type;
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
    void eraseInfoAboutChild(GOBJ* obj);
    QString getLabel() const {return obj_label.label;}
    void changeLabel(const QString& new_label) {obj_label.label = new_label;}
    virtual bool labelCaught(QPoint p);
    void labelSetGrabbed(bool grabbed = true) {obj_label.label_grabbed = grabbed;}
    bool labelGrabbed() {return obj_label.label_grabbed;}
    virtual void moveLabel(QPointF newpos);
    void delObj();
    void checkExistance();
    GeoBoard* board() const { return mBoard; }
    void setBoard(GeoBoard* board) { mBoard = board; }
    virtual void draw() = 0;
    virtual bool isCaught(QPointF p) = 0;
    virtual void move(QPointF dr) = 0;
    virtual void changeView() = 0;
    virtual void recalculate() = 0;
    virtual QString dumpData();
    QString generalDumpData();
    void set_id(int id) {obj_id = id;}
    int id() {return obj_id;}
signals:
    void selectionChanged();
protected:
    const GObj_Type type;
    QColor mColor;
    GeoBoard* mBoard;
    bool mIsSelected;
    ObjLabel obj_label;
    int obj_id = 0; //for dumps & saves
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
qreal distance(qreal x1, qreal y1, qreal x2, qreal y2);
qreal distance(const Point* p1, const Point* p2);
qreal distance(const Point* p, const Line* l);
QPointF getBaseOfPerpendicular(const Point* p, const Line* l);
QPointF getBaseOfPerpendicular(const QPointF* p, const Line* l);
intersect_sol get_inter_solution (const Line* l, const Circle* C);
bool onTheOneSide(QPointF firstP, QPointF secondP, QPointF thisP); //returns 'true' if thisP and secondP lie on the one side of firstP and 'false' otherwise
bool rightOrder4(QPointF p1, QPointF p2, QPointF p3, QPointF p4);
QString getNameOfType(GObj_Type t);
QString getNameOfChildType(Child_Type t);

#endif // GEOMETRY_MAIN_H
