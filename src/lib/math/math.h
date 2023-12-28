#ifndef MATH_MATH_H
#define MATH_MATH_H
#include "math_fwd.h"
#include "vec3.h"
#include "mat3.h"
#include <cmath>

namespace spec {

    namespace math {

        inline Float interpolate(Float point, Float a, Float b, Float f_a, Float f_b)
        {
            return f_a + (f_b - f_a) * (point - a) / (b - a);
        }

        Float clamp(Float x, Float a, Float b);
        vec3 clamp(const vec3 &x, Float a, Float b);

        inline Float sigmoid(Float x)
        {
            return std::fma(0.5, x / std::sqrt(std::fma(x, x, 1)), 0.5);
        }

        inline Float sigmoid_polynomial(Float x, const Float coef[3])
        {
            return sigmoid(std::fma(std::fma(coef[0], x, coef[1]), x, coef[2]));
        }
    }

    using math::mat3;
    using math::vec3;

}

#endif