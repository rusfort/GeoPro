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

class GeoBoard : public QWidget
{
    Q_OBJECT
public:
    //GeoBoard(QWidget *parent = 0);
    GeoBoard(QWidget *parent = 0, int width = 600, int height = 400, QColor color = Qt::white) : QWidget(parent), mColor(color) { this->resize(width, height); }
    QColor color() const { return mColor; }
    void setColor(QColor color) { mColor = color; }
    void addObject(GeoObject* obj) { mObjects.push_back(obj); }
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
private:
    QColor mColor;
    std::vector<GeoObject*> mObjects;
public:
    GObj_Type trytoadd = GObj_Type::NONE;
    int numitemstoadd = 0;
    ///Point *lastPoint = 0;
};

class GeoObject : public QObject
{
    Q_OBJECT
public:
    GeoObject(GeoBoard* board, QColor color = Qt::black);
    virtual ~GeoObject() { }
    GeoBoard* board() const { return mBoard; }
    void setBoard(GeoBoard* board) { mBoard = board; }
    QColor color() const { return mColor; }
    void setColor(QColor color) { mColor = color; }
    bool isSelected() const { return mIsSelected; }
    void setSelected(bool mode = true) { if(mIsSelected != mode) { mIsSelected = mode; emit selectionChanged(); } }
    virtual void draw() = 0;
    virtual bool isCatched(QPointF p) = 0;
    virtual void move(QPointF dr) = 0;
signals:
    void selectionChanged();
protected:
    QColor mColor;
    GeoBoard* mBoard;
    bool mIsSelected;
};


class GeoPoint : public GeoObject, public QPointF
{
    Q_OBJECT
public:
    GeoPoint(GeoBoard* board, double x = 0.0, double y = 0.0, double radius = 1.0,  QColor color = Qt::black);
    GeoPoint(const GeoPoint& copyFrom);
    double rad() const { return mRadius; }
    void setRad(double rad) { mRadius = rad; }
    void draw() override;
    bool isCatched(QPointF p) override;
    void move(QPointF newPos) override { rx() = newPos.x(); ry() = newPos.y(); emit posChanged(); }
signals:
    void posChanged();
private:
    double mRadius;
};

class GeoLine : public GeoObject
{
    Q_OBJECT
public:
    GeoLine(GeoBoard* board, GeoPoint* p1, GeoPoint* p2);
    void draw() override;
    bool isCatched(QPointF p) override;
    void move(QPointF newPos) override;
signals:
    void posChanged();
private:
    GeoPoint *mP1, *mP2;
};

#endif // WIDGET_H
