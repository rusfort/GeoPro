#include "widget.h"
#include "geometry_main.h"
#include "math.h"
#include <QPainter>
#include <iostream>
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>

#include "line.h"
#include "point.h"
#include "ray.h"
#include "segment.h"
#include "circle.h"
#include "angle.h"
#include "triangle.h"

#include "obj_menu.h"

void GeoBoard::paintEvent(QPaintEvent*)
{
    QPainter p;
    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(0, 0, this->width(), this->height(), mColor);
    if(active_grid) drawGrid(&p);
    if (trytoadd != GObj_Type::NONE){
        int offset = 20;
        if (trytoadd == GObj_Type::POINT)   p.drawText(QPointF(offset, offset), "Adding point");
        if (trytoadd == GObj_Type::SEGMENT) p.drawText(QPointF(offset, offset), "Adding segment");
        if (trytoadd == GObj_Type::RAY)     p.drawText(QPointF(offset, offset), "Adding ray");
        if (trytoadd == GObj_Type::LINE)    p.drawText(QPointF(offset, offset), "Adding line");
        if (trytoadd == GObj_Type::CIRCLE)  p.drawText(QPointF(offset, offset), "Adding circle");
        if (trytoadd == GObj_Type::TRIANGLE)  p.drawText(QPointF(offset, offset), "Adding triangle");
    }
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

    saveToCache();
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

void GeoBoard::addObject(GOBJ* obj) {
    mObjects.push_back(obj);
    inc_id();
    obj->set_id(get_cur_id());
    if (!obj->childObjects.empty()){
        for (auto ob : obj->childObjects){
            addObject(ob.first);
        }
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
    GOBJ* selected4menu = 0; //selected for opening the menu
    auto Pos = e->pos();
    bool one_caught = false;
    bool misscliked = true;
    num_obj_selected = 0;

    //high priority for points
    for(auto obj : mObjects){
        if (obj->isCaught(Pos) && !one_caught && obj->type_is() == GObj_Type::POINT && obj->is_visible()){
            one_caught = true;
            if (!selected4menu) selected4menu = obj;
            threeOrderedPoints.push_back(static_cast<Point*>(obj));
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

    if(!one_caught){
        num_obj_selected = 0;
        for(auto it = mObjects.rbegin(); it != mObjects.rend(); ++it){
            auto obj = *it;
            if (obj->isCaught(Pos) && !one_caught && obj->is_visible()){
                one_caught = true;
                if (!selected4menu) selected4menu = obj;
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
    if (misscliked) unselectAll();

    if(e->button() == Qt::RightButton){ //Object menu opening
        threeOrderedPoints.clear();
        if (selected4menu){
            Obj_menu *menu = new Obj_menu(parentWidget(), selected4menu);
            menu->setAttribute(Qt::WA_DeleteOnClose);
            menu->exec();
        }
        unselectAll();
        update();
        return;
    }

    if(numitemstoadd == 0 && misscliked){ //label movements
        for(auto obj : mObjects){
            if (obj->labelCaught(Pos)){
                obj->labelSetGrabbed();
                break;
            } else obj->labelSetGrabbed(false);
        }
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
            connect_objects(lastPoint, l, Child_Type::OnTwoPoints);
            connect_objects(p, l, Child_Type::OnTwoPoints);
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
            connect_objects(lastPoint, s, Child_Type::OnTwoPoints);
            connect_objects(p, s, Child_Type::OnTwoPoints);
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
            Ray *r = new Ray(this, lastPoint, p);
            addObject(r);
            connect_objects(lastPoint, r, Child_Type::OnTwoPoints);
            connect_objects(p, r, Child_Type::OnTwoPoints);
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
            connect_objects(lastPoint, C, Child_Type::OnTwoPoints);
            connect_objects(p, C, Child_Type::OnTwoPoints);
            addObject(C);
            C->basePoints.push_back(lastPoint);
            C->basePoints.push_back(p);
            C->recalculate();
        }
        numitemstoadd--;
        update();
    }
        break;
    case GObj_Type::TRIANGLE:{
        if (numitemstoadd > 2){
            if (selected_and_caught && selected_and_caught->type_is() == GObj_Type::POINT)
            {
                lastPoint2 = static_cast<Point*>(selected_and_caught);
                lastPoint2->setSelected(true);
            } else {
                lastPoint2 = new Point(this, Pos.x(), Pos.y());
                if (selected_and_caught) lastPoint2->setFixOnFigure(selected_and_caught);
                addObject(lastPoint2);
            }
        } else if (numitemstoadd > 1){
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
            if (onOneLine(lastPoint2, lastPoint, p)){
                QMessageBox::critical(this, "TRIANGLE ERROR", "Cannot build a triangle! 3 points on the same line!");
                numitemstoadd = 0;
                update();
                break;
            }
            Triangle* T = new Triangle(this, lastPoint2, lastPoint, p);
            T->exists = true;
            connect_objects(lastPoint2, T, Child_Type::Triangle);
            connect_objects(lastPoint, T, Child_Type::Triangle);
            connect_objects(p, T, Child_Type::Triangle);
            addObject(T);
            T->recalculate();
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
    unselectAll();
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
        if(obj->labelGrabbed()){
            obj->moveLabel(e->pos());
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
    update();
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
    threeOrderedPoints.clear();
}

void GeoBoard::delObject(GOBJ* obj){
    auto to_del = std::find(mObjects.begin(), mObjects.end(), obj);
    if (to_del != mObjects.end()){
        mObjects.erase(to_del);
        delete obj;
    }
}

void GeoBoard::connect_objects(GOBJ* parent_obj, GOBJ* child_obj, Child_Type type){
    child_obj->depending = true;
    child_obj->child_type = type;
    parent_obj->childObjects[child_obj] = type;
    child_obj->parentObjects.push_back(parent_obj);
}

bool GeoBoard::onOneLine(const Point* p1, const Point* p2, const Point* p3){
    qreal a1 = p1->X - p2->X;
    qreal b1 = p1->Y - p2->Y;
    qreal a2 = p1->X - p3->X;
    qreal b2 = p1->Y - p3->Y;
    qreal d = b2 * a1 - b1 * a2;

    if (std::abs(d) < EPS || (std::abs(a1) < EPS && std::abs(a2) < EPS)) return true;
    return false;
}

void GeoBoard::saveToCache(){
    QFile cache(".//cache.gprc");

    if(!cache.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::critical(this, "Cache error", "Cannot write to cache");
        return;
    }

    QTextStream stream(&cache);

    cacheStream(stream);

    cache.close();
}

void GeoBoard::cacheStream(QTextStream& stream){
    stream << scale << "\n";
    stream << shift.x() << " " << shift.y() << "\n";
    stream << mObjects.size() << "\n";
    for (auto obj : mObjects){
        stream << obj->generalDumpData() << " " << obj->dumpData() << "\n";
    }
}

void GeoBoard::saveStream(QTextStream& stream){
    stream << "GEOPRO SAVE DATA\n0.0.4\n";
    QDateTime dt = QDateTime::currentDateTime();
    stream << dt.toString("dd.MM.yyyy hh:mm") << "\n";
    stream << "3\n";
    stream << "#####\n#####\n#####\n";

    cacheStream(stream);

    stream << "3\n";
    stream << "#####\n#####\n#####\n";
    stream << "END GEOPRO DATA";
}
