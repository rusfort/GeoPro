#include "obj_menu.h"
#include "ui_obj_menu.h"

Obj_menu::Obj_menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Obj_menu)
{
    ui->setupUi(this);
    setWindowTitle(tr("Object menu & settings"));
}

Obj_menu::Obj_menu(QWidget *parent, GOBJ* ptr) :
    QDialog(parent), ui(new Ui::Obj_menu), gobj(ptr)
{
    ui->setupUi(this);
    setWindowTitle(tr("Object menu & settings"));
    QGridLayout *layout = new QGridLayout();
    ui->verticalLayout->addLayout(layout);
    QLabel *type_label = new QLabel();
    QLabel *type = new QLabel();
    type_label->setText("Object type: ");
    type->setText(getNameOfType(gobj->type_is()));
    layout->addWidget(type_label, 0, 0);
    layout->addWidget(type, 0, 1);
}

Obj_menu::~Obj_menu()
{
    delete ui;
}

void Obj_menu::on_Name_Ok_Button_clicked()
{
    gobj->changeLabel(ui->Name_Edit->text());
    close();
}

