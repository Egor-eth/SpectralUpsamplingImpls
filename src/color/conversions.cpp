#include "conversions.h" 
#include <memory>

namespace {

    Float _x(Float wl)
    {
        const int a = (static_cast<int>(wl) / CURVES_WAVELENGHTS_STEP) * CURVES_WAVELENGHTS_STEP;
        const int a_idx = (a - CURVES_WAVELENGHTS_START) / CURVES_WAVELENGHTS_STEP;
        if(a > CURVES_WAVELENGHTS_END) return 0.0f;

        const Float f_a = X_CURVE[a_idx];
        if(a == wl) return f_a;


        const int b_idx = a_idx + 1;
        if(b_idx < CURVES_WAVELENGHTS_START / CURVES_WAVELENGHTS_STEP) return 0.0f;

        const Float f_b = X_CURVE[b_idx];

        return (f_b - f_a) * CURVES_WAVELENGHTS_STEP / (wl - a);
    }

    Float _y(Float wl)
    {
        const int a = (static_cast<int>(wl) / CURVES_WAVELENGHTS_STEP) * CURVES_WAVELENGHTS_STEP;
        const int a_idx = (a - CURVES_WAVELENGHTS_START) / CURVES_WAVELENGHTS_STEP;
        if(a > CURVES_WAVELENGHTS_END) return 0.0f;

        const Float f_a = Y_CURVE[a_idx];
        if(a == wl) return f_a;


        const int b_idx = a_idx + 1;
        if(b_idx < CURVES_WAVELENGHTS_START / CURVES_WAVELENGHTS_STEP) return 0.0f;

        const Float f_b = Y_CURVE[b_idx];

        return (f_b - f_a) * CURVES_WAVELENGHTS_STEP / (wl - a);
    }

    Float _z(Float wl)
    {
        const int a = (static_cast<int>(wl) / CURVES_WAVELENGHTS_STEP) * CURVES_WAVELENGHTS_STEP;
        const int a_idx = (a - CURVES_WAVELENGHTS_START) / CURVES_WAVELENGHTS_STEP;
        if(a > CURVES_WAVELENGHTS_END) return 0.0f;

        const Float f_a = Z_CURVE[a_idx];
        if(a == wl) return f_a;


        const int b_idx = a_idx + 1;
        if(b_idx < CURVES_WAVELENGHTS_START / CURVES_WAVELENGHTS_STEP) return 0.0f;

        const Float f_b = Z_CURVE[b_idx];

        return (f_b - f_a) * CURVES_WAVELENGHTS_STEP / (wl - a);
    }
}

Float get_cie_y_integral()
{
    static Float val = 0.0f;
    static bool not_computed = true;
    if(not_computed) {
        for(int lambda = CURVES_WAVELENGHTS_START; lambda <= CURVES_WAVELENGHTS_END; lambda += CURVES_WAVELENGHTS_STEP) {
            val += _y(lambda) * CURVES_WAVELENGHTS_STEP;
        }
        not_computed = false;
    }
    return val;
}

#include <iostream>

vec3 spectre2xyz(const Spectrum &spectre)
{
    vec3 xyz{0.0f, 0.0f, 0.0f};
    const auto &wl = spectre.get_wavelenghts();
    for(int lambda : wl) {
        Float val = spectre[lambda];
        xyz.x += _x(lambda) * val;
        xyz.y += _y(lambda) * val;
        xyz.z += _z(lambda) * val;
    }
    //int count = (CURVES_WAVELENGHTS_END - CURVES_WAVELENGHTS_START) / CURVES_WAVELENGHTS_STEP;
    std::cout <<  get_cie_y_integral() << std::endl;
    xyz = xyz / static_cast<float>(wl.size());
    std::cout << xyz.x << " " << xyz.y << " " << xyz.z << std::endl;
    return xyz / get_cie_y_integral();
}