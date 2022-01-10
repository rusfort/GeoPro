#include "geometry_main.h"

STYLE::STYLE(DrStyle st0): style(st0){}
STYLE::~STYLE(){}

Point::Point(double x0, double y0, DrStyle st0):
    STYLE(st0), x(x0), y(y0) {}

Point::~Point(){}

Line::Line(double x01, double y01, double x02, double y02, DrStyle st0):
    STYLE(st0), p1(x01, y01), p2(x02, y02){}
