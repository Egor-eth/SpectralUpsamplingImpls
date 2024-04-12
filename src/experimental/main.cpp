#include <spec/spectral_util.h>
#include <spec/basic_spectrum.h>
#include <spec/conversions.h>
#include <internal/serialization/csv.h>
#include <internal/math/math.h>
#include <internal/math/fourier.h>
#include <internal/common/constants.h>
#include <spec/basic_spectrum.h>
#include <spec/fourier_spectrum.h>
#include <internal/common/format.h>
#include <numeric>
#include <iostream>
#include <fstream>

#include <internal/math/levinson.h>

using namespace spec;

void fourier_tests(const char *path)
{
    ISpectrum::ptr spec;
    ISpectrum::csptr light;
    util::load_spectrum(path, spec, light);

    std::vector<Float> wavelenghts;
    std::vector<Float> values;
    for(unsigned i = CURVES_WAVELENGHTS_START; i <= CURVES_WAVELENGHTS_END; i += 1) {
        wavelenghts.push_back(Float(i));
        values.push_back(spec->get_or_interpolate(Float(i)));
    }
    std::vector<Float> phases = math::wl_to_phases(wavelenghts);

    std::vector<Float> moments = math::real_fourier_moments_of(phases, values, 4);
    std::vector<Complex> lagrange = math::lagrange_multipliers(moments);

   /* std::cout << "lagrange: ";
    for(const auto &v : lagrange) {
        std::cout << v << " ";
    }
    std::cout << std::endl;*/


    LFourierSpectrum fspec{std::move(lagrange)};

    vec3 rgb_gt = spectre2rgb(*spec);
    vec3 rgb = spectre2rgb(fspec);

    std::cout << rgb_gt << " " << rgb << std::endl;

/*
    std::cout << "gt: ";
    for(unsigned i = 0; i < values.size(); ++i) {
        std::cout << values[i] << " ";
    }
    std::cout << std::endl << "fr: ";
    for(unsigned i = 0; i < values.size(); ++i) {
        std::cout << fspec.get_or_interpolate(wavelenghts[i]) << " ";
    }
    std::cout << std::endl;
*/
}

void fourier_tests1() {
    std::ifstream file{"input/leds.csv"};

    std::vector<Float> wavelenghts = std::get<0>(*csv::parse_line_m<Float>(file));
    auto data = csv::load_as_vector_m<Float>(file);

    const std::vector<Float> spd = std::get<0>(data[1192]);

    BasicSpectrum spec_gt;
    for(unsigned i = 0; i < wavelenghts.size(); ++i) {
        spec_gt.set(wavelenghts[i], spd[i]);
    }

    std::vector<Float> phases = math::wl_to_phases(wavelenghts);

    std::vector<Float> moments = math::real_fourier_moments_of(phases, spd, 16);

    std::vector<Float> res = math::mese(phases, moments);

    BasicSpectrum spec;
    for(unsigned i = 0; i < wavelenghts.size(); ++i) {
        spec.set(wavelenghts[i], res[i]);
    }

    vec3 rgb_gt = spectre2rgb(spec_gt);
    vec3 rgb = spectre2rgb(spec);
    std::cout << rgb_gt << " " << rgb << std::endl;
}

void dataset_tests()
{
    std::ifstream file{"input/leds.csv"};

    std::vector<unsigned> wavelenghts = std::get<0>(*csv::parse_line_m<unsigned>(file));
    auto data = csv::load_as_vector_m<Float>(file);

    std::ofstream output("output/rgbs_blue.csv");
    int n = 0;
    for(const auto &e : data) {
        const std::vector<Float> &spec = std::get<0>(e);
        Float max = 0.0f;
        for(unsigned i = 0; i < spec.size(); ++i) {
            if(spec[i] > max) max = spec[i];
        }

        BasicSpectrum sp;
        for(unsigned i = 0; i < wavelenghts.size(); ++i) {
            sp.set(wavelenghts[i], spec[i] / max);
        }
        vec3 xyz = spectre2xyz(sp);
        xyz /= xyz.y;
        vec3 rgb = xyz2rgb_unsafe(xyz);
        int t = rgb.x >= 0.0f && rgb.x <= 1.0f && rgb.y >= 0.0f && rgb.y <= 1.0f && rgb.z >= 0.0f && rgb.z <= 1.0f;

        if(true) {
            int amax = rgb[0] > rgb[1] ? 0 : 1;
            amax = rgb[amax] > rgb[2] ? amax : 2;

            bool singlecolor = rgb[amax] > 1.25f * (rgb[(amax + 1) % 3] + rgb[(amax + 2) % 3]);

            if(singlecolor && amax == 2) output << format("%f,%f,%f:%d", rgb.x, rgb.y, rgb.z, n) << std::endl;
        }
        n += 1;
    }
    
    output.close();
}

int main(int argc, char **argv)
{
    (void) argc; (void) argv;
    /*
    ISpectralImage::ptr img;
    ISpectrum::csptr lightsource;

    util::load_spectral_image("input/food_chili.hdr", img, lightsource);

    util::save_as_png1(*dyn_cast<BasicSpectralImage>(img.get()), "output/result", util::META_FILENAME, *lightsource);
    */
    fourier_tests1();
    /*
    std::vector<double> d{5.0f, -1.0f, 1.0f, 2.0f, 3.0f};
    std::vector<double> y{2.0f, 3.0f, 1.0f};
    auto res = math::levinson<double>(d, y);

    std::cout << res[0] << " " << res[1] << " " << res[2] << std::endl;
    */
    return 0;
} 

