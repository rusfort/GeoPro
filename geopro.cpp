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


GeoPro::GeoPro(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GeoPro)
{
    ui->setupUi(this);
    setWindowTitle(tr("GeoPro 0.0.1 [alpha] by Nikolay Kozakov"));

    b = new GeoBoard(this);
    setCentralWidget(b);

    ///QTimer *timer = new QTimer(this);
    ///connect(timer, &QTimer::timeout, native, &Widget::animate);
    ///timer->start(50);
}

GeoPro::~GeoPro()
{
    delete ui;
}

void GeoPro::on_actionClose_triggered()
{
    QApplication::quit();
}

void GeoPro::on_actionPoint_triggered()
{
    if(b->numitemstoadd > 0) return;
    b->unselectAll();
    b->trytoadd = GObj_Type::POINT;
    b->numitemstoadd = 1;
}

void GeoPro::on_actionLine_triggered()
{
    if(b->numitemstoadd > 0) return;
    b->unselectAll();
    b->trytoadd = GObj_Type::LINE;
    b->numitemstoadd = 2;
}

void GeoPro::on_actionSegment_triggered()
{
    if(b->numitemstoadd > 0) return;
    b->unselectAll();
    b->trytoadd = GObj_Type::SEGMENT;
    b->numitemstoadd = 2;
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
    b->getAllObj().clear();
    b->update();
}

void GeoPro::on_actionRay_triggered()
{
    if(b->numitemstoadd > 0) return;
    b->unselectAll();
    b->trytoadd = GObj_Type::RAY;
    b->numitemstoadd = 2;
}

void GeoPro::on_actionIntersection_triggered()
{
    if(b->numitemstoadd > 0) return;
    b->unselectAll();
    if(b->num_obj_selected > 2 || (b->num_obj_selected < 2 && b->num_obj_selected > 0)){
        QMessageBox::critical(b, "ERROR", "Can intersect only 2 objects!");
        return;
    }
    if(b->num_obj_selected == 0){
        QMessageBox::warning(b, "WARNING", "Select 2 objects to intersect!");
        return;
    }
    if(b->num_obj_selected == 2){
        b->trytoadd = GObj_Type::INTERSECTION;
        b->numitemstoadd = 1;
        return;
    }
}
