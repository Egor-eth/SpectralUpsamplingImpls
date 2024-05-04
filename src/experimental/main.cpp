#include <spec/spectral_util.h>
#include <spec/basic_spectrum.h>
#include <spec/conversions.h>
#include <internal/serialization/csv.h>
#include <internal/math/math.h>
#include <internal/common/constants.h>
#include <spec/basic_spectrum.h>
#include <upsample/sigpoly.h>
#include <internal/common/format.h>
#include <numeric>
#include <iostream>
#include <fstream>
#include <cassert>
#include <set>
#include <map>
#include <iomanip>

#include "fourier_tests.h"

using namespace spec;


//wavelenghts must be sampled to CURVES_WAVELENGHTS values
template<typename T>
math::base_vec3<T> _spectre2xyz(const std::vector<Float> &wavelenghts, const std::vector<T> &values)
{
    static T cieyint = T(util::get_cie_y_integral());

    math::base_vec3<T> xyz{};

    unsigned idx = 0u;
    for(unsigned i = 0; i < wavelenghts.size(); ++i) {
        const T val_lv = values[i];// * T(util::CIE_D6500.get_or_interpolate(wavelenghts[i]));
        
        xyz.x += T(util::_interp<X_CURVE>(wavelenghts[i])) * val_lv;
        xyz.y += T(util::_interp<Y_CURVE>(wavelenghts[i])) * val_lv;
        xyz.z += T(util::_interp<Z_CURVE>(wavelenghts[i])) * val_lv;
        idx += 1;
    }
   // xyz /= cieyint;
    return xyz;// * (T(CURVES_WAVELENGHTS_END - CURVES_WAVELENGHTS_START) / wavelenghts.size());
}

constexpr Float MIN_DIST = 2.34f;

template<typename T>
T _get_cie_y_integral(const std::vector<Float> &wavelenghts, const std::vector<T> &values)
{
    T val = 0.0f;
    for(unsigned i = 0; i < wavelenghts.size(); ++i) {
    //for(int lambda : wl) {

        Float delta = 1.0f / Float(wavelenghts.size());
       /* if(i == 0) {
            delta = (wavelenghts[1] - wavelenghts[0]) * 0.5f;
        }
        else if(i == wavelenghts.size() - 1) {
            delta = (wavelenghts.back() - wavelenghts[wavelenghts.size() - 2]) * 0.5f;
        }
        else {
            delta = (wavelenghts[i + 1] - wavelenghts[i - 1]) * 0.5f;
        }
       // std::cout << "delta " << delta << std::endl;
        delta = delta + 1;*/
        val += T(util::_interp<Y_CURVE>(wavelenghts[i])) * values[i];
    }
    return val;
}

void dataset_tests()
{
    std::ifstream file{"input/leds.csv"};

    std::vector<Float> wavelenghts = std::get<0>(*csv::parse_line_m<Float>(file));
    auto data = csv::load_as_vector_m<Float>(file);

    std::ofstream output_sp("output/dataset_spectra.csv");
    std::ofstream output_rgb("output/dataset_rgb.csv");
    int n = 0;
    assert(wavelenghts.size() != 0);

    std::vector<Float> d65_1(wavelenghts.size());
    for(unsigned i = 0; i < wavelenghts.size(); ++i) d65_1[i] = util::CIE_D6500.get_or_interpolate(wavelenghts[i]);
    data.push_back(std::make_tuple<>(std::move(d65_1)));

    //csv head
    output_rgb << "r,g,b" << std::endl << std::setprecision(10);
    output_sp << std::setprecision(10);
    for(unsigned i = 0; i < wavelenghts.size() - 1; ++i) {
        output_sp << wavelenghts[i] << ",";
    }
    output_sp << wavelenghts.back() << std::endl;

    std::set<vec3> used_lab{};
    std::map<vec3, const std::vector<Float> *> specs;

    for(auto &e : data) {
        std::vector<Float> &spec = std::get<0>(e);
        assert(spec.size() == wavelenghts.size());

        Float ciey = _get_cie_y_integral(wavelenghts, spec);

        Float max = 0.0f;
        for(unsigned i = 0; i < spec.size(); ++i) {
            if(spec[i] > max) max = spec[i];
        }
        for(unsigned i = 0; i < spec.size(); ++i) {
            spec[i] /= max;
        }

        vec3 xyz = _spectre2xyz(wavelenghts, spec);
        vec3 rgb = xyz2rgb_unsafe(xyz);

    
        bool t = rgb.x >= 0.0f && rgb.x <= 1.0f && rgb.y >= 0.0f && rgb.y <= 1.0f && rgb.z >= 0.0f && rgb.z <= 1.0f;

        if(t) {
            vec3 lab = rgb2cielab(rgb);
            bool not_used = true;
            for(const auto &l : used_lab) {
                if(vec3::distance(l, lab) < MIN_DIST) {
                    not_used = false;
                    break;
                }
            }

            if(not_used) {
                used_lab.insert(lab);
                bool constraint = true;

                if(constraint) {
                    specs.insert({rgb, &spec});
                }

            }
            else {
                std::cout << format("Excluded %d, %f, %f, %f", n, rgb.x, rgb.y, rgb.z) << std::endl;
            }
        }
        else {
            std::cout << format("Excluded %d, %f, %f, %f", n, rgb.x, rgb.y, rgb.z) << std::endl;
        }
        n += 1;
    }
    
    for(const auto &[rgb, spec] : specs) {

        output_rgb << format("%d,%d,%d\n", int(rgb.x * 255.999f), int(rgb.y * 255.999f), int(rgb.z * 255.999f));

        for(unsigned i = 0; i < spec->size() - 1; ++i) {
            output_sp << spec->at(i) << ",";
        }
        output_sp << spec->back() << std::endl;
    }

    output_sp.close();
    output_rgb.close();
}


const BasicSpectrum UNIFORM {
    {CURVES_WAVELENGHTS_START, 1.0f},
    {0.5f * (CURVES_WAVELENGHTS_START + CURVES_WAVELENGHTS_END), 1.0f},
    {CURVES_WAVELENGHTS_END, 1.0f}
};

void dataset_tests1()
{
    std::ifstream file{"input/leds.csv"};
    std::ofstream output("output/out.spd");
    unsigned n = 0u;
    std::vector<Float> wavelenghts = std::get<0>(*csv::parse_line_m<Float>(file));
    auto data = csv::load_as_vector_m<Float>(file);

    for(const auto &e : data) {
        std::vector<Float> spec = std::get<0>(e);
        assert(spec.size() == wavelenghts.size());

        if(n != 1488) {
            n += 1;
            continue;
        }
        Float max = 0.0f;
        Float avg = 0.0f;
        for(unsigned i = 0; i < spec.size(); ++i) {
            if(spec[i] > max) max = spec[i];
            avg += spec[i];
        }
        avg /= spec.size();
        Float ciey = _get_cie_y_integral(wavelenghts, spec);

        for(unsigned i = 0; i < spec.size(); ++i) {
            spec[i] /= max;
        }

        std::cout << ciey << std::endl;

        BasicSpectrum sp;
        for(unsigned i = 0; i < wavelenghts.size(); ++i) {
            sp.set(wavelenghts[i], spec[i]);
        }
        vec3 xyz = spectre2xyz(sp);
        
        vec3 rgb = xyz2rgb_unsafe(xyz);
        bool t = rgb.x >= 0.0f && rgb.x <= 1.0f && rgb.y >= 0.0f && rgb.y <= 1.0f && rgb.z >= 0.0f && rgb.z <= 1.0f;

        if(true) {
            int amax = rgb[0] > rgb[1] ? 0 : 1;
            amax = rgb[amax] > rgb[2] ? amax : 2;

            bool constraint = true;//rgb[amax] > 1.25f * (rgb[(amax + 1) % 3] + rgb[(amax + 2) % 3]);

            if(constraint) {
                // std::cout << rgb << std::endl;
                //output << format("%5d\t%5d\t%5d\t\t:%04d", int(rgb.x * 255.999f), int(rgb.y * 255.999f), int(rgb.z * 255.999f), n) << std::endl;
                for(unsigned i = 0; i < spec.size(); ++i) {
                    output << std::fixed << wavelenghts[i] << " " << spec[i] * 100.0f << "\n";
                }
            }
        }
        n += 1;
    }
    
    output.close();
}

int main(int argc, char **argv)
{
    (void) argc; (void) argv;

    dataset_tests();

    //SigPolyUpsampler upsampler{};
    //ISpectrum::ptr spec = upsampler.upsample_pixel({255, 255, 255});
    //std::cout << util::get_cie_y_integral(*spec) << " " << spectre2rgb(*spec) << std::endl;

    return 0;
} 

