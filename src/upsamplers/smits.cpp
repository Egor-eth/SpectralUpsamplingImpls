#include "smits.h"
#include "common/progress.h"
#include "math/math.h"

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


    void add_array_multiplied(Spectrum &s, const Float mul, const Float *ptr)
    {
        for(int wl = WL_START; wl <= WL_END; wl += WL_STEP) {
            s.get_or_create(wl) += mul * (*(ptr++));
        }
    }
}

void SmitsUpsampler::upsample_pixel_to(const Pixel &pixel, Spectrum &s) const
{
    s = Spectrum();
    vec3 rgb = pixel.to_vec3();

    if(pixel.r <= pixel.g && pixel.r <= pixel.b) {
        add_array_multiplied(s, rgb[0], WHITE_SPECTRUM);

        if(pixel.g <= pixel.b) {
            add_array_multiplied(s, rgb[1] - rgb[0], CYAN_SPECTRUM);
            add_array_multiplied(s, rgb[2] - rgb[1], BLUE_SPECTRUM);
        }
        else {
            add_array_multiplied(s, rgb[2] - rgb[0], CYAN_SPECTRUM);
            add_array_multiplied(s, rgb[1] - rgb[2], GREEN_SPECTRUM);
        }
    }
    else if(pixel.g <= pixel.r && pixel.g <= pixel.b) {
        add_array_multiplied(s, rgb[1], WHITE_SPECTRUM);

        if(pixel.r <= pixel.b) {
            add_array_multiplied(s, rgb[0] - rgb[1], MAGENTA_SPECTUM);
            add_array_multiplied(s, rgb[2] - rgb[0], BLUE_SPECTRUM);
        }
        else {
            add_array_multiplied(s, rgb[2] - rgb[1], MAGENTA_SPECTUM);
            add_array_multiplied(s, rgb[0] - rgb[2], RED_SPECTRUM);
        }
    }
    else /*if(pixel.b <= pixel.r && pixel.b <= pixel.g)*/ {
        add_array_multiplied(s, rgb[2], WHITE_SPECTRUM);

        if(pixel.r <= pixel.g) {
            add_array_multiplied(s, rgb[0] - rgb[2], YELLOW_SPECTUM);
            add_array_multiplied(s, rgb[1] - rgb[0], GREEN_SPECTRUM);
        }
        else {
            add_array_multiplied(s, rgb[1] - rgb[2], YELLOW_SPECTUM);
            add_array_multiplied(s, rgb[0] - rgb[1], RED_SPECTRUM);
        }
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