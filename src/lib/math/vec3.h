#ifndef MATH_VEC3_H
#define MATH_VEC3_H
#include <ostream>
#include "math_fwd.h"

namespace spec::math {

    template<typename T = Float>
    union base_vec3
    {
        T v[3];
        struct {
            T x, y, z;
        };

        base_vec3() : x(0.0f), y(0.0f), z(0.0f) {}
        base_vec3(const base_vec3 &v) = default;
        base_vec3(base_vec3 &&v) : x(std::move(v.x)), y(std::move(v.y)), z(std::move(v.z)) {}
        base_vec3(T x, T y, T z) : x(x), y(y), z(z) {}

        base_vec3 &operator=(const base_vec3 &v) = default;

        base_vec3 &operator=(base_vec3 &&v)
        {
            std::swap(x, v.x);
            std::swap(y, v.y);
            std::swap(z, v.z);
            return *this;
        }

        base_vec3 operator+(const base_vec3 &v) const
        {
            base_vec3 c(*this);
            c += v;
            return c;
        }

        base_vec3 operator-(const base_vec3 &v) const
        {
            base_vec3 c(*this);
            c -= v;
            return c;
        }

        base_vec3 operator*(T f) const
        {
            base_vec3 c(*this);
            c *= f;
            return c;
        }

        base_vec3 operator/(T f) const
        {
            base_vec3 c(*this);
            c /= f;
            return c;
        }

        base_vec3 operator*(const base_vec3 &v) const
        {
            base_vec3 c(*this);
            c *= v;
            return c;
        }

        base_vec3 operator/(const base_vec3 &v) const
        {
            base_vec3 c(*this);
            c /= v;
            return c;
        }

        inline T &operator[](int i)
        {
            return v[i];
        }

        inline T operator[](int i) const
        {
            return v[i];
        }

        inline base_vec3 operator-() const
        {
            return {-x, -y, -z};
        }

        base_vec3 &operator+=(const base_vec3 &v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        base_vec3 &operator-=(const base_vec3 &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        base_vec3 &operator*=(T f)
        {
            x *= f;
            y *= f;
            z *= f;
            return *this;
        }

        base_vec3 &operator/=(T f)
        {
            const T invf = 1.0f / f;
            x *= invf;
            y *= invf;
            z *= invf;
            return *this;
        }

        base_vec3 &operator*=(const base_vec3 &v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            return *this;
        }

        base_vec3 &operator/=(const base_vec3 &v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            return *this;
        }

        static T distance2(const base_vec3 &v1, const base_vec3 &v2)
        {
            T dx = v1.x - v2.x;
            T dy = v1.y - v2.y;
            T dz = v1.z - v2.z;

            return dx * dx + dy * dy + dz * dz;
        }

        inline static T distance(const base_vec3 &v1, const base_vec3 &v2)
        {
            return std::sqrt(distance2(v1, v2));
        }

    };

    template<typename T>
    std::ostream &operator<<(std::ostream &str, const base_vec3<T> &v)
    {
        str << '{' << v.x << ", " << v.y << ", " << v.z << "}";
        return str;
    }


    using vec3 = base_vec3<Float>;
}

#endif