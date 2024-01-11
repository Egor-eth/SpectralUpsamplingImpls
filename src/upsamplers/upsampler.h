#ifndef UPSAMPLERS_UPSAMPLER_H
#define UPSAMPLERS_UPSAMPLER_H
#include "spec/spectrum.h"
#include "imageutil/image.h"
#include <memory>

namespace spec {

    class IUpsampler
    {
    public:
        virtual ISpectralImage::ptr upsample(const Image &sourceImage) const = 0;
        virtual ISpectrum::ptr upsample_pixel(const Pixel &src) const = 0;

        virtual ~IUpsampler() {}

        using ptr = std::unique_ptr<IUpsampler>;
        using shared_ptr = std::shared_ptr<IUpsampler>;
    };

}


#endif