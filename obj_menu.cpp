#include "obj_menu.h"
#include "ui_obj_menu.h"
#include "point.h"
#include "ray.h"
#include "line.h"
#include "segment.h"
#include "circle.h"
#include "angle.h"

#include <QRegExpValidator>
#include <QMenu>
#include <QGroupBox>
#include <QRadioButton>

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
    ui_setup();
    //TODO
}

Obj_menu::~Obj_menu()
{
    gobj = nullptr;
    delete ui;
}

void Obj_menu::ui_setup(){
    resize(450, 150);
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);
    setMinimumSize(QSize(400, 100));
    setMaximumSize(QSize(600, 500));
    ui->verticalLayout = new QVBoxLayout(this);
    ui->verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    ui->horizontalLayout = new QHBoxLayout();
    ui->horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

    font.setFamily(QString::fromUtf8("DejaVu Sans"));

    setWindowTitle(tr("Object menu & settings"));

    layout = new QGridLayout();
    ui->verticalLayout->addLayout(layout);

    QLabel *name_label = new QLabel();
    QString objLabel = gobj->getLabel();
    QString label_is = (objLabel == "") ? "Unnamed object" : ("Current name: " + objLabel);
    name_label->setText(label_is);
    name_label->setFont(font);
    layout->addWidget(name_label, 0, 0);

    ui->label = new QLabel(this);
    ui->label->setObjectName(QString::fromUtf8("label"));
    ui->label->setFont(font);
    ui->label->setText(QApplication::translate("Obj_menu", "Set name:", nullptr));
    ui->horizontalLayout->addWidget(ui->label);
    ui->Name_Edit = new QLineEdit(this);
    ui->Name_Edit->setObjectName(QString::fromUtf8("Name_Edit"));
    ui->horizontalLayout->addWidget(ui->Name_Edit);
    layout->addLayout(ui->horizontalLayout, 0, 1);

    QLabel *type_label = new QLabel();
    type_label->setText("Object type: " + getNameOfType(gobj->type_is()));
    type_label->setFont(font);
    QLabel *depends_label = new QLabel();
    QString dep = (gobj->depending) ? "true" : "false";
    depends_label->setText("Depending: " + dep);
    depends_label->setFont(font);
    layout->addWidget(type_label, 1, 0);
    layout->addWidget(depends_label, 2, 0);

    visible = new QCheckBox("Visible", this);
    if (gobj->is_visible()) visible->setCheckState(Qt::Checked);
    visible->setFont(font);
    trace = new QCheckBox("Leave trace (not availible now)", this);
    trace->setFont(font);
    layout->addWidget(visible, 1, 1);
    layout->addWidget(trace, 2, 1);

    QPushButton *col_popupButton = new QPushButton(tr("Select color (N/A)"));
    col_popupButton->setFont(font);
    QMenu *col_menu = new QMenu();
    col_menu->addAction(tr("Black"));
    col_menu->addAction(tr("Red"));
    col_menu->addAction(tr("Green"));
    col_menu->addAction(tr("Gray"));
    col_popupButton->setMenu(col_menu);

    QPushButton *st_popupButton = new QPushButton(tr("Select style (N/A)"));
    st_popupButton->setFont(font);
    QMenu *st_menu = new QMenu();
    st_menu->addAction(tr("Thin"));
    st_menu->addAction(tr("Thick"));
    st_menu->addAction(tr("Dashed"));
    st_popupButton->setMenu(st_menu);

    layout->addWidget(col_popupButton, 3, 0);
    layout->addWidget(st_popupButton, 3, 1);

    switch (gobj->type_is()) {
    case GObj_Type::POINT:
        ui_setup_point();
        break;
    case GObj_Type::SEGMENT:
        ui_setup_segment();
        break;
    case GObj_Type::LINE:
        ui_setup_line();
        break;
    case GObj_Type::RAY:
        ui_setup_ray();
        break;
    case GObj_Type::CIRCLE:
        ui_setup_circle();
        break;
    case GObj_Type::ANGLE:
        ui_setup_angle();
        break;
    case GObj_Type::TRIANGLE:
        ui_setup_triangle();
        break;
    default:
        break;
    }

    ui->Name_Ok_Button = new QPushButton(this);
    ui->Name_Ok_Button->setObjectName(QString::fromUtf8("Name_Ok_Button"));
    ui->verticalLayout->addWidget(ui->Name_Ok_Button);
    ui->Name_Ok_Button->setText(QApplication::translate("Obj_menu", "Ok", nullptr));

    QMetaObject::connectSlotsByName(this);
}

void Obj_menu::ui_setup_point(){
    resize(450, 250);
    auto p = static_cast<Point*>(gobj);
    QString x = QString::number(p->X);
    QString y = QString::number(p->Y);
    QLabel *coordsx = new QLabel();
    coordsx->setText("x = " + x);
    coordsx->setFont(font);
    layout->addWidget(coordsx, 4, 0);
    QLabel *coordsy = new QLabel();
    coordsy->setText("y = " + y);
    coordsy->setFont(font);
    layout->addWidget(coordsy, 5, 0);

    QRegExpValidator* rxv = new QRegExpValidator(QRegExp("[+-]?\\d*"), this);

    auto horLO1 = new QHBoxLayout();
    horLO1->setObjectName(QString::fromUtf8("horLO1"));
    auto xLabel = new QLabel();
    xLabel->setObjectName(QString::fromUtf8("label"));
    if (gobj->depending) xLabel->setText(QApplication::translate("Obj_menu", "Cannot set x (depending object)", nullptr));
    else xLabel->setText(QApplication::translate("Obj_menu", "Set x:", nullptr));
    xEdit = new QLineEdit();
    xEdit->setObjectName(QString::fromUtf8("xEdit"));
    xEdit->setValidator(rxv);
    horLO1->addWidget(xLabel);
    if (!gobj->depending) horLO1->addWidget(xEdit);

    auto horLO2 = new QHBoxLayout();
    horLO2->setObjectName(QString::fromUtf8("horLO2"));
    auto yLabel = new QLabel();
    yLabel->setObjectName(QString::fromUtf8("label"));
    if (gobj->depending) yLabel->setText(QApplication::translate("Obj_menu", "Cannot set y (depending object)", nullptr));
    else yLabel->setText(QApplication::translate("Obj_menu", "Set y:", nullptr));
    yEdit = new QLineEdit();
    yEdit->setObjectName(QString::fromUtf8("yEdit"));
    yEdit->setValidator(rxv);
    horLO2->addWidget(yLabel);
    if (!gobj->depending) horLO2->addWidget(yEdit);

    layout->addLayout(horLO1, 4, 1);
    layout->addLayout(horLO2, 5, 1);

    fixed = new QCheckBox("Fix the point");
    if (p->is_fixed()) fixed->setCheckState(Qt::Checked);
    fixed->setFont(font);
    auto cannot_fix = new QLabel();
    cannot_fix->setObjectName(QString::fromUtf8("cannot_fix"));
    cannot_fix->setText(QApplication::translate("Obj_menu", "Cannot fix (depending object)", nullptr));
    if (!gobj->depending) layout->addWidget(fixed, 6, 1);
    else layout->addWidget(cannot_fix, 6, 1);
}

void Obj_menu::ui_setup_segment(){
    resize(550, 300);

    auto s = static_cast<Segment*>(gobj);
    QString l = QString::number(s->getlength());
    QLabel *len = new QLabel();
    len->setText("length = " + l);
    len->setFont(font);
    layout->addWidget(len, 4, 0);

    show = new QCheckBox("Show length (not availible now)");
    //if (s->len_showed()) show->setCheckState(Qt::Checked); //will be added soon
    show->setFont(font);
    layout->addWidget(show, 4, 1);

    /*numsticks = new QPushButton(tr("Equality sticks"));
    numsticks->setFont(font);
    QMenu *stick_menu = new QMenu();
    stick_menu->addAction(tr("No sticks"));
    stick_menu->addAction(tr("1"));
    stick_menu->addAction(tr("2"));
    stick_menu->addAction(tr("3"));
    numsticks->setMenu(stick_menu);*/

    /*numsticks = new QGroupBox(tr("Equality sticks"));
    QRadioButton *radio0 = new QRadioButton(tr("No sticks"));
    QRadioButton *radio1 = new QRadioButton(tr("1"));
    QRadioButton *radio2 = new QRadioButton(tr("2"));
    QRadioButton *radio3 = new QRadioButton(tr("3"));
    radio0->setChecked(true);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio0);
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);
    numsticks->setLayout(vbox);*/

    QRegExpValidator* rxv = new QRegExpValidator(QRegExp("[0-3]\\d{0}"), this);
    auto horLO = new QHBoxLayout();
    horLO->setObjectName(QString::fromUtf8("horLO"));
    auto stickLabel = new QLabel();
    stickLabel->setObjectName(QString::fromUtf8("label"));
    stickLabel->setText(QApplication::translate("Obj_menu", "Set number of equality sticks (0-3):", nullptr));
    stickLabel->setFont(font);
    stickEdit = new QLineEdit();
    stickEdit->setObjectName(QString::fromUtf8("stickEdit"));
    stickEdit->setValidator(rxv);
    horLO->addWidget(stickLabel);
    horLO->addWidget(stickEdit);
    layout->addLayout(horLO, 5, 1);


}

void Obj_menu::ui_setup_line(){
    //TODO
}

void Obj_menu::ui_setup_ray(){
    //TODO
}

void Obj_menu::ui_setup_circle(){
    //TODO
}

void Obj_menu::ui_setup_angle(){
    //TODO
}

void Obj_menu::ui_setup_triangle(){
    //TODO
}


void Obj_menu::on_Name_Ok_Button_clicked()
{
    auto newname = ui->Name_Edit->text();
    if (newname != "") gobj->changeLabel(newname);
    if (visible->checkState() == Qt::Unchecked) gobj->hide();

    switch (gobj->type_is()) {
    case GObj_Type::POINT:{
        auto p = static_cast<Point*>(gobj);

        auto newX = xEdit->text();
        if (newX != "") p->X = newX.toInt();

        auto newY = yEdit->text();
        if (newY != "") p->Y = newY.toInt();

        if (fixed->checkState() == Qt::Checked) p->fix();
        else p->unfix();
        break;
    }
    case GObj_Type::SEGMENT:{
        auto s = static_cast<Segment*>(gobj);
        auto newNum = stickEdit->text();
        if (newNum != "") s->setNumSticks(newNum.toInt());
        break;
    }
    case GObj_Type::LINE:
        //TODO
        break;
    case GObj_Type::RAY:
        //TODO
        break;
    case GObj_Type::CIRCLE:
        //TODO
        break;
    case GObj_Type::ANGLE:
        //TODO
        break;
    case GObj_Type::TRIANGLE:
        //TODO
        break;
    default:
        break;
    }

    gobj->changeView();
    close();
}

void Obj_menu::setnumsticks(){
    auto s = static_cast<Segment*>(gobj);
    s->setNumSticks(2);
}

