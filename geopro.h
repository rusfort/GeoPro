#ifndef GEOPRO_H
#define GEOPRO_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QtGui>
#include "painthelper.h"
#include "grwidget.h"

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
    void resizeEvent(QResizeEvent *event) override;
    //void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    void on_actionPoint_triggered();

    void on_actionLine_triggered();

    void on_actionSegment_triggered();

private:
    Ui::GeoPro *ui;
    //GraphWidget *widget;
};

#endif // GEOPRO_H
