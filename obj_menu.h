#ifndef OBJ_MENU_H
#define OBJ_MENU_H

#include <QDialog>
#include <QCheckBox>
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
    void ui_setup_point();
    void ui_setup_segment();
    void ui_setup_line();
    void ui_setup_ray();
    void ui_setup_circle();
    void ui_setup_angle();
    void ui_setup_triangle();
    ~Obj_menu();

private slots:
    void on_Name_Ok_Button_clicked();

private:
    Ui::Obj_menu *ui;
    GOBJ* gobj;
    QFont font;
    QGridLayout *layout;
    QCheckBox *visible;
    QCheckBox *trace;
    QCheckBox *fixed;
    QCheckBox *show;
    QLineEdit *xEdit;
    QLineEdit *yEdit;
    QPushButton *numsticks;
};

#endif // OBJ_MENU_H
