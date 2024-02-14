#include "sigpoly.h"
#include "spec/sp_lut.h"
#include "common/lazy_value.h"
#include "spec/sigpoly_spectrum.h"
#include <fstream>
#include "common/util.h"

namespace spec {

    namespace {

        LUT load_from_file(const std::string &path)
        {
            std::ifstream file{path};
            return LUT::load_from(file);
        }

        const LazyValue<LUT> LUTS[3] = {
            {[]() -> auto {return load_from_file("resources/sp_lut0.slf");}},
            {[]() -> auto {return load_from_file("resources/sp_lut1.slf");}},
            {[]() -> auto {return load_from_file("resources/sp_lut2.slf");}}
        };

        int argmax(const Pixel &p)
        {
            int m = p.rgba[0] > p.rgba[1] ? 0 : 1;
            return p.rgba[m] > p.rgba[2] ? m : 2;
        }

        void upsample_pixel_to(const Pixel &pixel, SigPolySpectrum &s) 
        {
            int amax = argmax(pixel);

            int alpha = pixel.rgba[amax];
            int a = pixel.rgba[(amax + 1) % 3] / static_cast<Float>(alpha) * 255.0f;
            int b = pixel.rgba[(amax + 2) % 3] / static_cast<Float>(alpha) * 255.0f;

            s = SigPolySpectrum(LUTS[amax]->eval(a, b, alpha));
        }

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