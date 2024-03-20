#include <spec/spectral_util.h>
#include <spec/basic_spectrum.h>
#include <spec/conversions.h>
#include <internal/serialization/csv.h>
#include <internal/math/math.h>
#include <numeric>
#include <iostream>
#include <fstream>

using namespace spec;



int main()
{
    ISpectralImage::ptr img;
    ISpectrum::csptr lightsource;

    util::load_spectral_image("input/food_chili.hdr", img, lightsource);

    util::save_as_png1(*dyn_cast<BasicSpectralImage>(img.get()), "output/result", util::META_FILENAME, *lightsource);

    return 0;
} 
