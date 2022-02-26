#ifndef OBJ_MENU_H
#define OBJ_MENU_H

#include <QDialog>
#include "geometry_main.h"

namespace Ui {
class Obj_menu;
}

class Obj_menu : public QDialog
{
    Q_OBJECT

public:
    explicit Obj_menu(QWidget *parent = nullptr);
    explicit Obj_menu(QWidget *parent, GOBJ* ptr);
    void ui_setup();
    ~Obj_menu();

private slots:
    void on_Name_Ok_Button_clicked();

private:
    Ui::Obj_menu *ui;
    GOBJ* gobj;
    QGridLayout *layout;
};

#endif // OBJ_MENU_H
