#ifndef GEOPRO_H
#define GEOPRO_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QtGui>
#include "widget.h"
#include "obj_menu.h"

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

    void on_actionOpen_object_settings_triggered();

    void change_label(GOBJ* obj);

    void on_actionAngle_by_3_points_triggered();

    void on_actionAngle_by_the_ray_and_point_triggered();

    void on_actionAngle_by_the_ray_and_degree_measure_triggered();

    bool getThreePoints(const QString& Obj_name, const QString& Err_msg);

    void on_actionTriangle_triggered();

    void restoreFromDump();

    void openFromFile(const QString& File_name);

    bool saveToFile();

    QString getbarefilename(const QString& File_name);

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void setTitle(QString new_file_name);

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionNew_triggered();

    void on_actionPrint_triggered();

private:
    Ui::GeoPro *ui;
    Obj_menu *menu;
    std::vector<Point*> threePoints;
    QString cur_file_name = "Untitled";
    QString app_name = "GeoPro";
    bool initially_saved = false;
public:
    GeoBoard *b;
    QImage image;
};

#endif // GEOPRO_H
