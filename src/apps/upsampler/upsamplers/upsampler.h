#ifndef UPSAMPLERS_UPSAMPLER_H
#define UPSAMPLERS_UPSAMPLER_H
#include "spec/spectral_image.h"
#include "imageutil/image.h"

class IUpsampler
{
public:
    virtual void upsample(const Image &sourceImage, SpectralImage &dest) const = 0;
    virtual void upsample_pixel_to(const Pixel &src, Spectrum &dst) const = 0;

    Spectrum upsample_pixel(const Pixel &src) const
    {
        Spectrum s;
        upsample_pixel_to(src, s);
        return s;
    }

    virtual ~IUpsampler() {}
};


#endif