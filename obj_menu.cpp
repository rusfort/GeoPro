#include "obj_menu.h"
#include "ui_obj_menu.h"

Obj_menu::Obj_menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Obj_menu)
{
    ui->setupUi(this);
    setWindowTitle(tr("Menu"));
}

Obj_menu::~Obj_menu()
{
    delete ui;
}

void Obj_menu::on_Name_Ok_Button_clicked()
{
    content = ui->Name_Edit->text();
}

