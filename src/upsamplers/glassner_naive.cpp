#include "glassner_naive.h" 
#include "spec/conversions.h"
#include "common/progress.h"
#include "math/math.h"



namespace spec {

    namespace {

        constexpr int AMPLITUDES[]{590, 560, 440};

        const mat3 XYZ_TO_SPECTRE_INV = math::inverse(mat3{ //C
            1.026f, 0.757f, 0.001f,
            0.594f, 0.995f, 0.004f,
            0.348f, 0.023f, 1.747f
        });

        void upsample_pixel_to(const Pixel &pixel, BasicSpectrum &spectrum)
        {
            spectrum = BasicSpectrum();
            vec3 ampls = rgb2xyz(pixel.to_vec3()) * XYZ_TO_SPECTRE_INV;
       
            spectrum.get_or_create(AMPLITUDES[0]) = ampls[0];
            spectrum.get_or_create(AMPLITUDES[1]) = ampls[1];
            spectrum.get_or_create(AMPLITUDES[2]) = ampls[2];
        }

    }



    ISpectrum::ptr GlassnerUpsampler::upsample_pixel(const Pixel &pixel) const
    {
        BasicSpectrum *spectrum = new BasicSpectrum();
        upsample_pixel_to(pixel, *spectrum);
        return ISpectrum::ptr(spectrum);
    }

    ISpectralImage::ptr GlassnerUpsampler::upsample(const Image &sourceImage) const
    {
        BasicSpectralImage *dest = new BasicSpectralImage(sourceImage.get_width(), sourceImage.get_height());
        init_progress_bar();
        const long img_size = sourceImage.get_width() * sourceImage.get_height();
        const double wh = 1.0 / img_size;

        dest->set_wavelenghts({590.0f, 560.0f, 440.0f});


        const Pixel *ptr = sourceImage.raw_data();
        BasicSpectrum *s_ptr = dest->raw_data();
        for(int i = 0; i < img_size; ++i) {
            upsample_pixel_to(ptr[i], s_ptr[i]);
            print_progress((i + 1) * wh);
        }
        finish_progress_bar();
        return ISpectralImage::ptr(dest);
    }
}