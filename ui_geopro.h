/********************************************************************************
** Form generated from reading UI file 'geopro.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GEOPRO_H
#define UI_GEOPRO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuNew_object;
    QMenu *menuConstruct;
    QMenu *menuInfo;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GeoPro)
    {
        if (GeoPro->objectName().isEmpty())
            GeoPro->setObjectName(QStringLiteral("GeoPro"));
        GeoPro->resize(849, 546);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../\320\230\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\321\217/\320\241\320\275\320\270\320\274\320\276\320\272 \321\215\320\272\321\200\320\260\320\275\320\260 \320\276\321\202 2022-01-13 23-40-10.png"), QSize(), QIcon::Normal, QIcon::Off);
        GeoPro->setWindowIcon(icon);
        actionOpen = new QAction(GeoPro);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionOpen->setEnabled(false);
        actionSave = new QAction(GeoPro);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave->setEnabled(false);
        actionSave_as = new QAction(GeoPro);
        actionSave_as->setObjectName(QStringLiteral("actionSave_as"));
        actionSave_as->setEnabled(false);
        actionClose = new QAction(GeoPro);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/resources/img/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose->setIcon(icon1);
        actionNew = new QAction(GeoPro);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionNew->setEnabled(false);
        actionUndo = new QAction(GeoPro);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        actionUndo->setEnabled(false);
        actionRedo = new QAction(GeoPro);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        actionRedo->setEnabled(false);
        actionPoint = new QAction(GeoPro);
        actionPoint->setObjectName(QStringLiteral("actionPoint"));
        actionSegment = new QAction(GeoPro);
        actionSegment->setObjectName(QStringLiteral("actionSegment"));
        actionLine = new QAction(GeoPro);
        actionLine->setObjectName(QStringLiteral("actionLine"));
        actionRay = new QAction(GeoPro);
        actionRay->setObjectName(QStringLiteral("actionRay"));
        actionCircle_by_the_center_radius = new QAction(GeoPro);
        actionCircle_by_the_center_radius->setObjectName(QStringLiteral("actionCircle_by_the_center_radius"));
        actionCircle_by_3_points = new QAction(GeoPro);
        actionCircle_by_3_points->setObjectName(QStringLiteral("actionCircle_by_3_points"));
        actionClear_all = new QAction(GeoPro);
        actionClear_all->setObjectName(QStringLiteral("actionClear_all"));
        actionPrint = new QAction(GeoPro);
        actionPrint->setObjectName(QStringLiteral("actionPrint"));
        actionPrint->setEnabled(false);
        actionPrint->setVisible(true);
        actionDelete_selected_objects = new QAction(GeoPro);
        actionDelete_selected_objects->setObjectName(QStringLiteral("actionDelete_selected_objects"));
        actionIntersection = new QAction(GeoPro);
        actionIntersection->setObjectName(QStringLiteral("actionIntersection"));
        actionMiddle_Center = new QAction(GeoPro);
        actionMiddle_Center->setObjectName(QStringLiteral("actionMiddle_Center"));
        actionAbout = new QAction(GeoPro);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionContact_author = new QAction(GeoPro);
        actionContact_author->setObjectName(QStringLiteral("actionContact_author"));
        centralWidget = new QWidget(GeoPro);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        GeoPro->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GeoPro);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 849, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuNew_object = new QMenu(menuBar);
        menuNew_object->setObjectName(QStringLiteral("menuNew_object"));
        menuConstruct = new QMenu(menuBar);
        menuConstruct->setObjectName(QStringLiteral("menuConstruct"));
        menuInfo = new QMenu(menuBar);
        menuInfo->setObjectName(QStringLiteral("menuInfo"));
        GeoPro->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GeoPro);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mainToolBar->sizePolicy().hasHeightForWidth());
        mainToolBar->setSizePolicy(sizePolicy);
        mainToolBar->setCursor(QCursor(Qt::OpenHandCursor));
        GeoPro->addToolBar(Qt::LeftToolBarArea, mainToolBar);
        statusBar = new QStatusBar(GeoPro);
        statusBar->setObjectName(QStringLiteral("statusBar"));
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
        menuNew_object->addAction(actionPoint);
        menuNew_object->addAction(actionSegment);
        menuNew_object->addAction(actionLine);
        menuNew_object->addAction(actionRay);
        menuNew_object->addAction(actionCircle_by_the_center_radius);
        menuNew_object->addAction(actionCircle_by_3_points);
        menuConstruct->addAction(actionIntersection);
        menuConstruct->addAction(actionMiddle_Center);
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
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionClear_all);

        retranslateUi(GeoPro);
        QObject::connect(actionClose, SIGNAL(triggered()), GeoPro, SLOT(close()));

        QMetaObject::connectSlotsByName(GeoPro);
    } // setupUi

    void retranslateUi(QMainWindow *GeoPro)
    {
        GeoPro->setWindowTitle(QApplication::translate("GeoPro", "GeoPro", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("GeoPro", "&Open", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("GeoPro", "Open project", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("GeoPro", "Ctrl+O", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionSave->setText(QApplication::translate("GeoPro", "&Save", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionSave->setShortcut(QApplication::translate("GeoPro", "Ctrl+S", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionSave_as->setText(QApplication::translate("GeoPro", "Save &as...", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionSave_as->setShortcut(QApplication::translate("GeoPro", "Ctrl+Shift+S", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionClose->setText(QApplication::translate("GeoPro", "&Close", Q_NULLPTR));
        actionNew->setText(QApplication::translate("GeoPro", "&New", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionNew->setToolTip(QApplication::translate("GeoPro", "Create new project", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionUndo->setText(QApplication::translate("GeoPro", "&Undo", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionUndo->setToolTip(QApplication::translate("GeoPro", "Undo last action", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionUndo->setShortcut(QApplication::translate("GeoPro", "Ctrl+Z", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionRedo->setText(QApplication::translate("GeoPro", "&Redo", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionRedo->setShortcut(QApplication::translate("GeoPro", "Ctrl+Shift+Z", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionPoint->setText(QApplication::translate("GeoPro", "&Point", Q_NULLPTR));
        actionSegment->setText(QApplication::translate("GeoPro", "&Segment", Q_NULLPTR));
        actionLine->setText(QApplication::translate("GeoPro", "&Line", Q_NULLPTR));
        actionRay->setText(QApplication::translate("GeoPro", "&Ray", Q_NULLPTR));
        actionCircle_by_the_center_radius->setText(QApplication::translate("GeoPro", "&Circle by the center and point", Q_NULLPTR));
        actionCircle_by_3_points->setText(QApplication::translate("GeoPro", "Circle &by 3 points", Q_NULLPTR));
        actionClear_all->setText(QApplication::translate("GeoPro", "&Clear all", Q_NULLPTR));
        actionPrint->setText(QApplication::translate("GeoPro", "&Print...", Q_NULLPTR));
        actionDelete_selected_objects->setText(QApplication::translate("GeoPro", "&Delete selected objects", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionDelete_selected_objects->setShortcut(QApplication::translate("GeoPro", "Del", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionIntersection->setText(QApplication::translate("GeoPro", "&Intersection", Q_NULLPTR));
        actionMiddle_Center->setText(QApplication::translate("GeoPro", "Middle / Center", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("GeoPro", "About", Q_NULLPTR));
        actionContact_author->setText(QApplication::translate("GeoPro", "Contact author", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("GeoPro", "&File", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("GeoPro", "&Edit", Q_NULLPTR));
        menuNew_object->setTitle(QApplication::translate("GeoPro", "&New object...", Q_NULLPTR));
        menuConstruct->setTitle(QApplication::translate("GeoPro", "&Construct...", Q_NULLPTR));
        menuInfo->setTitle(QApplication::translate("GeoPro", "&Info", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GeoPro: public Ui_GeoPro {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GEOPRO_H
