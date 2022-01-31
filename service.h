#ifndef SERVICE_H
#define SERVICE_H

#include <vector>
#include <map>

enum class DrStyle{
    None,
    Light,
    Bold
};

enum class GObj_Type{
    NONE,
    POINT,
    LINE,
    SEGMENT,
    RAY,
    CIRCLE
};

enum class Child_Type{
    Unknown,
    OnTwoPoints,
    OnThreePoints,
    Middle,
    Intersection
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

#endif // SERVICE_H
