#include "conversions.h" 
#include <memory>
#include "spec/spectral_util.h"
#include "common/lazy_value.h"

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

        const LazyValue<BasicSpectrum> CIE_D6500{[]() -> auto { return util::load_spd("resources/cie.stdillum.D6500.spd"); }};

    /*    const BasicSpectrum &get_D6500()
        {
            if(_notloaded) {
                _cie_D6500 = spec::util::load_spd("resources/cie.stdillum.D6500.spd");
                _notloaded = false;
            }
            return _cie_D6500;
        }*/


        Float get_cie_y_integral()
        {
            static Float val = 0.0f;
            static bool not_computed = true;
            if(not_computed) {
                for(int lambda = CURVES_WAVELENGHTS_START; lambda <= CURVES_WAVELENGHTS_END; lambda += CURVES_WAVELENGHTS_STEP) {
                //for(int lambda : wl) {
                    Float lightval = CIE_D6500->get_or_interpolate(lambda);
                    val += _y(lambda) * CURVES_WAVELENGHTS_STEP * lightval;
                }
                not_computed = false;
            }
            return val;
        }

        const vec3 XYZ_TO_CIELAB_XYZN{95.0489f, 100.0f, 108.8840f};

        Float _xyz2cielab_f(Float t)
        {
            static constexpr Float delta = 6.0f / 29.0f;
            static constexpr Float f_tn = 4.0f / 29.0f;
            static constexpr Float delta_div = 3.0f * delta * delta;
            static constexpr Float delta3 = delta * delta * delta;

            return t - delta3 > EPSILON ? std::cbrt(t) : std::fma(t, delta_div, f_tn);
        }
    }


    vec3 spectre2xyz_old(const ISpectrum &spectre)
    {
        vec3 xyz{0.0f, 0.0f, 0.0f};
        //const auto &wl = spectre.get_wavelenghts();

        int count = 0;
        for(int lambda = CURVES_WAVELENGHTS_START; lambda <= CURVES_WAVELENGHTS_END; lambda += CURVES_WAVELENGHTS_STEP) {
        //for(int lambda : wl) {
            Float val = spectre(lambda);
            Float lightval = CIE_D6500->get_or_interpolate(lambda);
            count += val != 0.0f;
            

            xyz.x += _x(lambda) * val * lightval;
            xyz.y += _y(lambda) * val * lightval;
            xyz.z += _z(lambda) * val * lightval;
        }
        xyz /= get_cie_y_integral();
        return xyz;
    }

    vec3 spectre2xyz(const ISpectrum &spectrum)
    {
        vec3 xyz{0.0f, 0.0f, 0.0f};
        //const BasicSpectrum &d65 = get_D6500();

        unsigned idx = 0u;
        for(int lambda = CURVES_WAVELENGHTS_START; lambda <= CURVES_WAVELENGHTS_END; lambda += CURVES_WAVELENGHTS_STEP) {
            const Float val_lv = spectrum(lambda) * CIE_D6500->get_or_interpolate(lambda);
            
            xyz.x += X_CURVE[idx] * val_lv;
            xyz.y += Y_CURVE[idx] * val_lv;
            xyz.z += Z_CURVE[idx] * val_lv;
            idx += 1;
        }
        xyz /= get_cie_y_integral();
        return xyz;
    }

    vec3 xyz2cielab(const vec3 &xyz)
    {
        const vec3 xyz_norm = xyz / XYZ_TO_CIELAB_XYZN;
        const Float f_x = _xyz2cielab_f(xyz_norm.x);
        const Float f_y = _xyz2cielab_f(xyz_norm.y);
        const Float f_z = _xyz2cielab_f(xyz_norm.z);

        return vec3{
            116.0f * f_y - 16.0f,
            500.0f * (f_x - f_y),
            200.0f * (f_y - f_z)
        };
    }

}