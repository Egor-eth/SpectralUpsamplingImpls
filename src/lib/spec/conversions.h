#ifndef COLOR_CONVERSIONS_H
#define COLOR_CONVERSIONS_H
#include "math/math.h"
#include "spec/basic_spectrum.h"

namespace spec {

    inline vec3 rgb2xyz(const vec3 &rgb)
    {
        return RGB_TO_XYZ * rgb;
    }

    inline vec3 xyz2rgb(const vec3 &xyz)
    {
        return math::clamp(XYZ_TO_RGB * xyz, 0.0f, 1.0f);
    }

    vec3 xyz2cielab(const vec3 &xyz);

    vec3 spectre2xyz(const ISpectrum &spectre);

    vec3 sigpoly2xyz(Float a1, Float a2, Float a3);

}
#endif