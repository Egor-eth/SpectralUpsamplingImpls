#include "naive_upsampler.h" 
#include "color/conversions.h"
#include "common/progress.h"

namespace {
    constexpr int AMPLITUDES[]{590, 560, 440};

    const mat3x3 XYZ_TO_SPECTRE_INV = glm::inverse(mat3x3{ //C
        1.026f, 0.757f, 0.001f,
        0.594f, 0.995f, 0.004f,
        0.348f, 0.023f, 1.747f
    });

}

void NaiveUpsampler::upsample_pixel_to(const Pixel &pixel, Spectre &spectre) const
{
    vec3 ampls = rgb2xyz(pixel.to_vec3()) * XYZ_TO_SPECTRE_INV;
    spectre = Spectre();
    spectre[AMPLITUDES[0]] = ampls[0];
    spectre[AMPLITUDES[1]] = ampls[1];
    spectre[AMPLITUDES[2]] = ampls[2];
}

void NaiveUpsampler::upsample(const Image &sourceImage, SpectralImage &dest) const
{
    init_progress_bar();
    const long img_size = sourceImage.get_width() * sourceImage.get_height();
    const double wh = 1.0 / img_size;

    dest = SpectralImage(sourceImage.get_width(), sourceImage.get_height());
    dest.set_wavelenghts({590.0f, 560.0f, 440.0f});


    const Pixel *ptr = sourceImage.raw_data();
    Spectre *s_ptr = dest.raw_data();
    for(int i = 0; i < img_size; ++i) {
        upsample_pixel_to(ptr[i], s_ptr[i]);
        print_progress(i * wh);
    }
    finish_progress_bar();
}