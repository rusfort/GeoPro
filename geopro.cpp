#include "geopro.h"
#include "ui_geopro.h"
#include "painthelper.h"
#include "widget.h"
#include "geometry_main.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>


GeoPro::GeoPro(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GeoPro)
{
    ui->setupUi(this);
    Widget *native = new Widget(&helper, this);
    //QLabel *nativeLabel = new QLabel(tr("Native"));
    //nativeLabel->setAlignment(Qt::AlignHCenter);

    auto central = new QWidget;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(native, 0, 0);
    //layout->addWidget(nativeLabel, 1, 0);
    central->setLayout(layout);

    setCentralWidget(central);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, native, &Widget::animate);
    timer->start(50);
}

GeoPro::~GeoPro()
{
    delete ui;
}

void GeoPro::on_actionClose_triggered()
{
    QApplication::quit();
}
