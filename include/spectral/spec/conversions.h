#ifndef INCLUDE_SPECTRAL_SPEC_CONVERSIONS_H
#define INCLUDE_SPECTRAL_SPEC_CONVERSIONS_H
#include <spectral/internal/math/math.h>
#include <spectral/spec/basic_spectrum.h>
#include <spectral/spec/spectral_util.h>
#include <utility>

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

    inline vec3 rgb2cielab(const vec3 &rgb) 
    {
        return xyz2cielab(rgb2xyz(rgb));
    }

    vec3 spectre2xyz(const ISpectrum &spectre, const ISpectrum &light);
    inline vec3 spectre2xyz(const ISpectrum &spectre)
    {
        return spectre2xyz(spectre, *util::CIE_D6500);
    }

    vec3 sigpoly2xyz(Float a1, Float a2, Float a3);


    //Approximation based on http://jcgt.org/published/0003/04/03/paper.pdf
    std::pair<Float, Float> color2ior(Float reflectivity, Float edgetint);

    inline std::pair<Float, Float> color2ior(Float color)
    {
        return color2ior(color, color);
    }

}
#endif