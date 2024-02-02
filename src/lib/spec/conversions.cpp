#include "conversions.h" 
#include <memory>
#include "spec/spectral_util.h"
#include "common/lazy_value.h"

namespace spec {
    namespace {

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
                    Float lightval = util::CIE_D6500->get_or_interpolate(lambda);
                    val += util::_interp<Y_CURVE>(lambda) * CURVES_WAVELENGHTS_STEP * lightval;
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
            Float lightval = util::CIE_D6500->get_or_interpolate(lambda);
            count += val != 0.0f;
            

            xyz.x += util::_interp<X_CURVE>(lambda) * val * lightval;
            xyz.y += util::_interp<Y_CURVE>(lambda) * val * lightval;
            xyz.z += util::_interp<Z_CURVE>(lambda) * val * lightval;
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
            const Float val_lv = spectrum(lambda) * util::CIE_D6500->get_or_interpolate(lambda);
            
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

    vec3 sigpoly2xyz(Float a1, Float a2, Float a3)
    {
        vec3 xyz{0.0f, 0.0f, 0.0f};
        //const BasicSpectrum &d65 = get_D6500();
        const Float coef[3]{a1, a2, a3};
        unsigned idx = 0u;
        for(int lambda = CURVES_WAVELENGHTS_START; lambda <= CURVES_WAVELENGHTS_END; lambda += CURVES_WAVELENGHTS_STEP) {
            const Float val_lv = math::sigmoid_polynomial(lambda, coef) * util::CIE_D6500->get_or_interpolate(lambda);
            
            xyz.x += X_CURVE[idx] * val_lv;
            xyz.y += Y_CURVE[idx] * val_lv;
            xyz.z += Z_CURVE[idx] * val_lv;
            idx += 1;
        }
        xyz /= get_cie_y_integral();
        return xyz;
    }

}