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
    std::vector<GOBJ*> selected;
    auto Pos = e->pos();
    for(auto obj : mObjects){
        obj->setSelected(obj->isCatched(Pos));
        if (obj->isSelected() && obj->isCatched(Pos)) selected.push_back(obj);
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
            lastPoint = new Point(this, Pos.x(), Pos.y(), 5);
            addObject(lastPoint);
        } else {
            Point *p = new Point(this, Pos.x(), Pos.y(), 5);
            addObject(p);
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
