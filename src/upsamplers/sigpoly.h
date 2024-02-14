#ifndef UPSAMPLERS_SIGPOLY_H
#define UPSAMPLERS_SIGPOLY_H
#include "upsampler.h"

namespace spec {
    class SigPolyUpsampler : public IUpsampler
    {
    public:
        ISpectralImage::ptr upsample(const Image &sourceImage) const override;
        ISpectrum::ptr upsample_pixel(const Pixel &src) const override;
        ~SigPolyUpsampler() = default;
    };
}

#endif