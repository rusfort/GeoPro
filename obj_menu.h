#ifndef OBJ_MENU_H
#define OBJ_MENU_H

#include <QDialog>

namespace Ui {
class Obj_menu;
}

class Obj_menu : public QDialog
{
    Q_OBJECT

public:
    explicit Obj_menu(QWidget *parent = nullptr);
    QString getContent() {return content;}
    ~Obj_menu();

private slots:
    void on_Name_Ok_Button_clicked();

private:
    Ui::Obj_menu *ui;
    QString content;
};

#endif // OBJ_MENU_H
