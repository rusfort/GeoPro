#include "widget.h"
#include <QPainter>
#include <iostream>

void GeoBoard::paintEvent(QPaintEvent*)
{
    QPainter p;
    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(0, 0, this->width(), this->height(), mColor);
    for(auto* obj : mObjects)
        obj->draw();
}

void GeoBoard::mousePressEvent(QMouseEvent* e)
{
    GOBJ* selected_and_caught = 0;
    auto Pos = e->pos();
    bool one_selected = false;
    for(auto obj : mObjects){
        if (obj->isCatched(Pos) && !one_selected){
            obj->setSelected(true);
            if (obj->isSelected()) selected_and_caught = obj;
            one_selected = true;
        } else obj->setSelected(false);
    }

    if(trytoadd == GObj_Type::NONE) return;

    switch (trytoadd) {
    case GObj_Type::POINT:
    {
        Point *p = new Point(this, Pos.x(), Pos.y(), 5);
        addObject(p);
        numitemstoadd--;
        update();
    }
        break;
    case GObj_Type::LINE:
    {
        if (numitemstoadd > 1){
            if (selected_and_caught && selected_and_caught->type_is() == GObj_Type::POINT)
            {
                lastPoint = static_cast<Point*>(selected_and_caught);
            } else {
                lastPoint = new Point(this, Pos.x(), Pos.y(), 5);
                addObject(lastPoint);
            }
        } else {
            Point *p = 0;
            if (selected_and_caught && selected_and_caught->type_is() == GObj_Type::POINT)
            {
                p = static_cast<Point*>(selected_and_caught);
            } else {
                p = new Point(this, Pos.x(), Pos.y(), 5);
                addObject(p);
            }
            Line *l = new Line(this, lastPoint, p);
            addObject(l);
        }
        numitemstoadd--;
        update();
    }
        break;
    case GObj_Type::SEGMENT:
    {
        //
    }
        break;
    default:
        break;
    }
    if (numitemstoadd == 0) trytoadd = GObj_Type::NONE;
}

void GeoBoard::mouseMoveEvent(QMouseEvent* e)
{
    for(auto obj : mObjects)
    {
        if(obj->isSelected())
            obj->move(e->pos());
    }
}

QPointF GeoBoard::getScreenView (const QPointF& math_point){
    return math_point + shift; ///TODO: add scale
}

QPointF GeoBoard::getMathPoint (const QPointF& screen_point){
    return screen_point - shift; ///TODO: add scale
}
