#ifndef UPSAMPLERS_SMITS_H
#define UPSAMPLERS_SMITS_H
#include "upsampler.h"

namespace spec {
    class SmitsUpsampler : public IUpsampler
    {
    public:
        ISpectralImage::ptr upsample(const Image &sourceImage) const override;
        ISpectrum::ptr upsample_pixel(const Pixel &src) const override;
        ~SmitsUpsampler() = default;
    };
}
#endif