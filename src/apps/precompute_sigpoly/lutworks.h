#ifndef LUTWORKS_H
#define LUTWORKS_H
#include "functions.h"
#include <istream>
#include <ostream>
#include <cinttypes>

class LUT
{
public:
    LUT(unsigned size) : size{size}, data{new vec3[size * size]} {}

    LUT(const LUT &) = delete;
    LUT &operator=(const LUT &) = delete;

    LUT(LUT &&l) : size{l.size}, data{}
    {
        std::swap(data, l.data);
    }

    ~LUT()
    {
        delete[] data;
    }

    unsigned get_size() const
    {
        return size;
    }

    vec3 &at(int i, int j)
    {
        return data[i * size + j];
    }

    const vec3 &at(int i, int j) const
    {
        return data[i * size + j];
    }

private:
    const unsigned size;
    vec3 *data;
};

void write_header(std::ostream &dst);
void write_lut(std::ostream &dst, const LUT &lut);

bool validate_header(std::istream &src);

LUT generate_lut(int zeroed_idx, int step = 5, int stable_val = 25);

#endif