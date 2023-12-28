#ifndef UPSAMPLERS_GLASSNER_NAIVE_H
#define UPSAMPLERS_GLASSNER_NAIVE_H
#include "upsampler.h"

namespace spec {

    class GlassnerUpsampler : public IUpsampler
    {
    public:
        void upsample(const Image &sourceImage, BasicSpectralImage &dest) const override;
        void upsample_pixel_to(const Pixel &pixel, BasicSpectrum &spectre) const override;
        ~GlassnerUpsampler() = default;
    };
}

#endif