#ifndef SERIALIZATION_BINARY_H
#define SERIALIZATION_BINARY_H
#include <ostream>
#include <istream>
#include "math/math.h"
#include "common/util.h"

namespace spec::binary {

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>, void>>
    void write(std::ostream &dst, const T val)
    {
        char buf[sizeof(T)];
        spec::serial_copy(reinterpret_cast<const char *>(&val), buf, sizeof(T));
        dst.write(buf, sizeof(T));
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>, void>>
    T read(std::istream &src)
    {
        char buf[sizeof(T)];
        T val;
        src.read(buf, sizeof(T));
        spec::serial_copy(buf, reinterpret_cast<char *>(&val), sizeof(T));
        return val;
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>, void>>
    void write_vec(std::ostream &dst, const math::base_vec3<T> &val)
    {
        write<T>(dst, val.x);
        write<T>(dst, val.y);
        write<T>(dst, val.z);
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>, void>>
    math::base_vec3<T> read_vec(std::istream &src)
    {
        math::base_vec3<T> v;
        v.x = read<T>(src);
        v.y = read<T>(src);
        v.z = read<T>(src);
        return v;
    }


    extern template void write(std::ostream &dst, Float val);
    extern template Float read(std::istream &src);
    extern template void write_vec(std::ostream &dst, const vec3 &val);
    extern template vec3 read_vec(std::istream &src);

}

#endif