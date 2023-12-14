#ifndef MATH_MATH_H
#define MATH_MATH_H
#include "math_fwd.h"
#include "vec3.h"
#include "mat3.h"
#include <cmath>

namespace math {

    inline Float interpolate(Float point, Float a, Float b, Float f_a, Float f_b)
    {
        return f_a + (f_b - f_a) * (point - a) / (b - a);
    }

    Float clamp(Float x, Float a, Float b);
    vec3 clamp(const vec3 &x, Float a, Float b);

    template<typename T>
    T sigmoid(T x)
    {
        return 0.5 + x / (2 * std::sqrt(1 + x * x));
    }
}

using math::mat3;
using math::vec3;


#endif