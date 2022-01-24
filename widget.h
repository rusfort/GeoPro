#ifndef WIDGET_H
#define WIDGET_H

#include <QObject>
#include <QWidget>
#include <QPointF>
#include <QColor>
#include <vector>
#include <QPaintEvent>
#include <QMouseEvent>

#include "geometry_main.h"

#define EPS 0.0001

class GeoObject;
class GOBJ;

class GeoBoard : public QWidget
{
    Q_OBJECT
public:
    GeoBoard(QWidget *parent = 0, int width = 600, int height = 400, QColor color = Qt::white) : QWidget(parent), mColor(color) { this->resize(width, height); }
    QColor color() const { return mColor; }
    void setColor(QColor color) { mColor = color; }
    void addObject(GOBJ* obj) { mObjects.push_back(obj); }
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
private:
    QColor mColor;
    std::vector<GOBJ*> mObjects;
public:
    GObj_Type trytoadd = GObj_Type::NONE;
    int numitemstoadd = 0;
    Point *lastPoint = 0;
};

#endif // WIDGET_H
