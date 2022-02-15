/********************************************************************************
** Form generated from reading UI file 'geopro.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GEOPRO_H
#define UI_GEOPRO_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GeoPro
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *actionClose;
    QAction *actionNew;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionPoint;
    QAction *actionSegment;
    QAction *actionLine;
    QAction *actionRay;
    QAction *actionCircle_by_the_center_radius;
    QAction *actionCircle_by_3_points;
    QAction *actionClear_all;
    QAction *actionPrint;
    QAction *actionDelete_selected_objects;
    QAction *actionIntersection;
    QAction *actionMiddle_Center;
    QAction *actionAbout;
    QAction *actionContact_author;
    QAction *actionHide_selected_objects;
    QAction *actionShow_all_hidden_objects;
    QAction *actionParallel_line;
    QAction *actionPerpendicular_line;
    QAction *actionBisector;
    QAction *actionWhite;
    QAction *actionGray;
    QAction *actionGrid;
    QAction *actionBack_to_original_view;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuBackground;
    QMenu *menuNew_object;
    QMenu *menuConstruct;
    QMenu *menuInfo;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GeoPro)
    {
        if (GeoPro->objectName().isEmpty())
            GeoPro->setObjectName(QString::fromUtf8("GeoPro"));
        GeoPro->resize(849, 546);
        GeoPro->setCursor(QCursor(Qt::ArrowCursor));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../\320\230\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\321\217/\320\241\320\275\320\270\320\274\320\276\320\272 \321\215\320\272\321\200\320\260\320\275\320\260 \320\276\321\202 2022-01-13 23-40-10.png"), QSize(), QIcon::Normal, QIcon::Off);
        GeoPro->setWindowIcon(icon);
        actionOpen = new QAction(GeoPro);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionOpen->setEnabled(false);
        actionSave = new QAction(GeoPro);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave->setEnabled(false);
        actionSave_as = new QAction(GeoPro);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        actionSave_as->setEnabled(false);
        actionClose = new QAction(GeoPro);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/img/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose->setIcon(icon1);
        actionNew = new QAction(GeoPro);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionNew->setEnabled(false);
        actionUndo = new QAction(GeoPro);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionUndo->setEnabled(false);
        actionRedo = new QAction(GeoPro);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionRedo->setEnabled(false);
        actionPoint = new QAction(GeoPro);
        actionPoint->setObjectName(QString::fromUtf8("actionPoint"));
        actionSegment = new QAction(GeoPro);
        actionSegment->setObjectName(QString::fromUtf8("actionSegment"));
        actionLine = new QAction(GeoPro);
        actionLine->setObjectName(QString::fromUtf8("actionLine"));
        actionRay = new QAction(GeoPro);
        actionRay->setObjectName(QString::fromUtf8("actionRay"));
        actionCircle_by_the_center_radius = new QAction(GeoPro);
        actionCircle_by_the_center_radius->setObjectName(QString::fromUtf8("actionCircle_by_the_center_radius"));
        actionCircle_by_3_points = new QAction(GeoPro);
        actionCircle_by_3_points->setObjectName(QString::fromUtf8("actionCircle_by_3_points"));
        actionClear_all = new QAction(GeoPro);
        actionClear_all->setObjectName(QString::fromUtf8("actionClear_all"));
        actionClear_all->setShortcutContext(Qt::WindowShortcut);
        actionPrint = new QAction(GeoPro);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        actionPrint->setEnabled(false);
        actionPrint->setVisible(true);
        actionDelete_selected_objects = new QAction(GeoPro);
        actionDelete_selected_objects->setObjectName(QString::fromUtf8("actionDelete_selected_objects"));
        actionIntersection = new QAction(GeoPro);
        actionIntersection->setObjectName(QString::fromUtf8("actionIntersection"));
        actionMiddle_Center = new QAction(GeoPro);
        actionMiddle_Center->setObjectName(QString::fromUtf8("actionMiddle_Center"));
        actionAbout = new QAction(GeoPro);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionContact_author = new QAction(GeoPro);
        actionContact_author->setObjectName(QString::fromUtf8("actionContact_author"));
        actionHide_selected_objects = new QAction(GeoPro);
        actionHide_selected_objects->setObjectName(QString::fromUtf8("actionHide_selected_objects"));
        actionShow_all_hidden_objects = new QAction(GeoPro);
        actionShow_all_hidden_objects->setObjectName(QString::fromUtf8("actionShow_all_hidden_objects"));
        actionParallel_line = new QAction(GeoPro);
        actionParallel_line->setObjectName(QString::fromUtf8("actionParallel_line"));
        actionPerpendicular_line = new QAction(GeoPro);
        actionPerpendicular_line->setObjectName(QString::fromUtf8("actionPerpendicular_line"));
        actionBisector = new QAction(GeoPro);
        actionBisector->setObjectName(QString::fromUtf8("actionBisector"));
        actionWhite = new QAction(GeoPro);
        actionWhite->setObjectName(QString::fromUtf8("actionWhite"));
        actionGray = new QAction(GeoPro);
        actionGray->setObjectName(QString::fromUtf8("actionGray"));
        actionGrid = new QAction(GeoPro);
        actionGrid->setObjectName(QString::fromUtf8("actionGrid"));
        actionBack_to_original_view = new QAction(GeoPro);
        actionBack_to_original_view->setObjectName(QString::fromUtf8("actionBack_to_original_view"));
        centralWidget = new QWidget(GeoPro);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        GeoPro->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GeoPro);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 849, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuBackground = new QMenu(menuEdit);
        menuBackground->setObjectName(QString::fromUtf8("menuBackground"));
        menuNew_object = new QMenu(menuBar);
        menuNew_object->setObjectName(QString::fromUtf8("menuNew_object"));
        menuConstruct = new QMenu(menuBar);
        menuConstruct->setObjectName(QString::fromUtf8("menuConstruct"));
        menuInfo = new QMenu(menuBar);
        menuInfo->setObjectName(QString::fromUtf8("menuInfo"));
        GeoPro->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GeoPro);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mainToolBar->sizePolicy().hasHeightForWidth());
        mainToolBar->setSizePolicy(sizePolicy);
        mainToolBar->setCursor(QCursor(Qt::OpenHandCursor));
        mainToolBar->setMovable(false);
        mainToolBar->setFloatable(false);
        GeoPro->addToolBar(Qt::LeftToolBarArea, mainToolBar);
        statusBar = new QStatusBar(GeoPro);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        GeoPro->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuNew_object->menuAction());
        menuBar->addAction(menuConstruct->menuAction());
        menuBar->addAction(menuInfo->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_as);
        menuFile->addAction(actionPrint);
        menuFile->addAction(actionClose);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addAction(actionDelete_selected_objects);
        menuEdit->addAction(actionClear_all);
        menuEdit->addAction(actionHide_selected_objects);
        menuEdit->addAction(actionShow_all_hidden_objects);
        menuEdit->addAction(menuBackground->menuAction());
        menuEdit->addAction(actionBack_to_original_view);
        menuBackground->addSeparator();
        menuBackground->addAction(actionWhite);
        menuBackground->addAction(actionGray);
        menuBackground->addAction(actionGrid);
        menuNew_object->addAction(actionPoint);
        menuNew_object->addAction(actionSegment);
        menuNew_object->addAction(actionLine);
        menuNew_object->addAction(actionRay);
        menuNew_object->addAction(actionCircle_by_the_center_radius);
        menuNew_object->addAction(actionCircle_by_3_points);
        menuConstruct->addAction(actionIntersection);
        menuConstruct->addAction(actionMiddle_Center);
        menuConstruct->addAction(actionParallel_line);
        menuConstruct->addAction(actionPerpendicular_line);
        menuConstruct->addAction(actionBisector);
        menuInfo->addAction(actionAbout);
        menuInfo->addAction(actionContact_author);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionPoint);
        mainToolBar->addAction(actionSegment);
        mainToolBar->addAction(actionLine);
        mainToolBar->addAction(actionRay);
        mainToolBar->addAction(actionCircle_by_the_center_radius);
        mainToolBar->addAction(actionCircle_by_3_points);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionIntersection);
        mainToolBar->addAction(actionMiddle_Center);
        mainToolBar->addAction(actionParallel_line);
        mainToolBar->addAction(actionPerpendicular_line);
        mainToolBar->addAction(actionBisector);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionHide_selected_objects);
        mainToolBar->addAction(actionShow_all_hidden_objects);
        mainToolBar->addAction(actionClear_all);
        mainToolBar->addAction(actionBack_to_original_view);

        retranslateUi(GeoPro);
        QObject::connect(actionClose, SIGNAL(triggered()), GeoPro, SLOT(close()));

        QMetaObject::connectSlotsByName(GeoPro);
    } // setupUi

    void retranslateUi(QMainWindow *GeoPro)
    {
        GeoPro->setWindowTitle(QApplication::translate("GeoPro", "GeoPro", nullptr));
        actionOpen->setText(QApplication::translate("GeoPro", "&Open", nullptr));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("GeoPro", "Open project", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("GeoPro", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionSave->setText(QApplication::translate("GeoPro", "&Save", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSave->setShortcut(QApplication::translate("GeoPro", "Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionSave_as->setText(QApplication::translate("GeoPro", "Save &as...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSave_as->setShortcut(QApplication::translate("GeoPro", "Ctrl+Shift+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionClose->setText(QApplication::translate("GeoPro", "&Close", nullptr));
#ifndef QT_NO_SHORTCUT
        actionClose->setShortcut(QApplication::translate("GeoPro", "Ctrl+Esc", nullptr));
#endif // QT_NO_SHORTCUT
        actionNew->setText(QApplication::translate("GeoPro", "&New", nullptr));
#ifndef QT_NO_TOOLTIP
        actionNew->setToolTip(QApplication::translate("GeoPro", "Create new project", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionNew->setShortcut(QApplication::translate("GeoPro", "Ctrl+N", nullptr));
#endif // QT_NO_SHORTCUT
        actionUndo->setText(QApplication::translate("GeoPro", "&Undo", nullptr));
#ifndef QT_NO_TOOLTIP
        actionUndo->setToolTip(QApplication::translate("GeoPro", "Undo last action", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionUndo->setShortcut(QApplication::translate("GeoPro", "Ctrl+Z", nullptr));
#endif // QT_NO_SHORTCUT
        actionRedo->setText(QApplication::translate("GeoPro", "&Redo", nullptr));
#ifndef QT_NO_SHORTCUT
        actionRedo->setShortcut(QApplication::translate("GeoPro", "Ctrl+Shift+Z", nullptr));
#endif // QT_NO_SHORTCUT
        actionPoint->setText(QApplication::translate("GeoPro", "&Point", nullptr));
#ifndef QT_NO_SHORTCUT
        actionPoint->setShortcut(QApplication::translate("GeoPro", "P", nullptr));
#endif // QT_NO_SHORTCUT
        actionSegment->setText(QApplication::translate("GeoPro", "&Segment", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSegment->setShortcut(QApplication::translate("GeoPro", "S", nullptr));
#endif // QT_NO_SHORTCUT
        actionLine->setText(QApplication::translate("GeoPro", "&Line", nullptr));
#ifndef QT_NO_SHORTCUT
        actionLine->setShortcut(QApplication::translate("GeoPro", "L", nullptr));
#endif // QT_NO_SHORTCUT
        actionRay->setText(QApplication::translate("GeoPro", "&Ray", nullptr));
#ifndef QT_NO_SHORTCUT
        actionRay->setShortcut(QApplication::translate("GeoPro", "R", nullptr));
#endif // QT_NO_SHORTCUT
        actionCircle_by_the_center_radius->setText(QApplication::translate("GeoPro", "&Circle by the center and point", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCircle_by_the_center_radius->setShortcut(QApplication::translate("GeoPro", "C", nullptr));
#endif // QT_NO_SHORTCUT
        actionCircle_by_3_points->setText(QApplication::translate("GeoPro", "Circle &by 3 points", nullptr));
        actionClear_all->setText(QApplication::translate("GeoPro", "&Clear all", nullptr));
#ifndef QT_NO_SHORTCUT
        actionClear_all->setShortcut(QApplication::translate("GeoPro", "Ctrl+Del", nullptr));
#endif // QT_NO_SHORTCUT
        actionPrint->setText(QApplication::translate("GeoPro", "&Print...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionPrint->setShortcut(QApplication::translate("GeoPro", "Ctrl+P", nullptr));
#endif // QT_NO_SHORTCUT
        actionDelete_selected_objects->setText(QApplication::translate("GeoPro", "&Delete selected objects", nullptr));
#ifndef QT_NO_SHORTCUT
        actionDelete_selected_objects->setShortcut(QApplication::translate("GeoPro", "Del", nullptr));
#endif // QT_NO_SHORTCUT
        actionIntersection->setText(QApplication::translate("GeoPro", "&Intersection", nullptr));
#ifndef QT_NO_SHORTCUT
        actionIntersection->setShortcut(QApplication::translate("GeoPro", "Ctrl+I", nullptr));
#endif // QT_NO_SHORTCUT
        actionMiddle_Center->setText(QApplication::translate("GeoPro", "Middle / Center", nullptr));
#ifndef QT_NO_SHORTCUT
        actionMiddle_Center->setShortcut(QApplication::translate("GeoPro", "Ctrl+M", nullptr));
#endif // QT_NO_SHORTCUT
        actionAbout->setText(QApplication::translate("GeoPro", "About", nullptr));
        actionContact_author->setText(QApplication::translate("GeoPro", "Contact author", nullptr));
        actionHide_selected_objects->setText(QApplication::translate("GeoPro", "Hide selected objects", nullptr));
#ifndef QT_NO_SHORTCUT
        actionHide_selected_objects->setShortcut(QApplication::translate("GeoPro", "Ctrl+H", nullptr));
#endif // QT_NO_SHORTCUT
        actionShow_all_hidden_objects->setText(QApplication::translate("GeoPro", "Show all hidden objects", nullptr));
#ifndef QT_NO_SHORTCUT
        actionShow_all_hidden_objects->setShortcut(QApplication::translate("GeoPro", "Ctrl+Shift+H", nullptr));
#endif // QT_NO_SHORTCUT
        actionParallel_line->setText(QApplication::translate("GeoPro", "Parallel line", nullptr));
        actionPerpendicular_line->setText(QApplication::translate("GeoPro", "Perpendicular line", nullptr));
        actionBisector->setText(QApplication::translate("GeoPro", "Bisector", nullptr));
        actionWhite->setText(QApplication::translate("GeoPro", "White", nullptr));
        actionGray->setText(QApplication::translate("GeoPro", "Gray", nullptr));
        actionGrid->setText(QApplication::translate("GeoPro", "Grid", nullptr));
        actionBack_to_original_view->setText(QApplication::translate("GeoPro", "Back to original view", nullptr));
        menuFile->setTitle(QApplication::translate("GeoPro", "&File", nullptr));
        menuEdit->setTitle(QApplication::translate("GeoPro", "&Edit", nullptr));
        menuBackground->setTitle(QApplication::translate("GeoPro", "Background...", nullptr));
        menuNew_object->setTitle(QApplication::translate("GeoPro", "&New object...", nullptr));
        menuConstruct->setTitle(QApplication::translate("GeoPro", "&Construct...", nullptr));
        menuInfo->setTitle(QApplication::translate("GeoPro", "&Info", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GeoPro: public Ui_GeoPro {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GEOPRO_H
