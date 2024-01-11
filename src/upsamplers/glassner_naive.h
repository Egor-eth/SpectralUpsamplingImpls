#ifndef UPSAMPLERS_GLASSNER_NAIVE_H
#define UPSAMPLERS_GLASSNER_NAIVE_H
#include "upsampler.h"
#include "spec/basic_spectrum.h"

namespace spec {

    class GlassnerUpsampler : public IUpsampler
    {
    public:
        ISpectralImage::ptr upsample(const Image &sourceImage) const override;
        ISpectrum::ptr upsample_pixel(const Pixel &pixel) const override;
        ~GlassnerUpsampler() = default;
    };
}

#endif