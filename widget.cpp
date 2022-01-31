#include "widget.h"
#include "geometry_main.h"
#include "math.h"
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

void GeoBoard::wheelEvent(QWheelEvent* e){
    auto pos = e->pos();
    auto zoom = pow(1.001, e->delta());
    if (scale * zoom > 5.0 || scale * zoom < 0.1) zoom = 1.0;
    shift = pos * (1 - zoom) + shift * zoom;
    scale *= zoom;
    for(auto obj : mObjects)
    {
        obj->changeView();
    }
    update();
}

void GeoBoard::mousePressEvent(QMouseEvent* e)
{
    GOBJ* selected_and_caught = 0;
    auto Pos = e->pos();
    bool one_caught = false;
    bool misscliked = true;
    num_obj_selected = 0;
    for(auto obj : mObjects){
        if (obj->isCaught(Pos) && !one_caught){
            one_caught = true;
            if (obj->isSelected()){
                if (trytoadd == GObj_Type::NONE) obj->setSelected(false);
                else {
                    selected_and_caught = obj;
                    break;
                }
            } else {
                obj->setSelected(true);
                if (trytoadd != GObj_Type::NONE){
                    selected_and_caught = obj;
                    break;
                }
            }
            misscliked = false;
        }
        if (obj->isSelected()) num_obj_selected++;
    }
    if (misscliked){
        for(auto obj : mObjects)
           obj->setSelected(false);
        num_obj_selected = 0;
    }

    if((trytoadd == GObj_Type::NONE && selected_and_caught == 0) || num_obj_selected > 1){
        board_grabbed = true;
        mouseG = Pos;
        return;
    }

    switch (trytoadd) {
    case GObj_Type::POINT:
    {
        Point *p;
        if (selected_and_caught && selected_and_caught->type_is() == GObj_Type::POINT){
            p = static_cast<Point*>(selected_and_caught);
            p->setSelected(true);
            num_obj_selected++;
        }
        else {
            p = new Point(this, Pos.x(), Pos.y());
            addObject(p);
        }
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
                lastPoint->setSelected(true);
            } else {
                lastPoint = new Point(this, Pos.x(), Pos.y());
                addObject(lastPoint);
            }
        } else {
            Point *p = 0;
            if (selected_and_caught && selected_and_caught->type_is() == GObj_Type::POINT)
            {
                p = static_cast<Point*>(selected_and_caught);
                p->setSelected(true);
                num_obj_selected++;
            } else {
                p = new Point(this, Pos.x(), Pos.y());
                addObject(p);
            }
            Line *l = new Line(this, lastPoint, p);
            addObject(l);
            lastPoint->childObjects[l] = Child_Type::OnTwoPoints;
            p->childObjects[l] = Child_Type::OnTwoPoints;
            l->parentObjects.push_back(lastPoint);
            l->parentObjects.push_back(p);
        }
        numitemstoadd--;
        update();
    }
        break;
    case GObj_Type::SEGMENT:
    {
        if (numitemstoadd > 1){
            if (selected_and_caught && selected_and_caught->type_is() == GObj_Type::POINT)
            {
                lastPoint = static_cast<Point*>(selected_and_caught);
                lastPoint->setSelected(true);
            } else {
                lastPoint = new Point(this, Pos.x(), Pos.y());
                addObject(lastPoint);
            }
        } else {
            Point *p = 0;
            if (selected_and_caught && selected_and_caught->type_is() == GObj_Type::POINT)
            {
                p = static_cast<Point*>(selected_and_caught);
                p->setSelected(true);
                num_obj_selected++;
            } else {
                p = new Point(this, Pos.x(), Pos.y());
                addObject(p);
            }
            Segment *s = new Segment(this, lastPoint, p);
            addObject(s);
            lastPoint->childObjects[s] = Child_Type::OnTwoPoints;
            p->childObjects[s] = Child_Type::OnTwoPoints;
            s->parentObjects.push_back(lastPoint);
            s->parentObjects.push_back(p);
        }
        numitemstoadd--;
        update();
    }
        break;
    case GObj_Type::RAY:
    {
        if (numitemstoadd > 1){
            if (selected_and_caught && selected_and_caught->type_is() == GObj_Type::POINT)
            {
                lastPoint = static_cast<Point*>(selected_and_caught);
                lastPoint->setSelected(true);
            } else {
                lastPoint = new Point(this, Pos.x(), Pos.y());
                addObject(lastPoint);
            }
        } else {
            Point *p = 0;
            if (selected_and_caught && selected_and_caught->type_is() == GObj_Type::POINT)
            {
                p = static_cast<Point*>(selected_and_caught);
                p->setSelected(true);
                num_obj_selected++;
            } else {
                p = new Point(this, Pos.x(), Pos.y());
                addObject(p);
            }
            Ray *l = new Ray(this, lastPoint, p);
            addObject(l);
            lastPoint->childObjects[l] = Child_Type::OnTwoPoints;
            p->childObjects[l] = Child_Type::OnTwoPoints;
            l->parentObjects.push_back(lastPoint);
            l->parentObjects.push_back(p);
        }
        numitemstoadd--;
        update();
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
        if(num_obj_selected > 1) break;
        if(obj->isSelected()){
            obj->move(e->pos());
            board_grabbed = false;
        }
    }
    if (board_grabbed){
        shift += e->pos() - mouseG;
        mouseG = e->pos();
        for(auto obj : mObjects)
        {
            obj->changeView();
        }
        update();
    }
}

void GeoBoard::mouseReleaseEvent(QMouseEvent* e){
    Q_UNUSED(e);
    if (board_grabbed) board_grabbed = false;
}

QPointF GeoBoard::getScreenView (const QPointF& math_point){
    return math_point * scale + shift;
}

QPointF GeoBoard::getMathPoint (const QPointF& screen_point){
    return (screen_point - shift) / scale;
}

void GeoBoard::selectAll() {
    for(auto obj : mObjects)
        obj->setSelected(true);
}

void GeoBoard::unselectAll() {
    for(auto obj : mObjects)
        obj->setSelected(false);
}

void GeoBoard::delObject(GOBJ* obj){
    auto to_del = std::find(mObjects.begin(), mObjects.end(), obj);
    if (to_del != mObjects.end()){
        mObjects.erase(to_del);
        delete obj;
    }
}
