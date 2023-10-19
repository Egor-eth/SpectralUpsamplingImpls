#include "naive_upsampler.h" 
#include "common/math.h"
#include "common/progress.h"


namespace {
    constexpr int AMPLITUDES[]{590, 560, 440};

    const mat3x3 XYZ_TO_SPECTRE_INV = glm::inverse(mat3x3{ //C
        1.026f, 0.757f, 0.001f,
        0.594f, 0.995f, 0.004f,
        0.348f, 0.023f, 1.747f
    });;

    const mat3x3 XYZ_TO_RGB_INV{ //M
        0.4124564f, 0.3575761f, 0.1804375f,
        0.2126729f, 0.7151522f, 0.0721750f,
        0.0193339f, 0.1191920f, 0.9503041f
    };

}

void NaiveUpsampler::upsample_pixel_to(const Pixel &pixel, Spectre &spectre) const
{
    vec3 ampls = XYZ_TO_RGB_INV * pixel.to_vec3() * XYZ_TO_SPECTRE_INV;
    spectre = Spectre();
    spectre[AMPLITUDES[0]] = ampls[0];
    spectre[AMPLITUDES[1]] = ampls[1];
    spectre[AMPLITUDES[2]] = ampls[2];
}

#include <iostream>

void NaiveUpsampler::upsample(const Image &sourceImage, SpectralImage &dest) const
{
    init_progress_bar();
    const double wh = 1.0 / (sourceImage.get_width() * sourceImage.get_height());

    dest = SpectralImage(sourceImage.get_width(), sourceImage.get_height());
    dest.set_wavelenghts({590.0f, 560.0f, 440.0f});
    for(int i = 0; i < sourceImage.get_width(); ++i) {
        for(int j = 0; j < sourceImage.get_height(); ++j) {
            upsample_pixel_to(sourceImage.at(i, j), dest.at(i, j));
            print_progress((i * sourceImage.get_height() + j) * wh);
        }
    }
    finish_progress_bar();
}