#ifndef MATH_MATH_H
#define MATH_MATH_H
#include "math_fwd.h"
#include "vec3.h"
#include "mat3.h"

using math::mat3;
using math::vec3;

inline Float interpolate(Float point, Float a, Float b, Float f_a, Float f_b)
{
    return (f_b - f_a) * (point - a) / (b - a);
}

#endif