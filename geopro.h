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
    void closeEvent (QCloseEvent *event) override;

    void on_actionClose_triggered();

    void on_actionPoint_triggered();

    void on_actionLine_triggered();

    void on_actionSegment_triggered();

    void on_actionDelete_selected_objects_triggered();

    void on_actionClear_all_triggered();

    void on_actionRay_triggered();

    void on_actionIntersection_triggered();

    void on_actionCircle_by_the_center_radius_triggered();

    void on_actionCircle_by_3_points_triggered();

    void on_actionMiddle_Center_triggered();

    void on_actionAbout_triggered();

    void on_actionContact_author_triggered();

    void on_actionHide_selected_objects_triggered();

    void on_actionShow_all_hidden_objects_triggered();

    void on_actionParallel_line_triggered();

    void on_actionPerpendicular_line_triggered();

    void on_actionBisector_triggered();

    void on_actionWhite_triggered();

    void on_actionGray_triggered();

    void on_actionGrid_triggered();

    void on_actionBack_to_original_view_triggered();

private:
    Ui::GeoPro *ui;
public:
    GeoBoard *b;
};

#endif // GEOPRO_H
