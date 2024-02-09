#include "binary.h"
#include <algorithm>

namespace spec {

    template void write(std::ostream &dst, Float val);
    template Float read(std::istream &src);

    void write_vec3(std::ostream &dst, const vec3 &val)
    {
        write<Float>(dst, val.x);
        write<Float>(dst, val.y);
        write<Float>(dst, val.z);
    }

    vec3 read_vec(std::istream &src)
    {
        vec3 v;
        v.x = read<Float>(src);
        v.y = read<Float>(src);
        v.z = read<Float>(src);
        return v;
    }

}