#include "geopro.h"
#include "ui_geopro.h"
#include "widget.h"
#include "geometry_main.h"

#include <iostream>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QtGui>
#include <QGraphicsView>
#include <QMessageBox>
#include <QShortcut>
#include <QCloseEvent>


GeoPro::GeoPro(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GeoPro)
{
    ui->setupUi(this);
    setWindowTitle(tr("GeoPro 0.0.1 [alpha] by Nikolay Kozakov"));

    b = new GeoBoard(this);
    setCentralWidget(b);
}

GeoPro::~GeoPro()
{
    delete ui;
}

void GeoPro::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "EXIT", "Are you sure you want to exit?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

void GeoPro::on_actionClose_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "EXIT", "Are you sure you want to exit?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
        QApplication::quit();
}

void GeoPro::on_actionPoint_triggered()
{
    if(b->numitemstoadd > 0) return;
    b->unselectAll();
    b->trytoadd = GObj_Type::POINT;
    b->numitemstoadd = 1;
    b->setMouseTracking(true);
}

void GeoPro::on_actionLine_triggered()
{
    if(b->numitemstoadd > 0) return;
    b->unselectAll();
    b->trytoadd = GObj_Type::LINE;
    b->numitemstoadd = 2;
    b->setMouseTracking(true);
}

void GeoPro::on_actionSegment_triggered()
{
    if(b->numitemstoadd > 0) return;
    b->unselectAll();
    b->trytoadd = GObj_Type::SEGMENT;
    b->numitemstoadd = 2;
    b->setMouseTracking(true);
}

void GeoPro::on_actionDelete_selected_objects_triggered()
{
    b->update();
    while (b->num_obj_selected > 0){
        for (auto& obj : b->getAllObj()){
            if (obj->isSelected()){
                obj->delObj();
                break;
            }
        }
        b->update();
    }
}

void GeoPro::on_actionClear_all_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "DELETE ALL", "Are you sure you want to clear the board?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes){
        b->getAllObj().clear();
        b->update();
    }
}

void GeoPro::on_actionRay_triggered()
{
    if(b->numitemstoadd > 0) return;
    b->unselectAll();
    b->trytoadd = GObj_Type::RAY;
    b->numitemstoadd = 2;
    b->setMouseTracking(true);
}

void GeoPro::on_actionIntersection_triggered()
{
    if(b->numitemstoadd > 0) return;
    if(b->num_obj_selected > 2 || (b->num_obj_selected < 2 && b->num_obj_selected > 0)){
        QMessageBox::critical(b, "INTERSECTION ERROR", "Can intersect only 2 objects!");
        b->unselectAll();
        b->update();
        return;
    }
    if(b->num_obj_selected == 0){
        QMessageBox::warning(b, "INTERSECTION WARNING", "Select 2 objects to intersect!");
        return;
    }
    if(b->num_obj_selected == 2){
        Point* intersection = new Point(b);
        Point* intersection2 = new Point(b);
        bool inter_circle = false; //if the second intersection is needed
        GOBJ* Obj1 = 0;
        GOBJ* Obj2 = 0;
        for (auto& obj : b->getAllObj()){
            if (obj->isSelected()){
                intersection->parents_intersected.insert(std::make_pair(obj->type_is(), obj));
                if (!Obj1) Obj1 = obj;
                else {
                    Obj2 = obj;
                    break;
                }
            }
        }
        if (Obj1 == 0 || Obj2 == 0){
            delete intersection;
            delete intersection2;
            QMessageBox::critical(b, "FATAL LOGIC ERROR", "Null object intersection!");
            QApplication::quit();
            return;
        }
        if (Obj1->type_is() == GObj_Type::POINT || Obj2->type_is() == GObj_Type::POINT){
            delete intersection;
            delete intersection2;
            b->unselectAll();
            b->update();
            QMessageBox::critical(b, "INTERSECTION ERROR", "Cannot intersect with a point!");
            return;
        }
        if (Obj1->type_is() == GObj_Type::CIRCLE || Obj2->type_is() == GObj_Type::CIRCLE) inter_circle = true;
        intersection->exists = false;
        intersection->depending = true;
        intersection->child_type = Child_Type::Intersection;
        b->addObject(intersection);
        Obj1->childObjects[intersection] = Child_Type::Intersection;
        Obj2->childObjects[intersection] = Child_Type::Intersection;
        intersection->parentObjects.push_back(Obj1);
        intersection->parentObjects.push_back(Obj2);
        intersection->setIntersectionType();
        intersection->recalculate();
        if (inter_circle){
            auto it2 = intersection->parents_intersected.begin();
            auto it1 = it2;
            ++it2;
            intersection2->parents_intersected.insert(*it1);
            intersection2->parents_intersected.insert(*it2);
            intersection2->exists = false;
            intersection2->depending = true;
            intersection2->child_type = Child_Type::Intersection2;
            b->addObject(intersection2);
            Obj1->childObjects[intersection2] = Child_Type::Intersection2;
            Obj2->childObjects[intersection2] = Child_Type::Intersection2;
            intersection2->parentObjects.push_back(Obj1);
            intersection2->parentObjects.push_back(Obj2);
            intersection2->setIntersectionType();
            intersection2->recalculate();
        }
        if (!inter_circle) delete intersection2;
        b->unselectAll();
        b->update();
        return;
    }
}

void GeoPro::on_actionCircle_by_the_center_radius_triggered()
{
    if(b->numitemstoadd > 0) return;
    b->unselectAll();
    b->trytoadd = GObj_Type::CIRCLE;
    b->numitemstoadd = 2;
    b->setMouseTracking(true);
}

void GeoPro::on_actionCircle_by_3_points_triggered()
{
    if(b->numitemstoadd > 0) return;
    if(b->num_obj_selected > 3 || (b->num_obj_selected < 3 && b->num_obj_selected > 0)){
        QMessageBox::critical(b, "CIRCLE ERROR", "Need 3 points!");
        b->unselectAll();
        b->update();
        return;
    }
    if(b->num_obj_selected == 0){
        QMessageBox::warning(b, "CIRCLE WARNING", "Select 3 points!");
        return;
    }
    if(b->num_obj_selected == 3){
        GOBJ* p1 = 0;
        GOBJ* p2 = 0;
        GOBJ* p3 = 0;
        for (auto& obj : b->getAllObj()){
            if (obj->isSelected()){
                if (!p1) p1 = obj;
                else {
                    if (!p2) p2 = obj;
                    else{
                        p3 = obj;
                        break;
                    }
                }
            }
        }
        if (p1->type_is() != GObj_Type::POINT || p2->type_is() != GObj_Type::POINT || p3->type_is() != GObj_Type::POINT){
            b->unselectAll();
            b->update();
            QMessageBox::critical(b, "CIRCLE ERROR", "Cannot build a circle! Need 3 points.");
            return;
        }
        auto params = getCircleCenterAndRadius(static_cast<Point*>(p1), static_cast<Point*>(p2), static_cast<Point*>(p3));
        Point* cen = new Point(b, params.first.x(), params.first.y());
        Circle* C = new Circle(b, cen, params.second);
        if (C->r() < EPS) C->exists = false;
        else C->exists = true;
        C->depending = true;
        cen->depending = true;
        C->child_type = Child_Type::Middle;
        cen->child_type = Child_Type::OnThreePoints;
        b->addObject(C);
        b->addObject(cen);
        p1->childObjects[cen] = Child_Type::OnThreePoints;
        p2->childObjects[cen] = Child_Type::OnThreePoints;
        p3->childObjects[cen] = Child_Type::OnThreePoints;
        cen->parentObjects.push_back(p1);
        cen->parentObjects.push_back(p2);
        cen->parentObjects.push_back(p3);
        cen->childObjects[C] = Child_Type::Middle;
        C->parentObjects.push_back(cen);
        C->basePoints.push_back(static_cast<Point*>(p1));
        C->basePoints.push_back(static_cast<Point*>(p2));
        C->basePoints.push_back(static_cast<Point*>(p3));
        C->recalculate();
        b->unselectAll();
        b->update();
        return;
    }
}

void GeoPro::on_actionMiddle_Center_triggered()
{
    if(b->numitemstoadd > 0) return;
    if(b->num_obj_selected > 2 || (b->num_obj_selected < 2 && b->num_obj_selected > 0)){
        QMessageBox::critical(b, "MIDDLE ERROR", "Can find a middle for only 2 points!");
        b->unselectAll();
        b->update();
        return;
    }
    if(b->num_obj_selected == 0){
        QMessageBox::warning(b, "MIDDLE WARNING", "Select 2 points!");
        return;
    }
    if(b->num_obj_selected == 2){
        Point* middle = new Point(b);
        GOBJ* p1 = 0;
        GOBJ* p2 = 0;

        for (auto& obj : b->getAllObj()){
            if (obj->isSelected()){
                if (!p1) p1 = obj;
                else {
                    if (!p2){
                        p2 = obj;
                        break;
                    }
                }
            }
        }

        if (p1->type_is() != GObj_Type::POINT || p2->type_is() != GObj_Type::POINT){
            delete middle;
            b->unselectAll();
            b->update();
            QMessageBox::critical(b, "MIDDLE ERROR", "Cannot build a middle! Need 2 points.");
            return;
        }

        middle->exists = true;
        middle->depending = true;
        middle->child_type = Child_Type::Middle;
        b->addObject(middle);
        p1->childObjects[middle] = Child_Type::Middle;
        p2->childObjects[middle] = Child_Type::Middle;
        middle->parentObjects.push_back(p1);
        middle->parentObjects.push_back(p2);
        middle->recalculate();
        b->unselectAll();
        b->update();
        return;
    }
}

void GeoPro::on_actionAbout_triggered()
{
    QMessageBox::information(b, "About GeoPro", "GeoPro is a professional geometry toolkit for teachers & students made by Nick Rusfort (Nikolay Kozakov). \nYou can find more detailed information in the README file in Github: https://github.com/rusfort/GeoPro");
}

void GeoPro::on_actionContact_author_triggered()
{
    QMessageBox::information(b, "Contact author", "Nick Rusfort (Nikolay Kozakov) \nContacts: \ne-mail: rusfort-fax@yandex.ru \nVK: vk.com/rusfort");
}

void GeoPro::on_actionHide_selected_objects_triggered()
{
    if (b->num_obj_selected > 0){
        for (auto& obj : b->getAllObj()){
            if (obj->isSelected()){
                obj->hide();
            }
        }
        b->unselectAll();
        b->update();
    }
}


void GeoPro::on_actionShow_all_hidden_objects_triggered()
{
    for (auto& obj : b->getAllObj()){
        if (!obj->is_visible()){
            obj->make_visible();
            obj->setSelected();
        }
    }
    b->update();
}


void GeoPro::on_actionParallel_line_triggered()
{
    if(b->numitemstoadd > 0) return;
    if(b->num_obj_selected > 2 || b->num_obj_selected == 1){
        QMessageBox::critical(b, "LINE ERROR", "Need a line/ray/segment and a point!");
        b->unselectAll();
        b->update();
        return;
    }
    if(b->num_obj_selected == 0){
        QMessageBox::warning(b, "LINE WARNING", "Select a line/ray/segment and a point!");
        return;
    }
    if(b->num_obj_selected == 2){
        GOBJ* p1 = 0;
        GOBJ* p2 = 0;
        Line* line = 0;
        for (auto& obj : b->getAllObj()){
            if (obj->isSelected()){
                if (!p1) p1 = obj;
                else {
                    if (!p2){
                        p2 = obj;
                        break;
                    }
                }
            }
        }

        if (p1->type_is() != GObj_Type::POINT){ //swap if the order is not ok
            if (p2->type_is() != GObj_Type::POINT){
                b->unselectAll();
                b->update();
                QMessageBox::critical(b, "LINE ERROR", "Cannot build a parallel line! Need a point.");
                return;
            } else {
                GOBJ* tmp = p1;
                p1 = p2;
                p2 = tmp;
            }
        }
        if (p2->type_is() != GObj_Type::LINE && p2->type_is() != GObj_Type::RAY && p2->type_is() != GObj_Type::SEGMENT){
            b->unselectAll();
            b->update();
            QMessageBox::critical(b, "LINE ERROR", "Cannot build a parallel line! Need a line/ray/segment with a point.");
            return;
        }
        if (p2->type_is() != GObj_Type::LINE){ //making lines (if not a line)
            if (p2->type_is() == GObj_Type::RAY) line = new Line(b, static_cast<Ray*>(p2));
            if (p2->type_is() == GObj_Type::SEGMENT) line = new Line(b, static_cast<Segment*>(p2));
        } else line = static_cast<Line*>(p2);

        auto point1 = static_cast<Point*>(p1);
        auto parallel = new Line(b, line->k(), -1, (point1->Y - line->k() * point1->X));
        parallel->setBaseLine(p2);
        parallel->setBasePoint(point1);

        parallel->exists = true;
        parallel->depending = true;
        parallel->child_type = Child_Type::Parallel;
        b->addObject(parallel);
        p1->childObjects[parallel] = Child_Type::Parallel;
        p2->childObjects[parallel] = Child_Type::Parallel;
        parallel->parentObjects.push_back(p1);
        parallel->parentObjects.push_back(p2);
        parallel->recalculate();

        b->unselectAll();
        b->update();

        if (p2->type_is() != GObj_Type::LINE) delete line;
        return;
    }
}


void GeoPro::on_actionPerpendicular_line_triggered()
{
    if(b->numitemstoadd > 0) return;
    if(b->num_obj_selected > 2 || b->num_obj_selected == 1){
        QMessageBox::critical(b, "LINE ERROR", "Need a line/ray/segment and a point!");
        b->unselectAll();
        b->update();
        return;
    }
    if(b->num_obj_selected == 0){
        QMessageBox::warning(b, "LINE WARNING", "Select a line/ray/segment and a point!");
        return;
    }
    if(b->num_obj_selected == 2){
        GOBJ* p1 = 0;
        GOBJ* p2 = 0;
        Line* line = 0;
        for (auto& obj : b->getAllObj()){
            if (obj->isSelected()){
                if (!p1) p1 = obj;
                else {
                    if (!p2){
                        p2 = obj;
                        break;
                    }
                }
            }
        }

        if (p1->type_is() != GObj_Type::POINT){ //swap if the order is not ok
            if (p2->type_is() != GObj_Type::POINT){
                b->unselectAll();
                b->update();
                QMessageBox::critical(b, "LINE ERROR", "Cannot build a perpendicular line! Need a point.");
                return;
            } else {
                GOBJ* tmp = p1;
                p1 = p2;
                p2 = tmp;
            }
        }
        if (p2->type_is() != GObj_Type::LINE && p2->type_is() != GObj_Type::RAY && p2->type_is() != GObj_Type::SEGMENT){
            b->unselectAll();
            b->update();
            QMessageBox::critical(b, "LINE ERROR", "Cannot build a perpendicular line! Need a line/ray/segment with a point.");
            return;
        }
        if (p2->type_is() != GObj_Type::LINE){ //making lines (if not a line)
            if (p2->type_is() == GObj_Type::RAY) line = new Line(b, static_cast<Ray*>(p2));
            if (p2->type_is() == GObj_Type::SEGMENT) line = new Line(b, static_cast<Segment*>(p2));
        } else line = static_cast<Line*>(p2);

        auto point1 = static_cast<Point*>(p1);
        auto tmp_k = std::abs(line->k()) < EPS ? 1 : -1/line->k();
        auto parallel = new Line(b, tmp_k, -1, (point1->Y - line->k() * point1->X));
        parallel->setBaseLine(p2);
        parallel->setBasePoint(point1);

        parallel->exists = true;
        parallel->depending = true;
        parallel->child_type = Child_Type::Perpendicular;
        b->addObject(parallel);
        p1->childObjects[parallel] = Child_Type::Perpendicular;
        p2->childObjects[parallel] = Child_Type::Perpendicular;
        parallel->parentObjects.push_back(p1);
        parallel->parentObjects.push_back(p2);
        parallel->recalculate();

        b->unselectAll();
        b->update();

        if (p2->type_is() != GObj_Type::LINE) delete line;
        return;
    }
}


void GeoPro::on_actionBisector_triggered()
{
    if(b->numitemstoadd > 0) return;
    if(b->num_obj_selected > 3 || (b->num_obj_selected < 3 && b->num_obj_selected > 0)){
        QMessageBox::critical(b, "BISECTOR ERROR", "Need 3 points!");
        b->unselectAll();
        b->update();
        return;
    }
    if(b->num_obj_selected == 0){
        QMessageBox::warning(b, "BISECTOR WARNING", "Select 3 points!");
        return;
    }
    if(b->num_obj_selected == 3){
        GOBJ* p1 = 0;
        GOBJ* p2 = 0;
        GOBJ* p3 = 0;
        for (auto& obj : b->getAllObj()){
            if (obj->isSelected()){
                if (!p1) p1 = obj;
                else {
                    if (!p2) p2 = obj;
                    else{
                        p3 = obj;
                        break;
                    }
                }
            }
        }
        //check that only points are selected
        if (p1->type_is() != GObj_Type::POINT || p2->type_is() != GObj_Type::POINT || p3->type_is() != GObj_Type::POINT){
            b->unselectAll();
            b->update();
            QMessageBox::critical(b, "BISECTOR ERROR", "Cannot build a bisector! Need 3 points.");
            return;
        }

        //forget the previous p1, p2, p3 (because order was random)
        p1 = b->getThreePoints()[0];
        p2 = b->getThreePoints()[1];
        p3 = b->getThreePoints()[2];

        b->clear_threePoints();

        QPointF res1 = QPointF(static_cast<Point*>(p1)->X, static_cast<Point*>(p1)->Y);
        QPointF res2 = QPointF(static_cast<Point*>(p2)->X, static_cast<Point*>(p2)->Y);
        QPointF res3 = QPointF(static_cast<Point*>(p3)->X, static_cast<Point*>(p3)->Y);
        auto L32 = QLineF(res3, res2).length();
        auto L12 = QLineF(res1, res2).length();

        if (L12 < EPS || L32 < EPS){
            b->unselectAll();
            b->update();
            QMessageBox::critical(b, "BISECTOR ERROR", "Cannot build a bisector! Points are too close.");
            return;
        } else {
            QPointF dr1 = res1 - res2;
            QPointF dr3 = res3 - res2;
            QPointF dr2 = dr1/L12 + dr3/L32;
            QPointF bisector_point = res2 + dr2;
            Point *b_p = new Point(b, bisector_point.x(), bisector_point.y());
            Ray *r = new Ray(b, static_cast<Point*>(p2), b_p);
            r->exists = true;
            r->depending = true;
            r->child_type = Child_Type::Bisector;
            b->addObject(r);
            p1->childObjects[r] = Child_Type::Bisector;
            p2->childObjects[r] = Child_Type::Bisector;
            p3->childObjects[r] = Child_Type::Bisector;
            r->parentObjects.push_back(p1);
            r->parentObjects.push_back(p2);
            r->parentObjects.push_back(p3);
            r->recalculate();
        }

        b->unselectAll();
        b->update();
        return;
    }
}


void GeoPro::on_actionWhite_triggered()
{
    b->setGrid(false);
    b->setColor(Qt::white);
    b->update();
}


void GeoPro::on_actionGray_triggered()
{
    b->setGrid(false);
    b->setColor(Qt::gray);
    b->update();
}


void GeoPro::on_actionGrid_triggered()
{
    b->setGrid(true);
    b->setColor(Qt::white);
    b->update();
}


void GeoPro::on_actionBack_to_original_view_triggered()
{
    b->scale = 1.0;
    b->shift.rx() = 0;
    b->shift.ry() = 0;
    for (auto obj : b->getAllObj()){
        obj->changeView();
    }
    b->update();
}

