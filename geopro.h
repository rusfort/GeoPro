#ifndef GEOPRO_H
#define GEOPRO_H

#include <QMainWindow>

namespace Ui {
class GeoPro;
}

class GeoPro : public QMainWindow
{
    Q_OBJECT

public:
    explicit GeoPro(QWidget *parent = 0);
    ~GeoPro();

private:
    Ui::GeoPro *ui;
};

#endif // GEOPRO_H
