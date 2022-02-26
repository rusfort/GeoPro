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
    ui_setup();
    //TODO
}

Obj_menu::~Obj_menu()
{
    delete ui;
}

void Obj_menu::ui_setup(){
    resize(400, 100);
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);
    setMinimumSize(QSize(400, 100));
    setMaximumSize(QSize(500, 500));
    ui->verticalLayout = new QVBoxLayout(this);
    ui->verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    ui->horizontalLayout = new QHBoxLayout();
    ui->horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    ui->label = new QLabel(this);
    ui->label->setObjectName(QString::fromUtf8("label"));
    QFont font;
    font.setFamily(QString::fromUtf8("DejaVu Sans"));
    ui->label->setFont(font);

    ui->horizontalLayout->addWidget(ui->label);

    ui->Name_Edit = new QLineEdit(this);
    ui->Name_Edit->setObjectName(QString::fromUtf8("Name_Edit"));

    ui->horizontalLayout->addWidget(ui->Name_Edit);

    ui->Name_Ok_Button = new QPushButton(this);
    ui->Name_Ok_Button->setObjectName(QString::fromUtf8("Name_Ok_Button"));

    ui->horizontalLayout->addWidget(ui->Name_Ok_Button);


    ui->verticalLayout->addLayout(ui->horizontalLayout);

    ui->label->setText(QApplication::translate("Obj_menu", "Name:", nullptr));
    ui->Name_Ok_Button->setText(QApplication::translate("Obj_menu", "Ok", nullptr));

    QMetaObject::connectSlotsByName(this);

    setWindowTitle(tr("Object menu & settings"));

    layout = new QGridLayout();
    ui->verticalLayout->addLayout(layout);
    QLabel *type_label = new QLabel();
    type_label->setText("Object type: " + getNameOfType(gobj->type_is()));
    type_label->setFont(font);
    QLabel *depends_label = new QLabel();
    QString dep = (gobj->depending) ? "true" : "false";
    depends_label->setText("Depending: " + dep);
    depends_label->setFont(font);
    layout->addWidget(type_label, 0, 0);
    layout->addWidget(depends_label, 1, 0);

    visible = new QCheckBox("Visible", this);
    if (gobj->is_visible()) visible->setCheckState(Qt::Checked);
    visible->setFont(font);
    trace = new QCheckBox("Leave trace (not availible now)", this);
    trace->setFont(font);
    layout->addWidget(visible, 0, 1);
    layout->addWidget(trace, 1, 1);
}

void Obj_menu::on_Name_Ok_Button_clicked()
{
    gobj->changeLabel(ui->Name_Edit->text());
    close();
}

