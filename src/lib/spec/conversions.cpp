#include "conversions.h" 
#include <memory>
#include "spec/spectral_util.h"


namespace spec {
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

            return f_a + (f_b - f_a) * (wl - a) / CURVES_WAVELENGHTS_STEP;
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

            return f_a + (f_b - f_a) * (wl - a) / CURVES_WAVELENGHTS_STEP;
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

            return f_a + (f_b - f_a) * (wl - a) / CURVES_WAVELENGHTS_STEP;
        }

        BasicSpectrum _cie_D6500;
        bool _notloaded = true;

        const BasicSpectrum &get_D6500()
        {
            if(_notloaded) {
                _cie_D6500 = spec::util::load_spd("resources/cie.stdillum.D6500.spd");
                _notloaded = false;
            }
            return _cie_D6500;
        }


        Float get_cie_y_integral(const std::set<Float> &)
        {
            static Float val = 0.0f;
            static bool not_computed = true;
            if(not_computed) {
                const BasicSpectrum &d65 = get_D6500();
                for(int lambda = CURVES_WAVELENGHTS_START; lambda <= CURVES_WAVELENGHTS_END; lambda += CURVES_WAVELENGHTS_STEP) {
                //for(int lambda : wl) {
                    Float lightval = d65.get_or_interpolate(lambda);
                    val += _y(lambda) * CURVES_WAVELENGHTS_STEP * lightval;
                }
                not_computed = false;
            }
            return val;
        }
    }


    vec3 spectre2xyz(const BasicSpectrum &spectre)
    {
        vec3 xyz{0.0f, 0.0f, 0.0f};
        const auto &wl = spectre.get_wavelenghts();
        const BasicSpectrum &d65 = get_D6500();

        int count = 0;
        for(int lambda = CURVES_WAVELENGHTS_START; lambda <= CURVES_WAVELENGHTS_END; lambda += CURVES_WAVELENGHTS_STEP) {
        //for(int lambda : wl) {
            Float val = spectre.get_or_interpolate(lambda);
            Float lightval = d65.get_or_interpolate(lambda);
            count += val != 0.0f;
            
            xyz.x += _x(lambda) * val * lightval;
            xyz.y += _y(lambda) * val * lightval;
            xyz.z += _z(lambda) * val * lightval;
        }
        xyz /= get_cie_y_integral(wl);
        return xyz;
    }
}