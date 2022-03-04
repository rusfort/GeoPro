#ifndef WIDGET_H
#define WIDGET_H

#include <QObject>
#include <QWidget>
#include <QPointF>
#include <QColor>
#include <vector>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextStream>

#include "service.h"

class GOBJ;
class Point;
class Obj_menu;

class GeoBoard : public QWidget
{
    Q_OBJECT
public:
    GeoBoard(QWidget *parent = 0, int width = 600, int height = 400, QColor color = Qt::white) : QWidget(parent), mColor(color){ this->resize(width, height); }
    QColor color() const { return mColor; }
    void setColor(QColor color) { mColor = color; }
    void setGrid(bool active = true) {active_grid = active;}
    void selectAll();
    void unselectAll();
    void addObject(GOBJ* obj);
    void delObject(GOBJ* obj);
    QPointF getScreenView (const QPointF& math_point);
    QPointF getMathPoint (const QPointF& screen_point);
    std::vector<GOBJ*>& getAllObj(){
        return mObjects;
    }
    std::vector<Point*>& getThreeOrderedPoints(){
        return threeOrderedPoints;
    }
    void clear_threeOrderedPoints(){
        threeOrderedPoints.clear();
    }
    void drawGrid(QPainter* p);
    void connect_objects(GOBJ* parent_obj, GOBJ* child_obj, Child_Type type); //connecting two objects using parentObjects and childObjects
    bool onOneLine(const Point* p1, const Point* p2, const Point* p3);
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;
    void saveToCache();
    void cacheStream(QTextStream& stream);
    void saveStream(QTextStream& stream);
    void inc_id() {++current_id;}
    int get_cur_id() {return current_id;}
private:
    QColor mColor;
    bool active_grid = false;
    std::vector<GOBJ*> mObjects;
    int current_id = 0;
    std::vector<Point*> threeOrderedPoints; //for bisector (order is important)
    bool board_grabbed = false;
public:
    QLineEdit *AddLabel;
    QPushButton *LabelButton;
    GObj_Type trytoadd = GObj_Type::NONE;
    int numitemstoadd = 0;
    Point *lastPoint = 0;
    Point *lastPoint2 = 0;
    qreal scale = 1;   //for screen move & scale view
    QPointF mouseG;    //for screen move & scale view
    QPointF shift;     //for screen move & scale view
    int num_obj_selected = 0;
    QPoint lastMousePos;//for point drawing (when adding)
};

#endif // WIDGET_H
