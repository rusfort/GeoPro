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

#include "line.h"
#include "point.h"
#include "ray.h"
#include "segment.h"
#include "circle.h"
#include "angle.h"
#include "triangle.h"

GeoPro::GeoPro(QWidget *parent) : QMainWindow(parent), ui(new Ui::GeoPro) {
    ui->setupUi(this);
    setWindowTitle(tr("GeoPro 0.0.3 [alpha] by Nikolay Kozakov"));

    auto ex = new QShortcut(this);
    ex->setKey(Qt::Key_Backspace);
    connect(ex, SIGNAL(activated()), this, SLOT(on_actionDelete_selected_objects_triggered()));
    auto res = new QShortcut(this);
    res->setKey(Qt::Key_U);
    connect(res, SIGNAL(activated()), this, SLOT(restoreFromDump())); // TEST!

    b = new GeoBoard(this);
    setCentralWidget(b);
}

GeoPro::~GeoPro() { delete ui; }

void GeoPro::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "EXIT", "Are you sure you want to exit?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        event->ignore();
    } else {
        b->getAllObj().clear();
        b->update();
        event->accept();
    }
}

void GeoPro::on_actionClose_triggered()
{
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
                if (!Obj1) {
                    Obj1 = obj;
                    continue;
                }
                Obj2 = obj;
                break;
            }
        }
        if (Obj1 == 0 || Obj2 == 0){ // !Obj1 || !Obj2
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
        b->connect_objects(Obj1, intersection, Child_Type::Intersection);
        b->connect_objects(Obj2, intersection, Child_Type::Intersection);
        intersection->parents_intersected.insert(std::make_pair(Obj1->type_is(), Obj1));
        intersection->parents_intersected.insert(std::make_pair(Obj2->type_is(), Obj2));
        b->addObject(intersection);
        intersection->setIntersectionType();
        intersection->recalculate();
        if (inter_circle){
            auto it2 = intersection->parents_intersected.begin();
            auto it1 = it2;
            ++it2;
            intersection2->parents_intersected.insert(*it1);
            intersection2->parents_intersected.insert(*it2);
            intersection2->exists = false;
            b->connect_objects(Obj1, intersection2, Child_Type::Intersection2);
            b->connect_objects(Obj2, intersection2, Child_Type::Intersection2);
            b->addObject(intersection2);
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

bool GeoPro::getThreePoints(const QString& Obj_name, const QString& Err_msg){
    if(b->numitemstoadd > 0) return false;
    if(b->num_obj_selected > 3 || (b->num_obj_selected < 3 && b->num_obj_selected > 0)){
        QMessageBox::critical(b, Obj_name + " ERROR", "Need 3 points!");
        b->unselectAll();
        b->update();
        return false;
    }
    if(b->num_obj_selected == 0){
        QMessageBox::warning(b, Obj_name + " WARNING", "Select 3 points!");
        return false;
    }

    GOBJ* p1 = 0;
    GOBJ* p2 = 0;
    GOBJ* p3 = 0;
    for (auto& obj : b->getAllObj()){
        if (obj->isSelected()){
            if (!p1) {
                p1 = obj;
                continue;
            }
            if(!p2) {
                p2 = obj;
                continue;
            }
            p3 = obj;
            break;
        }
    }
    if (p1->type_is() != GObj_Type::POINT || p2->type_is() != GObj_Type::POINT || p3->type_is() != GObj_Type::POINT){
        b->unselectAll();
        b->update();
        QMessageBox::critical(b, Obj_name + " ERROR", Err_msg);
        return false;
    }

    threePoints.clear();
    threePoints.push_back(static_cast<Point*>(p1));
    threePoints.push_back(static_cast<Point*>(p2));
    threePoints.push_back(static_cast<Point*>(p3));

    return true;
}

void GeoPro::on_actionCircle_by_3_points_triggered()
{
    if(!getThreePoints("CIRCLE", "Cannot build a circle! Need 3 points.")) return;
    Point* p1 = threePoints[0];
    Point* p2 = threePoints[1];
    Point* p3 = threePoints[2];
    auto params = getCircleCenterAndRadius(p1, p2, p3);
    if (params.second < EPS){
        b->unselectAll();
        b->update();
        QMessageBox::critical(b, "CIRCLE ERROR", "Cannot build a circle! 3 points on the same line!");
        return;
    }
    Point* cen = new Point(b, params.first.x(), params.first.y());
    Circle* C = new Circle(b, cen, params.second);
    if (C->r() < EPS) C->exists = false;
    else C->exists = true;
    b->connect_objects(cen, C, Child_Type::Middle);
    b->connect_objects(p1, cen, Child_Type::OnThreePoints);
    b->connect_objects(p2, cen, Child_Type::OnThreePoints);
    b->connect_objects(p3, cen, Child_Type::OnThreePoints);
    b->addObject(cen);
    //b->addObject(C);
    C->basePoints.push_back(p1);
    C->basePoints.push_back(p2);
    C->basePoints.push_back(p3);
    C->recalculate();
    b->unselectAll();
    b->update();
    return;
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
                if(!p1) {
                    p1 = obj;
                    continue;
                }
                if(!p2) {
                    p2 = obj;
                    break;
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
        b->connect_objects(p1, middle, Child_Type::Middle);
        b->connect_objects(p2, middle, Child_Type::Middle);
        b->addObject(middle);
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
                if(!p1) {
                    p1 = obj;
                    continue;
                }
                if(!p2) {
                    p2 = obj;
                    break;
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
        b->connect_objects(p1, parallel, Child_Type::Parallel);
        b->connect_objects(p2, parallel, Child_Type::Parallel);
        b->addObject(parallel);
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
                if(!p1) {
                    p1 = obj;
                    continue;
                }
                if(!p2) {
                    p2 = obj;
                    break;
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
        auto perpendicular = new Line(b, tmp_k, -1, (point1->Y - line->k() * point1->X));
        perpendicular->setBaseLine(p2);
        perpendicular->setBasePoint(point1);

        perpendicular->exists = true;
        b->connect_objects(p1, perpendicular, Child_Type::Perpendicular);
        b->connect_objects(p2, perpendicular, Child_Type::Perpendicular);
        b->addObject(perpendicular);
        perpendicular->recalculate();

        b->unselectAll();
        b->update();

        if (p2->type_is() != GObj_Type::LINE) delete line;
        return;
    }
}


void GeoPro::on_actionBisector_triggered()
{
    if(!getThreePoints("BISECTOR", "Cannot build a bisector! Need 3 points.")) return;
    if (b->onOneLine(threePoints[0], threePoints[1], threePoints[2])){
        b->unselectAll();
        b->update();
        QMessageBox::critical(b, "BISECTOR ERROR", "Cannot build a bisector! 3 points on the same line!\nTip: use perpendicular line.");
        return;
    }

    //forget the previous points (because order was random)
    auto p1 = b->getThreeOrderedPoints()[0];
    auto p2 = b->getThreeOrderedPoints()[1];
    auto p3 = b->getThreeOrderedPoints()[2];

    b->clear_threeOrderedPoints();

    QPointF res1 = QPointF(p1->X, p1->Y);
    QPointF res2 = QPointF(p2->X, p2->Y);
    QPointF res3 = QPointF(p3->X, p3->Y);
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
        Ray *r = new Ray(b, p2, b_p);
        r->exists = true;
        b->connect_objects(p1, r, Child_Type::Bisector);
        b->connect_objects(p2, r, Child_Type::Bisector);
        b->connect_objects(p3, r, Child_Type::Bisector);
        b->addObject(r);
        r->recalculate();
    }

    b->unselectAll();
    b->update();
    return;
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


void GeoPro::on_actionOpen_object_settings_triggered()
{
    if(b->numitemstoadd > 0) return;
    if(b->num_obj_selected > 1){
        QMessageBox::critical(b, "OBJECT SETTINGS ERROR", "Can manage only one object!");
        b->unselectAll();
        b->update();
        return;
    }
    if(b->num_obj_selected == 0){
        QMessageBox::warning(b, "OBJECT SETTINGS WARNING", "Select an object!");
        return;
    }
    if(b->num_obj_selected == 1){
        GOBJ* p = 0;
        for (auto& obj : b->getAllObj()){
            if (obj->isSelected()){
                if (!p) p = obj;
                else break;
            }
        }
        if(!p){
            QMessageBox::critical(b, "OBJECT SETTINGS ERROR", "NULL OBJECT ERROR");
            b->unselectAll();
            b->update();
            return;
        }

        menu = new Obj_menu(this, p);
        menu->setAttribute(Qt::WA_DeleteOnClose);
        menu->exec();
    }
    b->unselectAll();
    b->update();
    return;
}

void GeoPro::change_label(GOBJ* obj){
    QString Label(b->AddLabel->text());

    obj->changeLabel(Label);
}


void GeoPro::on_actionAngle_by_3_points_triggered()
{
    if(!getThreePoints("ANGLE", "Cannot build an angle! Need 3 points.")) return;
    if (b->onOneLine(threePoints[0], threePoints[1], threePoints[2])){
        b->unselectAll();
        b->update();
        QMessageBox::critical(b, "ANGLE ERROR", "Cannot build an angle! 3 points on the same line!");
        return;
    }

    //forget the previous points (because order was random)
    auto p1 = b->getThreeOrderedPoints()[0];
    auto p2 = b->getThreeOrderedPoints()[1];
    auto p3 = b->getThreeOrderedPoints()[2];

    b->clear_threeOrderedPoints();

    QPointF res1 = QPointF(p1->X, p1->Y);
    QPointF res2 = QPointF(p2->X, p2->Y);
    QPointF res3 = QPointF(p3->X, p3->Y);
    auto L32 = QLineF(res3, res2).length();
    auto L12 = QLineF(res1, res2).length();

    if (L12 < EPS || L32 < EPS){
        b->unselectAll();
        b->update();
        QMessageBox::critical(b, "ANGLE ERROR", "Cannot build an angle! Points are too close.");
        return;
    } else {
        Angle *a = new Angle(b, p2, p1, p3);
        a->exists = true;
        b->connect_objects(p1, a, Child_Type::Angle);
        b->connect_objects(p2, a, Child_Type::Angle);
        b->connect_objects(p3, a, Child_Type::Angle);
        b->addObject(a);
        a->recalculate();
    }

    b->unselectAll();
    b->update();
    return;
}


void GeoPro::on_actionAngle_by_the_ray_and_point_triggered()
{
    if(b->numitemstoadd > 0) return;
    if(b->num_obj_selected > 2 || b->num_obj_selected == 1){
        QMessageBox::critical(b, "ANGLE ERROR", "This type of angle need one ray and one point!");
        b->unselectAll();
        b->update();
        return;
    }
    if(b->num_obj_selected == 0){
        QMessageBox::warning(b, "ANGLE WARNING", "Select one ray and one point!");
        return;
    }

    GOBJ* o1 = 0;
    GOBJ* o2 = 0;

    for (auto& obj : b->getAllObj()){
        if (obj->isSelected()){
            if(!o1) {
                o1 = obj;
                continue;
            }
            if(!o2) {
                o2 = obj;
                break;
            }
        }
    }

    if ((o1->type_is() != GObj_Type::POINT && o2->type_is() != GObj_Type::POINT)
            || (o1->type_is() != GObj_Type::RAY && o2->type_is() != GObj_Type::RAY)){
        b->unselectAll();
        b->update();
        QMessageBox::critical(b, "ANGLE ERROR", "Cannot build an angle! This type of angle need one ray and one point.");
        return;
    }

    if (o1->type_is() != GObj_Type::POINT) std::swap(o1, o2);

    auto r = static_cast<Ray*>(o2);
    auto p = static_cast<Point*>(o1);

    if (b->onOneLine(p, r->getFirstPoint(), r->getSecondPoint())){
        b->unselectAll();
        b->update();
        QMessageBox::critical(b, "ANGLE ERROR", "Cannot build an angle! Three points on the one line.");
        return;
    }

    Angle *a = new Angle(b, r, p);
    a->exists = true;
    b->connect_objects(o1, a, Child_Type::Angle);
    b->connect_objects(o2, a, Child_Type::Angle);
    b->addObject(a);
    a->recalculate();
    b->update();
}


void GeoPro::on_actionAngle_by_the_ray_and_degree_measure_triggered()
{
    QMessageBox::information(b, "ANGLE BY DEGREE MEASURE", "Will be available in beta :)");
}


void GeoPro::on_actionTriangle_triggered()
{
    if (b->num_obj_selected == 0){
        b->trytoadd = GObj_Type::TRIANGLE;
        b->numitemstoadd = 3;
        b->setMouseTracking(true);
        return;
    }
    if(!getThreePoints("TRIANGLE", "Cannot build a triangle! Need 3 points.")) return;
    if (b->onOneLine(threePoints[0], threePoints[1], threePoints[2])){
        b->unselectAll();
        b->update();
        QMessageBox::critical(b, "TRIANGLE ERROR", "Cannot build a triangle! 3 points on the same line!");
        return;
    }

    //forget the previous points (because order was random)
    auto p1 = b->getThreeOrderedPoints()[0];
    auto p2 = b->getThreeOrderedPoints()[1];
    auto p3 = b->getThreeOrderedPoints()[2];

    b->clear_threeOrderedPoints();

    Triangle *t = new Triangle(b, p1, p2, p3);
    t->exists = true;
    b->connect_objects(p1, t, Child_Type::Triangle);
    b->connect_objects(p2, t, Child_Type::Triangle);
    b->connect_objects(p3, t, Child_Type::Triangle);
    b->addObject(t);
    t->recalculate();

    b->unselectAll();
    b->update();
    return;
}

void GeoPro::restoreFromDump(){
    QFile dump(".//cache_test.gprc");

    if(!dump.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::critical(this, "Dump error", "Cannot restore from dump");
        return;
    }

    QTextStream stream(&dump);

    //header parsing
    qreal tmp;
    stream >> tmp;
    b->scale = tmp;
    stream >> tmp;
    b->shift.rx() = tmp;
    stream >> tmp;
    b->shift.ry() = tmp;
    int n;
    stream >> n; //number of objects

    b->parsedObjects.clear();
    b->getAllObj().clear();

    //objects parsing
    for (int i = 0; i < n; ++i) {
        if (!b->parseObject(stream)){
            QMessageBox::critical(this, "Dump error", "Cannot parse correctly");
            return;
        }
    }

    //updating the board
    for(auto obj : b->getAllObj())
    {
        obj->recalculate();
        obj->changeView();
    }
    b->update();
    dump.flush();
    dump.close();
}


void GeoPro::on_actionUndo_triggered()
{
    if (b->lastStates.empty()) return;
    auto cache = b->lastStates.back();
    b->loadFromCache(cache);
    b->lastStates.pop_back();
    b->nextStates.push_front(cache);
    if (b->nextStates.size() > b->max_cache_size) b->nextStates.pop_back();
}


void GeoPro::on_actionRedo_triggered()
{
    if (b->nextStates.empty()) return;
    auto cache = b->nextStates.front();
    b->loadFromCache(cache);
    b->nextStates.pop_front();
    b->lastStates.push_back(cache);
    if (b->lastStates.size() > b->max_cache_size) b->lastStates.pop_front();
}

