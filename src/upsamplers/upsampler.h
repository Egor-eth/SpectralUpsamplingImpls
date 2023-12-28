#ifndef UPSAMPLERS_UPSAMPLER_H
#define UPSAMPLERS_UPSAMPLER_H
#include "spec/spectral_image.h"
#include "imageutil/image.h"

namespace spec {
    class IUpsampler
    {
    public:
        virtual void upsample(const Image &sourceImage, BasicSpectralImage &dest) const = 0;
        virtual void upsample_pixel_to(const Pixel &src, BasicSpectrum &dst) const = 0;

        BasicSpectrum upsample_pixel(const Pixel &src) const
        {
            BasicSpectrum s;
            upsample_pixel_to(src, s);
            return s;
        }

        virtual ~IUpsampler() {}
    };

}


#endif