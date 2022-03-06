#ifndef SERVICE_H
#define SERVICE_H

#include <vector>
#include <map>
#include <assert.h>

enum class DrStyle{
    None,
    Light,
    Bold
};

enum class GObj_Type{
    NONE,
    POINT,
    LINE,
    RAY,
    SEGMENT,
    CIRCLE,
    ANGLE,
    TRIANGLE
};

enum class Child_Type{
    Unknown,
    OnTwoPoints,
    OnThreePoints,
    Middle,
    Intersection,
    Intersection2, //for circles
    OnLine,        //for non-free points
    OnRay,         //for non-free points
    OnSegment,     //for non-free points
    OnCircle,      //for non-free points
    Parallel,      //for non-free lines
    Perpendicular, //for non-free lines
    Bisector,      //for non-free rays
    Angle,
    Triangle,
    InTriangle     //for objects in triangles
};

enum class Intersection_Type{
    None,
    Line_Line,
    Line_Ray,
    Line_Segment,
    Line_Circle,
    Ray_Ray,
    Ray_Segment,
    Ray_Circle,
    Segment_Segment,
    Segment_Circle,
    Circle_Circle
};

enum class Triangle_Obj{
    Not_in_Triangle,
    I,
    M,
    H,
    O,
    E,
    In_C,
    Cir_C,
    Eul_C,
    Ed_a,
    Ed_b,
    Ed_c,
    An_alp,
    An_bet,
    An_gam
};

#endif // SERVICE_H
