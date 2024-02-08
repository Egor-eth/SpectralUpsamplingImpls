#ifndef MATH_MATH_H
#define MATH_MATH_H
#include "math_fwd.h"
#include "vec3.h"
#include "mat3.h"

namespace spec {

    namespace math {

        inline Float interpolate(Float point, Float a, Float b, Float f_a, Float f_b)
        {
            return f_a + (f_b - f_a) * (point - a) / (b - a);
        }

        Float clamp(Float x, Float a, Float b);
        vec3 clamp(const vec3 &x, Float a, Float b);

        Float sigmoid_polynomial(Float x, const Float coef[3]);
    }

    using math::mat3;
    using math::vec3;
    using math::vec3d;

}

#endif