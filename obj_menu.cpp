#include "obj_menu.h"
#include "ui_obj_menu.h"
#include "point.h"
#include "ray.h"
#include "line.h"
#include "segment.h"
#include "circle.h"
#include "angle.h"
#include "triangle.h"

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
    resize(500, 250);
    auto l = static_cast<Line*>(gobj);
    QString k = QString::number(l->k());
    bool negX = false;
    bool negY = false;
    auto getX = l->x0();
    if (getX < 0){
        negX = true;
        getX = -getX;
    }
    auto getY = l->y0();
    if (getY < 0){
        negY = true;
        getY = -getY;
    }
    QString x0 = QString::number(getX);
    QString y0 = QString::number(getY);
    QLabel *equation_label = new QLabel();
    equation_label->setText("Line equation: ");
    equation_label->setFont(font);
    layout->addWidget(equation_label, 4, 0);
    QLabel *equation = new QLabel();
    if (!negX && !negY) equation->setText("y - " + y0 + " = " + k + " * (x - " + x0 + ")");
    if ( negX && !negY) equation->setText("y - " + y0 + " = " + k + " * (x + " + x0 + ")");
    if (!negX &&  negY) equation->setText("y + " + y0 + " = " + k + " * (x - " + x0 + ")");
    if ( negX &&  negY) equation->setText("y + " + y0 + " = " + k + " * (x + " + x0 + ")");
    equation->setFont(font);
    layout->addWidget(equation, 4, 1);

    QLabel *line_type = new QLabel();
    line_type->setText("Line type: " + getNameOfChildType(gobj->get_child_type()));
    line_type->setFont(font);
    layout->addWidget(line_type, 5, 0);
}

void Obj_menu::ui_setup_ray(){
    resize(500, 250);
    auto r = static_cast<Ray*>(gobj);
    QString k = QString::number(r->k());
    bool negX = false;
    bool negY = false;
    auto getX = r->x0();
    if (getX < 0){
        negX = true;
        getX = -getX;
    }
    auto getY = r->y0();
    if (getY < 0){
        negY = true;
        getY = -getY;
    }
    QString x0 = QString::number(getX);
    QString y0 = QString::number(getY);
    QLabel *equation_label = new QLabel();
    equation_label->setText("Ray (line) equation: ");
    equation_label->setFont(font);
    layout->addWidget(equation_label, 4, 0);
    QLabel *equation = new QLabel();
    if (!negX && !negY) equation->setText("y - " + y0 + " = " + k + " * (x - " + x0 + ")");
    if ( negX && !negY) equation->setText("y - " + y0 + " = " + k + " * (x + " + x0 + ")");
    if (!negX &&  negY) equation->setText("y + " + y0 + " = " + k + " * (x - " + x0 + ")");
    if ( negX &&  negY) equation->setText("y + " + y0 + " = " + k + " * (x + " + x0 + ")");
    equation->setFont(font);
    layout->addWidget(equation, 4, 1);

    QLabel *ray_type = new QLabel();
    ray_type->setText("Ray type: " + getNameOfChildType(gobj->get_child_type()));
    ray_type->setFont(font);
    layout->addWidget(ray_type, 5, 0);
}

void Obj_menu::ui_setup_circle(){
    resize(450, 250);
    auto c = static_cast<Circle*>(gobj);
    auto p = c->getcenter();
    QString x = QString::number(p->X);
    QString y = QString::number(p->Y);
    QString r = QString::number(c->r());
    QLabel *coordsx = new QLabel();
    coordsx->setText("Center x = " + x);
    coordsx->setFont(font);
    layout->addWidget(coordsx, 4, 0);
    QLabel *coordsy = new QLabel();
    coordsy->setText("Center y = " + y);
    coordsy->setFont(font);
    layout->addWidget(coordsy, 5, 0);
    QLabel *rad = new QLabel();
    rad->setText("Radius = " + r);
    rad->setFont(font);
    layout->addWidget(rad, 4, 1);
}

void Obj_menu::ui_setup_angle(){
    resize(550, 300);

    auto a = static_cast<Angle*>(gobj);
    QString d = QString::number(a->degrees());
    QLabel *len = new QLabel();
    len->setText("degree measure = " + d);
    len->setFont(font);
    layout->addWidget(len, 4, 0);

    show = new QCheckBox("Show degree measure \n(not availible now)");
    //if (s->deg_showed()) show->setCheckState(Qt::Checked); //will be added soon
    show->setFont(font);
    layout->addWidget(show, 4, 1);

    QRegExpValidator* rxv = new QRegExpValidator(QRegExp("[0-3]\\d{0}"), this);
    auto horLO = new QHBoxLayout();
    horLO->setObjectName(QString::fromUtf8("horLO"));
    auto stickLabel = new QLabel();
    stickLabel->setObjectName(QString::fromUtf8("label"));
    stickLabel->setText(QApplication::translate("Obj_menu", "Set number of equality arcs (0-3):", nullptr));
    stickLabel->setFont(font);
    stickEdit = new QLineEdit();
    stickEdit->setObjectName(QString::fromUtf8("stickEdit"));
    stickEdit->setValidator(rxv);
    horLO->addWidget(stickLabel);
    horLO->addWidget(stickEdit);
    layout->addLayout(horLO, 5, 1);
}

void Obj_menu::ui_setup_triangle(){
    resize(550, 550);
    auto t = static_cast<Triangle*>(gobj);
    QString a = QString::number(t->a());
    QString b = QString::number(t->b());
    QString c = QString::number(t->c());
    QString per = QString::number(t->perimeter());
    QString area = QString::number(t->area());
    QLabel *a_label = new QLabel();
    a_label->setText("a = " + a);
    a_label->setFont(font);
    layout->addWidget(a_label, 4, 0);
    QLabel *b_label = new QLabel();
    b_label->setText("b = " + b);
    b_label->setFont(font);
    layout->addWidget(b_label, 5, 0);
    QLabel *c_label = new QLabel();
    c_label->setText("c = " + c);
    c_label->setFont(font);
    layout->addWidget(c_label, 6, 0);
    QLabel *p_label = new QLabel();
    p_label->setText("perimeter = " + per);
    p_label->setFont(font);
    layout->addWidget(p_label, 7, 0);
    QLabel *ar_label = new QLabel();
    ar_label->setText("area = " + area);
    ar_label->setFont(font);
    layout->addWidget(ar_label, 8, 0);

    //main points

    auto box = new QCheckBox("Show bisectors intersection");
    if (t->manage_bis()->is_visible()) box->setCheckState(Qt::Checked);
    boxes.push_back(box);
    box->setFont(font);
    layout->addWidget(box, 4, 1);

    box = new QCheckBox("Show medians intersection");
    if (t->manage_med()->is_visible()) box->setCheckState(Qt::Checked);
    boxes.push_back(box);
    box->setFont(font);
    layout->addWidget(box, 5, 1);

    box = new QCheckBox("Show heights intersection");
    if (t->manage_hgt()->is_visible()) box->setCheckState(Qt::Checked);
    boxes.push_back(box);
    box->setFont(font);
    layout->addWidget(box, 6, 1);

    box = new QCheckBox("Show midperpendiculars intersection");
    if (t->manage_mdp()->is_visible()) box->setCheckState(Qt::Checked);
    boxes.push_back(box);
    box->setFont(font);
    layout->addWidget(box, 7, 1);

    box = new QCheckBox("Show Euler circle center");
    if (t->manage_eul()->is_visible()) box->setCheckState(Qt::Checked);
    boxes.push_back(box);
    box->setFont(font);
    layout->addWidget(box, 8, 1);

    //main angles

    box = new QCheckBox("Show alpha angle");
    if (t->manage_alpha()->is_visible()) box->setCheckState(Qt::Checked);
    boxes.push_back(box);
    box->setFont(font);
    layout->addWidget(box, 9, 1);

    box = new QCheckBox("Show beta angle");
    if (t->manage_beta()->is_visible()) box->setCheckState(Qt::Checked);
    boxes.push_back(box);
    box->setFont(font);
    layout->addWidget(box, 10, 1);

    box = new QCheckBox("Show gamma angle");
    if (t->manage_gamma()->is_visible()) box->setCheckState(Qt::Checked);
    boxes.push_back(box);
    box->setFont(font);
    layout->addWidget(box, 11, 1);

    //main circles

    box = new QCheckBox("Show inscribed circle");
    if (t->manage_in()->is_visible()) box->setCheckState(Qt::Checked);
    boxes.push_back(box);
    box->setFont(font);
    layout->addWidget(box, 9, 0);

    box = new QCheckBox("Show circumscribed circle");
    if (t->manage_cir()->is_visible()) box->setCheckState(Qt::Checked);
    boxes.push_back(box);
    box->setFont(font);
    layout->addWidget(box, 10, 0);

    box = new QCheckBox("Show Euler circle");
    if (t->manage_Euler()->is_visible()) box->setCheckState(Qt::Checked);
    boxes.push_back(box);
    box->setFont(font);
    layout->addWidget(box, 11, 0);
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
    case GObj_Type::ANGLE:{
        auto a = static_cast<Angle*>(gobj);
        auto newNum = stickEdit->text();
        if (newNum != "") a->setNumArcs(newNum.toInt());
        break;
    }
    case GObj_Type::TRIANGLE:{
        auto t = static_cast<Triangle*>(gobj);
        if (boxes[0]->checkState() == Qt::Checked) t->manage_bis()->make_visible();
        else t->manage_bis()->hide();
        if (boxes[1]->checkState() == Qt::Checked) t->manage_med()->make_visible();
        else t->manage_med()->hide();
        if (boxes[2]->checkState() == Qt::Checked) t->manage_hgt()->make_visible();
        else t->manage_hgt()->hide();
        if (boxes[3]->checkState() == Qt::Checked) t->manage_mdp()->make_visible();
        else t->manage_mdp()->hide();
        if (boxes[4]->checkState() == Qt::Checked) t->manage_eul()->make_visible();
        else t->manage_eul()->hide();

        if (boxes[5]->checkState() == Qt::Checked) t->manage_alpha()->make_visible();
        else t->manage_alpha()->hide();
        if (boxes[6]->checkState() == Qt::Checked) t->manage_beta()->make_visible();
        else t->manage_beta()->hide();
        if (boxes[7]->checkState() == Qt::Checked) t->manage_gamma()->make_visible();
        else t->manage_gamma()->hide();

        if (boxes[8]->checkState() == Qt::Checked) t->manage_in()->make_visible();
        else t->manage_in()->hide();
        if (boxes[9]->checkState() == Qt::Checked) t->manage_cir()->make_visible();
        else t->manage_cir()->hide();
        if (boxes[10]->checkState() == Qt::Checked) t->manage_Euler()->make_visible();
        else t->manage_Euler()->hide();
        break;
    }
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

