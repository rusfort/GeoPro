#ifndef WIDGET_H
#define WIDGET_H

#include <QObject>
#include <QWidget>
#include <QPointF>
#include <QColor>
#include <vector>
#include <QPaintEvent>
#include <QMouseEvent>

#include "service.h"

class GOBJ;
class Point;

class GeoBoard : public QWidget
{
    Q_OBJECT
public:
    GeoBoard(QWidget *parent = 0, int width = 600, int height = 400, QColor color = Qt::white) : QWidget(parent), mColor(color){ this->resize(width, height); }
    QColor color() const { return mColor; }
    void setColor(QColor color) { mColor = color; }
    void selectAll();
    void unselectAll();
    void addObject(GOBJ* obj) { mObjects.push_back(obj); }
    void delObject(GOBJ* obj);
    QPointF getScreenView (const QPointF& math_point);
    QPointF getMathPoint (const QPointF& screen_point);
    std::vector<GOBJ*>& getAllObj(){
        return mObjects;
    }
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;
private:
    QColor mColor;
    std::vector<GOBJ*> mObjects;
    bool board_grabbed = false;
public:
    GObj_Type trytoadd = GObj_Type::NONE;
    int numitemstoadd = 0;
    Point *lastPoint = 0;
    qreal scale = 1;   //for screen move & scale view
    QPointF mouseG;    //for screen move & scale view
    QPointF shift;     //for screen move & scale view
    int num_obj_selected = 0;
    QPoint lastMousePos;//for point drawing (when adding)
};

#endif // WIDGET_H
