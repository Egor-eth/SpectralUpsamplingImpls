#ifndef COMMON_H
#define COMMON_H
#include "imageutil/spectral_image.h"
#include "imageutil/image.h"

class IUpsampler
{
public:
    virtual void upsample(const Image &sourceImage, SpectralImage &dest) const = 0;
    virtual void upsample_pixel_to(const Pixel &src, Spectre &dst) const = 0;

    Spectre upsample_pixel(const Pixel &src) const
    {
        Spectre s;
        upsample_pixel_to(src, s);
        return s;
    }

    virtual ~IUpsampler() {}
};


#endif