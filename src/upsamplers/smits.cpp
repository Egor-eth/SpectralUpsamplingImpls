#include "smits.h"
#include "common/progress.h"
#include "common/math.h"

namespace {

    constexpr Float WHITE_SPECTRUM[] = {
        1.0000f, 1.0000f, 0.9999f, 0.9993f, 0.9992f,
        0.9998f, 1.0000f, 1.0000f, 1.0000f, 1.0000f
    };

    constexpr Float CYAN_SPECTRUM[] = {
        0.9710f, 0.9426f, 1.0007f, 1.0007f, 1.0007f,
        1.0007f, 0.1564f, 0.0000f, 0.0000f, 0.0000f
    };

    constexpr Float MAGENTA_SPECTUM[] = {
        1.0000f, 1.0000f, 0.9685f, 0.2229f, 0.0000f,
        0.0458f, 0.8369f, 1.0000f, 1.0000f, 0.9959f
    };

    constexpr Float YELLOW_SPECTUM[] = {
        0.0001f, 0.0000f, 0.1088f, 0.6651f, 1.0000f,
        1.0000f, 0.9996f, 0.9586f, 0.9685f, 0.9840f
    };

    constexpr Float RED_SPECTRUM[] = {
        0.1012f, 0.0515f, 0.0000f, 0.0000f, 0.0000f, 
        0.0000f, 0.8325f, 1.0149f, 1.0149f, 1.0149f
    };

    constexpr Float GREEN_SPECTRUM[] = {
        0.0000f, 0.0000f, 0.0273f, 0.7937f, 1.0000f,
        0.9418f, 0.1719f, 0.0000f, 0.0000f, 0.0025f
    };

    constexpr Float BLUE_SPECTRUM[] = {
        1.0000f, 1.0000f, 0.8916f, 0.3323f, 0.0000f,
        0.0000f, 0.0003f, 0.0369f, 0.0483f, 0.0496f
    }; 

    constexpr size_t SPECTRUM_SIZE = sizeof(WHITE_SPECTRUM) / sizeof(Float); 

    constexpr int WL_START = 380;
    constexpr int WL_END = 720;
    constexpr int WL_STEP = (WL_END - WL_START) / SPECTRUM_SIZE;

}

void SmitsUpsampler::upsample_pixel_to(const Pixel &pixel, Spectrum &spectre) const
{
    spectre = Spectrum();
    if(pixel.r <= pixel.g && pixel.r <= pixel.b) {

    }
}

void SmitsUpsampler::upsample(const Image &sourceImage, SpectralImage &dest) const
{
    init_progress_bar();
   
    const long img_size = sourceImage.get_width() * sourceImage.get_height();
    const double wh = 1.0 / img_size;

    dest = SpectralImage(sourceImage.get_width(), sourceImage.get_height());

    for(int lambda = WL_START; lambda <= WL_END; lambda += WL_STEP) {
        dest.add_wavelenght(lambda);
    }

    const Pixel *ptr = sourceImage.raw_data();
    Spectrum *s_ptr = dest.raw_data();
    for(int i = 0; i < img_size; ++i) {
        upsample_pixel_to(ptr[i], s_ptr[i]);
        print_progress((i + 1) * wh);
    }

    finish_progress_bar();
}