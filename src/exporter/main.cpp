#include <spec/fourier_spectrum.h>
#include <spec/basic_spectrum.h>
#include <spec/spectral_util.h>
#include <imageutil/pixel.h>
#include <upsample/functional/fourier.h>
#include <internal/common/constants.h>
#include <internal/common/format.h>
#include <fstream>
#include <cstring>
#include <iostream>

using namespace spec;

FourierLUT load_lut() {
    std::ifstream file{"resources/f_emission_lut.eflf"};
    return FourierLUT::load_from(file);
}

int main(int argc, char **argv)
{
    if(argc == 1) return 1;

    const Pixel color = Pixel::from_rgb(std::stoi(argv[1], nullptr, 16));

    FourierLUT lut = load_lut();

    FourierEmissionSpectrum fspec = upsample::fourier_emiss_int(color, 1.0f, lut);

    BasicSpectrum spec;
    for(int wl = CURVES_WAVELENGHTS_START; wl < CURVES_WAVELENGHTS_END; wl += CURVES_WAVELENGHTS_STEP)
    {
        spec.set(wl, fspec.get_or_interpolate(wl));
    }

    util::save_spd(format("output/spd/%s.spd", argv[1]), spec);

    return 0;
}