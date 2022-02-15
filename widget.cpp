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
    if(active_grid) drawGrid(&p);
    if (numitemstoadd > 0){
        p.setBrush(QBrush(Qt::black));
        p.drawEllipse(QPointF(lastMousePos.x(), lastMousePos.y()), 5, 5);
    }

    //high priority for points
    bool caught_point = false;
    for(auto* obj : mObjects){
        if (obj->isCaught(lastMousePos) && obj->type_is() == GObj_Type::POINT){
            caught_point = true;
            break;
        }
    }
    //then check & draw others
    for(auto* obj : mObjects){
        bool was_caught = false;
        if (numitemstoadd > 0 && obj->isCaught(lastMousePos)){
            if (!caught_point || obj->type_is() == GObj_Type::POINT){
                obj->setSelected();
                was_caught = true;
            }
        }
        obj->draw();   
        if (was_caught) obj->setSelected(false);
    }
}

void GeoBoard::drawGrid(QPainter* p){
    const qreal grid_step = 50; //grid step in math units
    qreal begin_x = 0;
    qreal begin_y = 0;
    int start_x = getScreenView(QPointF(0, 0)).x();
    int start_y = getScreenView(QPointF(0, 0)).y();
    qreal end_x = this->width();
    qreal end_y = this->height();
    for (int i = 0;; ++i){
        p->setPen(Qt::lightGray);
        int left =  start_x - i * grid_step * scale;
        int right =  start_x + (i + 1) * grid_step * scale;
        if (right < end_x) p->drawLine(right, 0, right, this->height());
        if (left > begin_x) p->drawLine(left, 0, left, this->height());
        if (right >= end_x && left <= begin_x) break;
    }
    for (int i = 0;; ++i){
        p->setPen(Qt::lightGray);
        int up =  start_y - i * grid_step * scale;
        int down =  start_y + (i + 1) * grid_step * scale;
        if (down < end_y) p->drawLine(0, down, this->width(), down);
        if (up > begin_y) p->drawLine(0, up, this->width(), up);
        if (down >= end_y && up <= begin_y) break;
    }
}

void GeoBoard::wheelEvent(QWheelEvent* e){
    auto pos = e->pos();
    auto zoom = pow(1.001, e->delta());
    if (scale * zoom > 5.0 || scale * zoom < 0.2) zoom = 1.0;
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

    //high priority for points
    for(auto obj : mObjects){
        if (obj->isCaught(Pos) && !one_caught && obj->type_is() == GObj_Type::POINT){
            one_caught = true;
            threePoints.push_back(static_cast<Point*>(obj));
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
    //WARNING: CODE DUPLICATING! FIX: mObject has to be a multimap sorted by the obj type
    if(!one_caught){
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
    }
    if (misscliked){
        for(auto obj : mObjects)
           obj->setSelected(false);
        num_obj_selected = 0;
        threePoints.clear();
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
            if (selected_and_caught) p->setFixOnFigure(selected_and_caught);
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
                if (selected_and_caught) lastPoint->setFixOnFigure(selected_and_caught);
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
                if (selected_and_caught) p->setFixOnFigure(selected_and_caught);
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
                if (selected_and_caught) lastPoint->setFixOnFigure(selected_and_caught);
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
                if (selected_and_caught) p->setFixOnFigure(selected_and_caught);
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
                if (selected_and_caught) lastPoint->setFixOnFigure(selected_and_caught);
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
                if (selected_and_caught) p->setFixOnFigure(selected_and_caught);
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
    case GObj_Type::CIRCLE:
    {
        if (numitemstoadd > 1){
            if (selected_and_caught && selected_and_caught->type_is() == GObj_Type::POINT)
            {
                lastPoint = static_cast<Point*>(selected_and_caught);
                lastPoint->setSelected(true);
            } else {
                lastPoint = new Point(this, Pos.x(), Pos.y());
                if (selected_and_caught) lastPoint->setFixOnFigure(selected_and_caught);
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
                if (selected_and_caught) p->setFixOnFigure(selected_and_caught);
                addObject(p);
            }
            Circle* C = new Circle(this, lastPoint, sqrt((lastPoint->scr_x - p->scr_x) * (lastPoint->scr_x - p->scr_x) +
                                                      (lastPoint->scr_y - p->scr_y) * (lastPoint->scr_y - p->scr_y)) / scale);
            if (C->r() * scale < EPS) C->exists = false;
            else C->exists = true;
            C->depending = true;
            C->child_type = Child_Type::OnTwoPoints;
            addObject(C);
            lastPoint->childObjects[C] = Child_Type::OnTwoPoints;
            p->childObjects[C] = Child_Type::OnTwoPoints;
            C->parentObjects.push_back(lastPoint);
            C->parentObjects.push_back(p);
            C->basePoints.push_back(lastPoint);
            C->basePoints.push_back(p);
            C->recalculate();
        }
        numitemstoadd--;
        update();
    }
        break;
    default:
        break;
    }
    if (numitemstoadd == 0){
        trytoadd = GObj_Type::NONE;
        setMouseTracking(false);
    }
}

void GeoBoard::mouseMoveEvent(QMouseEvent* e)
{
    if (numitemstoadd > 0){
        lastMousePos = e->pos();
        update();
        return;
    }

    for(auto obj : mObjects)
    {
        if(num_obj_selected > 1) break;
        if(obj->depending) obj->recalculate();
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
    num_obj_selected = 0;
}

void GeoBoard::delObject(GOBJ* obj){
    auto to_del = std::find(mObjects.begin(), mObjects.end(), obj);
    if (to_del != mObjects.end()){
        mObjects.erase(to_del);
        delete obj;
    }
}
