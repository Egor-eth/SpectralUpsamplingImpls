#ifndef COMMON_MATH_H
#define COMMON_MATH_H
#include "constants.h"

using glm::vec3;
using glm::mat3x3; 

inline Float interpolate(Float point, Float a, Float b, Float f_a, Float f_b)
{
    return (f_b - f_a) * (point - a) / (b - a);
}

#endif