#include "sigpoly.h"
#include <fstream>
#include "common/util.h"

#include <iostream>

namespace spec {

    namespace {

        LUT load_from_file(const std::string &path)
        {
            std::ifstream file{path};
            return LUT::load_from(file);
        }

        int argmax(const Pixel &p)
        {
            int m = p.rgba[0] >= p.rgba[1] ? 0 : 1;
            return p.rgba[m] >= p.rgba[2] ? m : 2;
        }

    }

    void SigPolyUpsampler::upsample_pixel_to(const Pixel &pixel, SigPolySpectrum &s) const
    {
        int amax = argmax(pixel);
        int a = 0, b = 0;
        int alpha = pixel.rgba[amax];
        if(alpha != 0) {
            a = pixel.rgba[(amax + 1) % 3] / static_cast<Float>(alpha) * 255.0f;
            b = pixel.rgba[(amax + 2) % 3] / static_cast<Float>(alpha) * 255.0f;
        }
       // std::cout << "amax: " << amax << " a, b, alpha: " << a << " " << b << " " << alpha << std::endl;

        s = SigPolySpectrum(luts[amax].eval(a, b, alpha));
    }

    SigPolyUpsampler::SigPolyUpsampler()
        : luts{load_from_file("resources/sp_lut0.slf"), load_from_file("resources/sp_lut1.slf"), load_from_file("resources/sp_lut2.slf")}
    {

    }

    ISpectrum::ptr SigPolyUpsampler::upsample_pixel(const Pixel &src) const
    {
        SigPolySpectrum *spectrum = new SigPolySpectrum();
        upsample_pixel_to(src, *spectrum);
        return ISpectrum::ptr(spectrum);
    }

    ISpectralImage::ptr SigPolyUpsampler::upsample(const Image &sourceImage) const
    {
        SigPolySpectralImage *dest = new SigPolySpectralImage(sourceImage.get_width(), sourceImage.get_height());
       
        const long img_size = sourceImage.get_width() * sourceImage.get_height();
        init_progress_bar(img_size);

        const Pixel *ptr = sourceImage.raw_data();
        SigPolySpectrum *s_ptr = dest->raw_data();
        for(int i = 0; i < img_size; ++i) {
            upsample_pixel_to(ptr[i], s_ptr[i]);
            print_progress(i + 1);
        }

        finish_progress_bar();
        return ISpectralImage::ptr(dest);
    }

}