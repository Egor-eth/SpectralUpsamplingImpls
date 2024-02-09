#ifndef SERIALIZATION_BINARY_H
#define SERIALIZATION_BINARY_H
#include <ostream>
#include <istream>
#include "math/math.h"
#include "common/util.h"

namespace spec {

    template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value, void>>
    void write(std::ostream &dst, T val)
    {
        char buf[sizeof(T)];
        spec::serial_copy(reinterpret_cast<const char *>(&val), buf, sizeof(T));
        dst.write(buf, sizeof(T));
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value, void>>
    T read(std::istream &src)
    {
        char buf[sizeof(T)];
        T val;
        src.read(buf, sizeof(T));
        spec::serial_copy(buf, reinterpret_cast<char *>(&val), sizeof(T));
        return val;
    }

    extern template void write(std::ostream &dst, Float val);
    extern template Float read(std::istream &src);

    void write_vec3(std::ostream &dst, const vec3 &val);

    vec3 read_vec(std::ostream &src);

}

#endif