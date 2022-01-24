#include "widget.h"
#include <QPainter>
#include <iostream>

//GeoBoard::GeoBoard(QWidget *parent): QWidget(parent) {}

void GeoBoard::paintEvent(QPaintEvent*)
{
    QPainter p;
    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(0, 0, this->width(), this->height(), mColor);
    for(auto* obj : mObjects)
        obj->draw();
}

void GeoBoard::mousePressEvent(QMouseEvent* e)
{
    for(auto obj : mObjects)
        obj->setSelected(obj->isCatched(e->pos()));
    if(trytoadd == GObj_Type::NONE) return;

    switch (trytoadd) {
    case GObj_Type::POINT:
    {
        auto Pos1 = e->pos();
        Point *p1 = new Point(this, Pos1.x(), Pos1.y(), 5);
        std::cout << "LOL\n" << std::endl;
        addObject(p1);
        numitemstoadd--;
        update();
    }
        break;
    case GObj_Type::LINE:
    {
        //
    }
        break;
    case GObj_Type::SEGMENT:
    {
        //
    }
        break;
    default:
        break;
    }
    if (numitemstoadd == 0) trytoadd = GObj_Type::NONE;
}

void GeoBoard::mouseMoveEvent(QMouseEvent* e)
{
    for(auto obj : mObjects)
    {
        if(obj->isSelected())
            obj->move(e->pos());
    }
}

GeoObject::GeoObject(GeoBoard* board, QColor color) :
    mColor(color), mBoard(board), mIsSelected(false)
{
    connect(this, SIGNAL(selectionChanged()), mBoard, SLOT(update()));
}




GeoPoint::GeoPoint(GeoBoard* board, double x, double y, double radius,  QColor color) :
    GeoObject(board, color), QPointF(x, y), mRadius(radius)
{
    connect(this, SIGNAL(posChanged()), mBoard, SLOT(update()));
}

GeoPoint::GeoPoint(const GeoPoint& copyFrom) :
    GeoObject(copyFrom.board(), copyFrom.color()), QPointF(copyFrom.x(), copyFrom.y()), mRadius(copyFrom.rad())
{

}

void GeoPoint::draw()
{
    QPainter p;
    p.begin(this->board());
    p.setRenderHint(QPainter::Antialiasing);
    if(mIsSelected)
    {
        QPen pen(Qt::blue);
        pen.setWidth(2);
        p.setPen(pen);
    }
    p.setBrush(QBrush(mColor));
    p.drawEllipse(*this, mRadius, mRadius);
}

bool GeoPoint::isCatched(QPointF p)
{
    double dx = rx() - p.x();
    double dy = ry() - p.y();
    return dx * dx + dy * dy - mRadius * mRadius < 0;
}


GeoLine::GeoLine(GeoBoard* board, GeoPoint* p1, GeoPoint* p2) :
    GeoObject(board, p1->color()), mP1(p1), mP2(p2)
{

}

void GeoLine::draw()
{
    double k = mBoard->width() > mBoard->height() ? mBoard->width() : mBoard->height();
    QPointF dr = *mP1 - *mP2;
    QPointF p1 = *mP1 + k * dr;
    QPointF p2 = *mP1 - k * dr;

    QPainter p;
    p.begin(mBoard);
     p.setRenderHint(QPainter::Antialiasing);
    p.setPen(mColor);
    p.drawLine(p1, p2);
}


bool GeoLine::isCatched(QPointF p)
{
    return false;
}


void GeoLine::move(QPointF newPos)
{
    return;
}


