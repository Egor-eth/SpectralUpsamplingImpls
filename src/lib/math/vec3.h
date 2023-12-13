#ifndef MATH_VEC3_H
#define MATH_VEC3_H

#include <ostream>
#include "math_fwd.h"
namespace math {

    union vec3
    {
        Float v[3];
        struct {
            Float x, y, z;
        };

        vec3() : x(0.0f), y(0.0f), z(0.0f) {}
        vec3(const vec3 &v) = default;
        vec3(Float x, Float y, Float z) : x(x), y(y), z(z) {}

        vec3 &operator=(const vec3 &v) = default;

        vec3 operator-() const;

        vec3 &operator+=(const vec3 &v);
        vec3 &operator-=(const vec3 &v);

        vec3 &operator*=(Float f);
        vec3 &operator/=(Float f);

        vec3 operator+(const vec3 &v) const
        {
            vec3 c(*this);
            c += v;
            return c;
        }

        vec3 operator-(const vec3 &v) const
        {
            vec3 c(*this);
            c -= v;
            return c;
        }

        vec3 operator*(Float f) const
        {
            vec3 c(*this);
            c *= f;
            return c;
        }

        vec3 operator/(Float f) const
        {
            vec3 c(*this);
            c /= f;
            return c;
        }

        inline Float &operator[](int i)
        {
            return v[i];
        }

        inline Float operator[](int i) const
        {
            return v[i];
        }

    };

    std::ostream &operator<<(std::ostream &str, const vec3 &v);

}

#endif