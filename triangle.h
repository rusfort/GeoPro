#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "geometry_main.h"

class Triangle : public GOBJ
{
    Q_OBJECT
public:
    Triangle(GeoBoard* board, Point* p1, Point* p2, Point* p3); //classic (3 Points)
    virtual ~Triangle();
    void draw() override;
    bool isCaught(QPointF p) override;
    void move(QPointF newPos) override;
    void changeView() override;
    void recalculate() override;
    qreal a() const;
    qreal b() const;
    qreal c() const;
    qreal alpha() const;
    qreal beta() const;
    qreal gamma() const;
    qreal perimeter() const {return a() + b() + c();}
    qreal area() const;
    Point*  manage_bis()   {return bis_i;}
    Point*  manage_med()   {return med_i;}
    Point*  manage_hgt()   {return hgt_i;}
    Point*  manage_mdp()   {return mdp_i;}
    Point*  manage_eul()   {return eul_c;}
    Circle* manage_in ()   {return in;}
    Circle* manage_cir()   {return cir;}
    Circle* manage_Euler() {return Euler;}
    Angle*  manage_alpha() {return _alpha;}
    Angle*  manage_beta()  {return _beta;}
    Angle*  manage_gamma() {return _gamma;}
signals:
    void posChanged();
private:
    Point *mP1, *mP2, *mP3;
    Point *bis_i;  //bisectors intersection (inscribed circle center)
    Point *med_i;  //medians intersection
    Point *hgt_i;  //heights intersection
    Point *mdp_i;  //midperpendiculars intersection (circumscribed circle center)
    Point *eul_c;  //Euler circle center
    Segment *edge_a = 0;
    Segment *edge_b = 0;
    Segment *edge_c = 0;
    Circle *in;    //inscribed circle
    Circle *cir;   //circumscribed circle
    Circle *Euler; //Euler circle
    Angle *_alpha;
    Angle *_beta;
    Angle *_gamma;
};

#endif // TRIANGLE_H
