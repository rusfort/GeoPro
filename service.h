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
    Middle
};

#endif // SERVICE_H
