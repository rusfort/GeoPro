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
    Bisector,       //for non-free rays
    Angle
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
