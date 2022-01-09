#include "geometry_main.h"

Point::Point(double x0, double y0, DrStyle st0):
    x(x0), y(y0), style(st0){}

Line::Line(double x01, double y01, double x02, double y02, DrStyle st0):
    x1(x01), y1(y01), x2(x02), y2(y02), style(st0){}
