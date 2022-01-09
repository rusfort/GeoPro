#ifndef GEOPRO_H
#define GEOPRO_H

#include <QMainWindow>
#include "painthelper.h"

namespace Ui {
class GeoPro;
}

class GeoPro : public QMainWindow
{
    Q_OBJECT

public:
    explicit GeoPro(QWidget *parent = 0);
    ~GeoPro();

private slots:
    void on_actionClose_triggered();

    void on_graph_destroyed();

private:
    Ui::GeoPro *ui;
    Helper helper;
};

#endif // GEOPRO_H
