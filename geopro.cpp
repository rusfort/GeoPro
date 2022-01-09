#include "geopro.h"
#include "ui_geopro.h"

GeoPro::GeoPro(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GeoPro)
{
    ui->setupUi(this);
}

GeoPro::~GeoPro()
{
    delete ui;
}
