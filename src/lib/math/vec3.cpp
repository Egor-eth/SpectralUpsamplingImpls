#include "vec3.h" 

namespace spec {

    namespace math {

        vec3 vec3::operator-() const
        {
            return {-x, -y, -z};
        }

        vec3 &vec3::operator+=(const vec3 &v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        vec3 &vec3::operator-=(const vec3 &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        vec3 &vec3::operator*=(Float f)
        {
            x *= f;
            y *= f;
            z *= f;
            return *this;
        }

        vec3 &vec3::operator/=(Float f)
        {
            const Float invf = 1.0f / f;
            x *= invf;
            y *= invf;
            z *= invf;
            return *this;
        }

        std::ostream &operator<<(std::ostream &str, const vec3 &v)
        {
            str << '{' << v.x << ", " << v.y << ", " << v.z << "}";
            return str;
        }

    }

}