#ifndef GEOPRO_H
#define GEOPRO_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QtGui>
#include "widget.h"

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

    void on_actionPoint_triggered();

    void on_actionLine_triggered();

    void on_actionSegment_triggered();

private:
    Ui::GeoPro *ui;
public:
    GeoBoard *b;
    //GraphWidget *widget;
};

#endif // GEOPRO_H
