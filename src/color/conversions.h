#ifndef COLOR_CONVERSIONS_H
#define COLOR_CONVERSIONS_H
#include "common/math.h"
#include "spectre.h"

inline vec3 rgb2xyz(const vec3 &rgb)
{
    return RGB_TO_XYZ * rgb;
}

inline vec3 xyz2rgb(const vec3 &xyz)
{
    return XYZ_TO_RGB * xyz;
}

vec3 spectre_to_xyz(const Spectre &spectre);

#endif