#include "glassner_naive.h" 
#include "conversions.h"
#include "common/progress.h"
#include "math/math.h"

namespace {
    constexpr int AMPLITUDES[]{590, 560, 440};

    const mat3 XYZ_TO_SPECTRE_INV = math::inverse(mat3{ //C
        1.026f, 0.757f, 0.001f,
        0.594f, 0.995f, 0.004f,
        0.348f, 0.023f, 1.747f
    });

}

void GlassnerUpsampler::upsample_pixel_to(const Pixel &pixel, Spectrum &spectre) const
{
    vec3 ampls = rgb2xyz(pixel.to_vec3()) * XYZ_TO_SPECTRE_INV;
    spectre = Spectrum();
    spectre.get_or_create(AMPLITUDES[0]) = ampls[0];
    spectre.get_or_create(AMPLITUDES[1]) = ampls[1];
    spectre.get_or_create(AMPLITUDES[2]) = ampls[2];
}

void GlassnerUpsampler::upsample(const Image &sourceImage, SpectralImage &dest) const
{
    init_progress_bar();
    const long img_size = sourceImage.get_width() * sourceImage.get_height();
    const double wh = 1.0 / img_size;

    dest = SpectralImage(sourceImage.get_width(), sourceImage.get_height());
    dest.set_wavelenghts({590.0f, 560.0f, 440.0f});


    const Pixel *ptr = sourceImage.raw_data();
    Spectrum *s_ptr = dest.raw_data();
    for(int i = 0; i < img_size; ++i) {
        upsample_pixel_to(ptr[i], s_ptr[i]);
        print_progress((i + 1) * wh);
    }
    finish_progress_bar();
}