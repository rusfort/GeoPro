#include "grwidget.h"

#include <iostream>
#include <QtGui>
#include <QWidget>
#include <QCoreApplication>

#include "testgraph.h"
#include "geometry_main.h"

#include <math.h>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    SIZE_X = 2000;
    SIZE_Y = 2000;
    scene->setSceneRect(-SIZE_X/2, -SIZE_Y/2, SIZE_X, SIZE_Y);
    //scene->setSceneRect(0, 0, parent->size().width(), parent->size().height());
    setScene(scene);
    scale(2, 2);
    setCacheMode(CacheBackground);
    //setViewportUpdateMode(BoundingRectViewportUpdate);
    setViewportUpdateMode(MinimalViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        //centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        //centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        //centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        //centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        scaleView(qreal(1.2));
        break;
    case Qt::Key_Minus:
        scaleView(1 / qreal(1.2));
        break;
    case Qt::Key_Space:
        break;
    case Qt::Key_Enter:
        /*foreach (QGraphicsItem *item, scene()->items()) {
            if (qgraphicsitem_cast<Node *>(item))
                item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
        }*/
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    foreach (Node *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, event->delta() / 240.0));
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    Q_UNUSED(painter);

    // Shadow
    /*QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);*/

    // Fill
    /*QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    //painter->fillRect(rect.intersect(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);*/

    // Text
    /*QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr("Click and drag the nodes around, and zoom with the mouse "
                       "wheel or the '+' and '-' keys"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);*/
}

/*void GraphWidget::mouseMoveEvent(QMouseEvent *event)
{
    QList<QGraphicsScene *> scenes = findChildren<QGraphicsScene *>();
    foreach (QGraphicsItem *item, scenes[0]->items()) {
        if (qgraphicsitem_cast<Point *>(item)){
            auto p = qgraphicsitem_cast<Point *>(item);
            if (p->kbm){
                auto POS = mapFromGlobal(event->localPos().toPoint());
                p->setPos(POS.x(), POS.y());
                //p->mouseMoveEvent(reinterpret_cast<QGraphicsSceneMouseEvent *>(event));
                //p->update();
                break;
            }
        }
    }
    //update();
}*/

/*void GraphWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    std::cout << "ddd\n" << std::endl;
    //QList<QGraphicsScene *> scenes = findChildren<QGraphicsScene *>();
    //QCoreApplication::notify(scenes[0], event);
    foreach (QGraphicsItem *item, scenes[0]->items()) {
        if (qgraphicsitem_cast<Point *>(item)){
            auto p = qgraphicsitem_cast<Point *>(item);
            if (p->kbm){
                auto POS = mapFromGlobal(event->localPos().toPoint());
                p->setPos(POS.x(), POS.y());
                //p->mouseMoveEvent(reinterpret_cast<QGraphicsSceneMouseEvent *>(event));
                //p->update();
                break;
            }
        }

    }

    //QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMousePress);
    if (itemAt(mouseEvent) == NULL)
    {
        //
    } else {
    QList<QGraphicsScene *> scenes = findChildren<QGraphicsScene *>();
    scenes[0]->mousePressEvent(mouseEvent);
    }
}*/

/*void GraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    QList<QGraphicsScene *> scenes = findChildren<QGraphicsScene *>();
    foreach (QGraphicsItem *item, scenes[0]->items()) {
        if (qgraphicsitem_cast<Point *>(item)){
            auto p = qgraphicsitem_cast<Point *>(item);
            if (p->kbm){
                p->kbm = false;
                //break;
            }
        }
        item->update();
    }
    //update();
}*/

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.4 || factor > 20)
        return;

    scale(scaleFactor, scaleFactor);
}

/*void GraphWidget::resizeEvent(QResizeEvent* event)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, event->size().width(), event->size().height());
    setScene(scene);
}*/
